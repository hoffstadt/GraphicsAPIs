#pragma once

#include <vector>
#include <array>
#include <optional>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mvVertexBuffer.h"
#include "mvIndexBuffer.h"
#include "mvPipeline.h"

namespace Marvel {

	class mvGraphicsContext;
	class mvCommandBuffer;

	class mvDevice
	{

	public:

		struct QueueFamilyIndices {

			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;

			bool isComplete()
			{
				return graphicsFamily.has_value() && presentFamily.has_value();
			}
		};

		struct SwapChainSupportDetails {
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};

	public:

		mvDevice(GLFWwindow* window);
		~mvDevice();

		// temporary utilities
		uint32_t         getCurrentImageIndex() const { return _currentImageIndex; }
		int              getCurrentCommandBufferIndex() const { return _currentBufferIndex; }
		void             setCurrentCommandBufferIndex(int buffer) { _currentBufferIndex = buffer; }
		void             beginpresent(mvGraphicsContext&);
		void             endpresent(mvGraphicsContext& graphics, std::vector<std::shared_ptr<mvCommandBuffer>>& commandBuffers);
		VkFramebuffer    getFrameBuffer(uint32_t index) { return _swapChainFramebuffers[index]; }
		size_t           getSwapChainImageCount() const { return _swapChainImages.size(); }

		// utilities
		void           createBuffer      (VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void           copyBuffer        (mvGraphicsContext&, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		VkShaderModule createShaderModule(const std::vector<char>& code);

		void finish();

		VkDevice           getDevice();
		VkPhysicalDevice   getPhysicalDevice() { return _physicalDevice; }
		VkExtent2D         getSwapChainExtent();
		VkRenderPass       getRenderPass();
		std::uint32_t      findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		VkQueue            getGraphicsQueue() { return _graphicsQueue; }

		void createCommandPool();
		VkCommandBuffer beginSingleTimeCommands();
		void endSingleTimeCommands(VkCommandBuffer commandBuffer);

		VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
		void createImage(uint32_t width, uint32_t height, VkFormat format,
			VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
			VkImage& image, VkDeviceMemory& imageMemory);

		void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	private:

		void createVulkanInstance();
		void setupDebugMessenger();
		void createSurface(GLFWwindow* window);
		void pickPhysicalDevice();
		void createLogicalDevice();
		void createSwapChain(GLFWwindow* window);
		void createImageViews();
		void createRenderPass();
		void createFrameBuffers();
		void createSyncObjects();
		void createDepthResources();


		// helpers
		bool                    checkValidationLayerSupport();
		bool                    isDeviceSuitable(VkPhysicalDevice device);
		
		bool                    checkDeviceExtensionSupport(VkPhysicalDevice device);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
		

	private:

		int           _currentBufferIndex = 0;
		uint32_t      _currentImageIndex = 0;
		VkCommandPool _commandPool;

		// depth buffer
		VkImage        _depthImage;
		VkDeviceMemory _depthImageMemory;
		VkImageView    _depthImageView;

		// options
		bool _enableValidationLayers = true;

		VkInstance                   _instance;
		VkDebugUtilsMessengerEXT     _debugMessenger;
		VkSurfaceKHR                 _surface;
		VkPhysicalDevice             _physicalDevice = VK_NULL_HANDLE;
		VkDevice                     _device;
		VkQueue                      _graphicsQueue;
		VkQueue                      _presentQueue;
		VkSwapchainKHR               _swapChain;
		std::vector<VkImage>         _swapChainImages;
		std::vector<VkImageView>     _swapChainImageViews;
		VkFormat                     _swapChainImageFormat;
		VkExtent2D                   _swapChainExtent;
		VkRenderPass                 _renderPass;
		std::vector<VkFramebuffer>   _swapChainFramebuffers;
		std::vector<VkSemaphore>     _imageAvailableSemaphores;
		std::vector<VkSemaphore>     _renderFinishedSemaphores;
		std::vector<VkFence>         _inFlightFences;
		std::vector<VkFence>         _imagesInFlight;

		size_t                       _currentFrame = 0;

		const std::vector<const char*> _validationLayers = { "VK_LAYER_KHRONOS_validation"};
		const std::vector<const char*> _deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME};
		const int                      _max_frames_in_flight = 2;
	};

}