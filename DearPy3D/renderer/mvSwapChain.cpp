#include "mvSwapChain.h"
#include <stdexcept>
#include "mvGraphics.h"

namespace DearPy3D {

	void mvSwapChain::init(mvGraphics& graphics, float width, float height)
	{
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(graphics.getPhysicalDevice());

        // choose swap surface Format
        VkSurfaceFormatKHR surfaceFormat = swapChainSupport.formats[0];
        for (const auto& availableFormat : swapChainSupport.formats)
        {
            if (availableFormat.format == VK_FORMAT_R8G8B8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                surfaceFormat = availableFormat;
                break;
            }
        }

        // chose swap present mode
        VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
        for (const auto& availablePresentMode : swapChainSupport.presentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_FIFO_KHR)
            {
                presentMode = availablePresentMode;
                break;
            }
        }

        // chose swap extent
        VkExtent2D extent;
        if (swapChainSupport.capabilities.currentExtent.width != UINT32_MAX)
            extent = swapChainSupport.capabilities.currentExtent;
        else
        {
            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::max(swapChainSupport.capabilities.minImageExtent.width, std::min(swapChainSupport.capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(swapChainSupport.capabilities.minImageExtent.height, std::min(swapChainSupport.capabilities.maxImageExtent.height, actualExtent.height));

            extent = actualExtent;
        }

        _minImageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && _minImageCount > swapChainSupport.capabilities.maxImageCount)
            _minImageCount = swapChainSupport.capabilities.maxImageCount;

        {
            VkSwapchainCreateInfoKHR createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createInfo.surface = _surface;
            createInfo.minImageCount = _minImageCount;
            createInfo.imageFormat = surfaceFormat.format;
            createInfo.imageColorSpace = surfaceFormat.colorSpace;
            createInfo.imageExtent = extent;
            createInfo.imageArrayLayers = 1;
            createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

            mvPhysicalDevice::QueueFamilyIndices indices = graphics.getPhysicalDevice().findQueueFamilies(graphics.getPhysicalDevice());
            uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

            if (indices.graphicsFamily != indices.presentFamily)
            {
                createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                createInfo.queueFamilyIndexCount = 2;
                createInfo.pQueueFamilyIndices = queueFamilyIndices;
            }
            else
                createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

            createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
            createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            createInfo.presentMode = presentMode;
            createInfo.clipped = VK_TRUE;

            createInfo.oldSwapchain = VK_NULL_HANDLE;

            if (vkCreateSwapchainKHR(graphics.getLogicalDevice(), &createInfo, nullptr, &_swapChain) != VK_SUCCESS)
                throw std::runtime_error("failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(graphics.getLogicalDevice(), _swapChain, &_minImageCount, nullptr);
        _swapChainImages.resize(_minImageCount);
        vkGetSwapchainImagesKHR(graphics.getLogicalDevice(), _swapChain, &_minImageCount, _swapChainImages.data());

        _swapChainImageFormat = surfaceFormat.format;
        _swapChainExtent = extent;
        graphics.getDeletionQueue().pushDeletor([=, &graphics]() { vkDestroySwapchainKHR(graphics.getLogicalDevice(), _swapChain, nullptr); });

        // creating image views
        _swapChainImageViews.resize(_swapChainImages.size());
        for (uint32_t i = 0; i < _swapChainImages.size(); i++)
            _swapChainImageViews[i] = graphics.getLogicalDevice().createImageView(_swapChainImages[i], _swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
	}

	void mvSwapChain::createSurface(mvGraphics& graphics, GLFWwindow* window)
	{
		if (glfwCreateWindowSurface(graphics.getInstance(), window, nullptr, &_surface) != VK_SUCCESS)
			throw std::runtime_error("failed to create window surface!");
	}

    void mvSwapChain::setupImGui(mvGraphics& graphics, GLFWwindow* window)
    {
        _imgui = std::make_unique<mvImGuiManager>(window, graphics);
    }

    void mvSwapChain::resize(mvGraphics& graphics, float width, float height)
    {

        init(graphics, width, height);
        graphics.getLogicalDevice().createCommandPool(graphics, graphics.getPhysicalDevice());
        createRenderPass(graphics);
        createDepthResources(graphics);
        createFrameBuffers(graphics);

        _imgui->resize(graphics);
    }

    void mvSwapChain::freeImGui()
    {
        _imgui.reset();
    }

    void mvSwapChain::createSyncObjects(mvGraphics& graphics)
    {

        _imageAvailableSemaphores.resize(_max_frames_in_flight);
        _renderFinishedSemaphores.resize(_max_frames_in_flight);
        _inFlightFences.resize(_max_frames_in_flight);
        _imagesInFlight.resize(_swapChainImages.size(), VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < _max_frames_in_flight; i++)
        {
            if (vkCreateSemaphore(graphics.getLogicalDevice(), &semaphoreInfo, nullptr, &_imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(graphics.getLogicalDevice(), &semaphoreInfo, nullptr, &_renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(graphics.getLogicalDevice(), &fenceInfo, nullptr, &_inFlightFences[i]) != VK_SUCCESS)
                throw std::runtime_error("failed to create synchronization objects for a frame!");
        }

    }

    void mvSwapChain::beginFrame(mvGraphics& graphics)
    {
        vkWaitForFences(graphics.getLogicalDevice(), 1, &_inFlightFences[_currentFrame], VK_TRUE, UINT64_MAX);

        vkAcquireNextImageKHR(graphics.getLogicalDevice(), _swapChain, UINT64_MAX, _imageAvailableSemaphores[_currentFrame],
            VK_NULL_HANDLE, &_currentImageIndex);

        if (_imagesInFlight[_currentImageIndex] != VK_NULL_HANDLE)
            vkWaitForFences(graphics.getLogicalDevice(), 1, &_imagesInFlight[_currentImageIndex], VK_TRUE, UINT64_MAX);

        _imagesInFlight[_currentImageIndex] = _inFlightFences[_currentFrame];
    }

    void mvSwapChain::present(mvGraphics& graphics)
    {

        VkSemaphore signalSemaphores[] = { _renderFinishedSemaphores[_currentFrame] };

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { _swapChain };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &_currentImageIndex;

        vkQueuePresentKHR(graphics.getLogicalDevice().getPresentQueue(), &presentInfo);

        _currentFrame = (_currentFrame + 1) % _max_frames_in_flight;
    }

    void mvSwapChain::begin(mvGraphics& graphics)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(graphics.getLogicalDevice().getCommandBuffers()[_currentImageIndex], &beginInfo) != VK_SUCCESS)
            throw std::runtime_error("failed to begin recording command buffer!");


        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = _renderPass;
        renderPassInfo.framebuffer = _swapChainFramebuffers[_currentImageIndex];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = _swapChainExtent;

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
        clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassInfo.clearValueCount = 2;
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(graphics.getLogicalDevice().getCommandBuffers()[_currentImageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        _imgui->beginFrame(graphics);
    }

    void mvSwapChain::end(mvGraphics& graphics)
    {
        _imgui->endFrame(graphics);

        vkCmdEndRenderPass(graphics.getLogicalDevice().getCommandBuffers()[_currentImageIndex]);

        if (vkEndCommandBuffer(graphics.getLogicalDevice().getCommandBuffers()[_currentImageIndex]) != VK_SUCCESS)
            throw std::runtime_error("failed to record command buffer!");
    }

    void mvSwapChain::draw(mvGraphics& graphics, uint32_t vertexCount)
    {
        vkCmdDrawIndexed(graphics.getLogicalDevice().getCommandBuffers()[_currentImageIndex], vertexCount, 1, 0, 0, 0);
    }

    void mvSwapChain::endFrame(mvGraphics& graphics)
    {
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { _imageAvailableSemaphores[_currentFrame] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &graphics.getLogicalDevice().getCommandBuffers()[_currentImageIndex];

        VkSemaphore signalSemaphores[] = { _renderFinishedSemaphores[_currentFrame] };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkResetFences(graphics.getLogicalDevice(), 1, &_inFlightFences[_currentFrame]);

        if (vkQueueSubmit(graphics.getLogicalDevice().getGraphicsQueue(), 1, &submitInfo, _inFlightFences[_currentFrame]) != VK_SUCCESS)
            throw std::runtime_error("failed to submit draw command buffer!");
    }

    void mvSwapChain::createFrameBuffers(mvGraphics& graphics)
    {
        _swapChainFramebuffers.resize(_swapChainImageViews.size());

        for (size_t i = 0; i < _swapChainImageViews.size(); i++)
        {
            VkImageView attachments[] = {
                _swapChainImageViews[i],
                _depthImageView
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = _renderPass;
            framebufferInfo.attachmentCount = 2;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = _swapChainExtent.width;
            framebufferInfo.height = _swapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(graphics.getLogicalDevice(), &framebufferInfo, nullptr, &_swapChainFramebuffers[i]) != VK_SUCCESS)
                throw std::runtime_error("failed to create framebuffer!");

            graphics.getDeletionQueue().pushDeletor([=, &graphics]() {
                vkDestroyFramebuffer(graphics.getLogicalDevice(), _swapChainFramebuffers[i], nullptr);
                vkDestroyImageView(graphics.getLogicalDevice(), _swapChainImageViews[i], nullptr);
                });
        }
    }

    void mvSwapChain::createDepthResources(mvGraphics& graphics)
    {
        VkFormat depthFormat = VK_FORMAT_D32_SFLOAT;

        graphics.getLogicalDevice().createImage(_swapChainExtent.width, _swapChainExtent.height, depthFormat,
            VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            _depthImage, _depthImageMemory);

        _depthImageView = graphics.getLogicalDevice().createImageView(_depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

        graphics.getDeletionQueue().pushDeletor([=, &graphics]() {
            vkDestroyImageView(graphics.getLogicalDevice(), _depthImageView, nullptr);
            vkDestroyImage(graphics.getLogicalDevice(), _depthImage, nullptr);
            vkFreeMemory(graphics.getLogicalDevice(), _depthImageMemory, nullptr);
            });
    }

    mvSwapChain::SwapChainSupportDetails mvSwapChain::querySwapChainSupport(VkPhysicalDevice device)
    {
        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, _surface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, nullptr);

        // todo: put in appropriate spot
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, 0, _surface, &presentSupport);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    void mvSwapChain::freeSyncObjects(mvGraphics& graphics)
    {
        for (size_t i = 0; i < _max_frames_in_flight; i++)
        {
            vkDestroySemaphore(graphics.getLogicalDevice(), _imageAvailableSemaphores[i], nullptr);
            vkDestroySemaphore(graphics.getLogicalDevice(), _renderFinishedSemaphores[i], nullptr);
            vkDestroyFence(graphics.getLogicalDevice(), _inFlightFences[i], nullptr);
        }
    }

    void mvSwapChain::freeSurface(mvGraphics& graphics)
    {
        vkDestroySurfaceKHR(graphics.getInstance(), _surface, nullptr);
    }

    VkCommandBuffer mvSwapChain::getCurrentCommandBuffer(mvGraphics& graphics)
    { 
        return graphics.getLogicalDevice().getCommandBuffers()[_currentImageIndex]; 
    }

    VkRenderPassBeginInfo mvSwapChain::getMainRenderPassInfo()
    {
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = _renderPass;
        renderPassInfo.framebuffer = _swapChainFramebuffers[_currentImageIndex];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = _swapChainExtent;

        return renderPassInfo;
    }

    void mvSwapChain::createRenderPass(mvGraphics& graphics)
    {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = _swapChainImageFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = VK_FORMAT_D32_SFLOAT;
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcAccessMask = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if (vkCreateRenderPass(graphics.getLogicalDevice(), &renderPassInfo, nullptr, &_renderPass) != VK_SUCCESS)
            throw std::runtime_error("failed to create render pass!");

        graphics.getDeletionQueue().pushDeletor([=, &graphics]() {
            vkFreeCommandBuffers(graphics.getLogicalDevice(),
                graphics.getLogicalDevice().getCommandPool(), 
                static_cast<uint32_t>(graphics.getLogicalDevice().getCommandBuffers().size()), 
                graphics.getLogicalDevice().getCommandBuffers().data());
            vkDestroyRenderPass(graphics.getLogicalDevice(), _renderPass, nullptr); }
        );
    }
}