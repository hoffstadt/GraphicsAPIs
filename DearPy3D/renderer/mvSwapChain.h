#pragma once

#include <vector>
#include <memory>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "mvImGuiManager.h"

namespace DearPy3D {

	class mvGraphics;

	class mvSwapChain
	{

		struct SwapChainSupportDetails 
		{
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};

	public:

		void resize(mvGraphics& graphics, float width, float height);

		// setup
		void init(mvGraphics& graphics, float width, float height);
		void createSurface(mvGraphics& graphics, GLFWwindow* window);
		void setupImGui(mvGraphics& graphics, GLFWwindow* window);
		void createRenderPass(mvGraphics& graphics);

		void createFrameBuffers(mvGraphics& graphics);
		void createDepthResources(mvGraphics& graphics);
		void createSyncObjects(mvGraphics& graphics);

		// cleanup
		void freeSyncObjects(mvGraphics& graphics);
		void freeSurface(mvGraphics& graphics);
		void freeImGui();

		void beginFrame(mvGraphics& graphics);
		void present(mvGraphics& graphics);
		uint32_t getCurrentImageIndex() const { return _currentImageIndex; }
		void     begin(mvGraphics& graphics);
		void     draw(mvGraphics& graphics, uint32_t vertexCount);
		void     end(mvGraphics& graphics);
		void     endFrame(mvGraphics& graphics);

		VkRenderPassBeginInfo getMainRenderPassInfo();
		VkCommandBuffer       getCurrentCommandBuffer(mvGraphics& graphics);

		uint32_t     getMinImageCount() { return _minImageCount; }
		VkRenderPass getRenderPass() { return _renderPass; }
		VkExtent2D   getSwapChainExtent() { return _swapChainExtent; }
		VkFormat     getSwatChainFormat() const { return _swapChainImageFormat; }

	private:

		// helpers
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

	private:

		std::unique_ptr<mvImGuiManager> _imgui = nullptr;
		VkFormat                   _swapChainImageFormat;
		VkSwapchainKHR             _swapChain;
		std::vector<VkImage>       _swapChainImages;
		std::vector<VkImageView>   _swapChainImageViews;
		std::vector<VkFramebuffer> _swapChainFramebuffers;
		VkImage                    _depthImage;
		VkDeviceMemory             _depthImageMemory;
		VkImageView                _depthImageView;
		std::vector<VkSemaphore>   _imageAvailableSemaphores;
		std::vector<VkSemaphore>   _renderFinishedSemaphores;
		std::vector<VkFence>       _inFlightFences;
		std::vector<VkFence>       _imagesInFlight;
		VkRenderPass               _renderPass;
		VkExtent2D                 _swapChainExtent;
		uint32_t                   _minImageCount = 0;
		uint32_t                   _currentImageIndex = 0;
		size_t                     _currentFrame = 0;
		VkSurfaceKHR               _surface;
		const int                  _max_frames_in_flight = 2;
	
	};

}