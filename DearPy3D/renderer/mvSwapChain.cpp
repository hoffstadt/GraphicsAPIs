#include "mvSwapChain.h"
#include <stdexcept>
#include "mvGraphics.h"

namespace DearPy3D {

	void mvSwapChain::init(float width, float height)
	{
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(mvGraphics::GetContext().getPhysicalDevice());

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

            mvPhysicalDevice::QueueFamilyIndices indices = mvGraphics::GetContext().getPhysicalDevice().findQueueFamilies(mvGraphics::GetContext().getPhysicalDevice());
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

            if (vkCreateSwapchainKHR(mvGraphics::GetContext().getLogicalDevice(), &createInfo, nullptr, &_swapChain) != VK_SUCCESS)
                throw std::runtime_error("failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(mvGraphics::GetContext().getLogicalDevice(), _swapChain, &_minImageCount, nullptr);
        _swapChainImages.resize(_minImageCount);
        vkGetSwapchainImagesKHR(mvGraphics::GetContext().getLogicalDevice(), _swapChain, &_minImageCount, _swapChainImages.data());

        _swapChainImageFormat = surfaceFormat.format;
        _swapChainExtent = extent;
        mvGraphics::GetContext().getDeletionQueue().pushDeletor([=]() { vkDestroySwapchainKHR(mvGraphics::GetContext().getLogicalDevice(), _swapChain, nullptr); });

        // creating image views
        _swapChainImageViews.resize(_swapChainImages.size());
        for (uint32_t i = 0; i < _swapChainImages.size(); i++)
            _swapChainImageViews[i] = mvGraphics::GetContext().getLogicalDevice().createImageView(_swapChainImages[i], _swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
	}

	void mvSwapChain::createSurface(GLFWwindow* window)
	{
		if (glfwCreateWindowSurface(mvGraphics::GetContext().getInstance(), window, nullptr, &_surface) != VK_SUCCESS)
			throw std::runtime_error("failed to create window surface!");
	}

    void mvSwapChain::resize(float width, float height)
    {

        init(width, height);
        mvGraphics::GetContext().getLogicalDevice().createCommandPool(mvGraphics::GetContext().getPhysicalDevice());
        createRenderPass();
        createDepthResources();
        createFrameBuffers();
    }

    void mvSwapChain::createSyncObjects()
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
            if (vkCreateSemaphore(mvGraphics::GetContext().getLogicalDevice(), &semaphoreInfo, nullptr, &_imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(mvGraphics::GetContext().getLogicalDevice(), &semaphoreInfo, nullptr, &_renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(mvGraphics::GetContext().getLogicalDevice(), &fenceInfo, nullptr, &_inFlightFences[i]) != VK_SUCCESS)
                throw std::runtime_error("failed to create synchronization objects for a frame!");
        }

    }

    void mvSwapChain::beginFrame()
    {
        vkWaitForFences(mvGraphics::GetContext().getLogicalDevice(), 1, &_inFlightFences[_currentFrame], VK_TRUE, UINT64_MAX);

        vkAcquireNextImageKHR(mvGraphics::GetContext().getLogicalDevice(), _swapChain, UINT64_MAX, _imageAvailableSemaphores[_currentFrame],
            VK_NULL_HANDLE, &_currentImageIndex);

        if (_imagesInFlight[_currentImageIndex] != VK_NULL_HANDLE)
            vkWaitForFences(mvGraphics::GetContext().getLogicalDevice(), 1, &_imagesInFlight[_currentImageIndex], VK_TRUE, UINT64_MAX);

        _imagesInFlight[_currentImageIndex] = _inFlightFences[_currentFrame];
    }

    void mvSwapChain::present()
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

        vkQueuePresentKHR(mvGraphics::GetContext().getLogicalDevice().getPresentQueue(), &presentInfo);

        _currentFrame = (_currentFrame + 1) % _max_frames_in_flight;
    }

    void mvSwapChain::draw(uint32_t vertexCount)
    {
        vkCmdDrawIndexed(mvGraphics::GetContext().getLogicalDevice().getCommandBuffers()[_currentImageIndex], vertexCount, 1, 0, 0, 0);
    }

    void mvSwapChain::endFrame()
    {
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { _imageAvailableSemaphores[_currentFrame] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &mvGraphics::GetContext().getLogicalDevice().getCommandBuffers()[_currentImageIndex];

        VkSemaphore signalSemaphores[] = { _renderFinishedSemaphores[_currentFrame] };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkResetFences(mvGraphics::GetContext().getLogicalDevice(), 1, &_inFlightFences[_currentFrame]);

        if (vkQueueSubmit(mvGraphics::GetContext().getLogicalDevice().getGraphicsQueue(), 1, &submitInfo, _inFlightFences[_currentFrame]) != VK_SUCCESS)
            throw std::runtime_error("failed to submit draw command buffer!");
    }

    void mvSwapChain::createFrameBuffers()
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

            if (vkCreateFramebuffer(mvGraphics::GetContext().getLogicalDevice(), &framebufferInfo, nullptr, &_swapChainFramebuffers[i]) != VK_SUCCESS)
                throw std::runtime_error("failed to create framebuffer!");

            mvGraphics::GetContext().getDeletionQueue().pushDeletor([=]() {
                vkDestroyFramebuffer(mvGraphics::GetContext().getLogicalDevice(), _swapChainFramebuffers[i], nullptr);
                vkDestroyImageView(mvGraphics::GetContext().getLogicalDevice(), _swapChainImageViews[i], nullptr);
                });
        }
    }

    void mvSwapChain::createDepthResources()
    {
        VkFormat depthFormat = VK_FORMAT_D32_SFLOAT;

        mvGraphics::GetContext().getLogicalDevice().createImage(_swapChainExtent.width, _swapChainExtent.height, depthFormat,
            VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            _depthImage, _depthImageMemory);

        _depthImageView = mvGraphics::GetContext().getLogicalDevice().createImageView(_depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

        mvGraphics::GetContext().getDeletionQueue().pushDeletor([=]() {
            vkDestroyImageView(mvGraphics::GetContext().getLogicalDevice(), _depthImageView, nullptr);
            vkDestroyImage(mvGraphics::GetContext().getLogicalDevice(), _depthImage, nullptr);
            vkFreeMemory(mvGraphics::GetContext().getLogicalDevice(), _depthImageMemory, nullptr);
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

    void mvSwapChain::freeSyncObjects()
    {
        for (size_t i = 0; i < _max_frames_in_flight; i++)
        {
            vkDestroySemaphore(mvGraphics::GetContext().getLogicalDevice(), _imageAvailableSemaphores[i], nullptr);
            vkDestroySemaphore(mvGraphics::GetContext().getLogicalDevice(), _renderFinishedSemaphores[i], nullptr);
            vkDestroyFence(mvGraphics::GetContext().getLogicalDevice(), _inFlightFences[i], nullptr);
        }
    }

    void mvSwapChain::freeSurface()
    {
        vkDestroySurfaceKHR(mvGraphics::GetContext().getInstance(), _surface, nullptr);
    }

    VkCommandBuffer mvSwapChain::getCurrentCommandBuffer()
    { 
        return mvGraphics::GetContext().getLogicalDevice().getCommandBuffers()[_currentImageIndex];
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

    void mvSwapChain::createRenderPass()
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

        if (vkCreateRenderPass(mvGraphics::GetContext().getLogicalDevice(), &renderPassInfo, nullptr, &_renderPass) != VK_SUCCESS)
            throw std::runtime_error("failed to create render pass!");

        mvGraphics::GetContext().getDeletionQueue().pushDeletor([=]() {
            vkFreeCommandBuffers(mvGraphics::GetContext().getLogicalDevice(),
                mvGraphics::GetContext().getLogicalDevice().getCommandPool(),
                static_cast<uint32_t>(mvGraphics::GetContext().getLogicalDevice().getCommandBuffers().size()),
                mvGraphics::GetContext().getLogicalDevice().getCommandBuffers().data());
            vkDestroyRenderPass(mvGraphics::GetContext().getLogicalDevice(), _renderPass, nullptr); }
        );
    }
}