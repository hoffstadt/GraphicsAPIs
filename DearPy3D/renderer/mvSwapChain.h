#pragma once

#include <vector>
#include <memory>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

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

		void resize(float width, float height);

		// setup
		void init(float width, float height);
		void createSurface(GLFWwindow* window);
		void createRenderPass();

		void createFrameBuffers();
		void createDepthResources();
		void createSyncObjects();

		// cleanup
		void freeSyncObjects();
		void freeSurface();

		void beginFrame();
		void present();
		void endFrame();

		uint32_t getCurrentImageIndex() const { return _currentImageIndex; }
		void     draw(uint32_t vertexCount);
		
		VkRenderPassBeginInfo getMainRenderPassInfo();
		VkCommandBuffer       getCurrentCommandBuffer();

		uint32_t     getMinImageCount() { return _minImageCount; }
		VkRenderPass getRenderPass() { return _renderPass; }
		VkExtent2D   getSwapChainExtent() { return _swapChainExtent; }
		VkFormat     getSwatChainFormat() const { return _swapChainImageFormat; }

	private:

		// helpers
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

	private:

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