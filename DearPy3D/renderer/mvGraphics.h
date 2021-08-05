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
#include "mvAllocator.h"

namespace DearPy3D {

	class mvGraphics
	{

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

		mvGraphics(GLFWwindow* window);

		void recreateSwapChain(GLFWwindow* window);
		void cleanupSwapChain();

		VkDevice         getDevice()              { return _device; }
		VkPhysicalDevice getPhysicalDevice()      { return _physicalDevice; }
		VkInstance       getInstance()            { return _instance; }
		VkDescriptorPool getDescriptorPool()      { return _descriptorPool; }
		uint32_t         getMinImageCount()       { return _minImageCount; }
		uint32_t         getGraphicsQueueFamily() { return _graphicsQueueFamily; }
		VkQueue          getGraphicsQueue()       { return _graphicsQueue; }
		VkRenderPass     getRenderPass()          { return _renderPass; }
		VkCommandPool    getCommandPool()         { return _commandPool; }



		// command buffers
		VkCommandBuffer       beginSingleTimeCommands();
		void                  endSingleTimeCommands(VkCommandBuffer commandBuffer);
		VkRenderPassBeginInfo getMainRenderPassInfo();
		VkCommandBuffer       getCurrentCommandBuffer() { return _commandBuffers[_currentImageIndex]; }

		// descriptor pool
		void allocateDescriptorSet(mvDescriptorSet* descriptorSet, mvDescriptorSetLayout& layout);

		// temporary utilities
		uint32_t getCurrentImageIndex() const { return _currentImageIndex; }
		void     beginFrame();
		void     begin();
		void     draw(uint32_t vertexCount);
		void     end();
		void     endFrame();
		void     present();

		void setCamera(glm::mat4 camera) { _camera = camera; }
		void setProjection(glm::mat4 projection) { _projection = projection; }
		glm::mat4 getCamera() const { return _camera; }
		glm::mat4 getProjection() const { return _projection; }

		// resource utilities
		void        createBuffer         (VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void        copyBuffer           (VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		void        transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		void        copyBufferToImage    (VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		VkImageView createImageView      (VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
		void        createImage          (uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

		// misc
		VkExtent2D    getSwapChainExtent();
		std::uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		
	private:

		// initialization
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
		void createCommandPool();
		void createDescriptorPool();

		// internal helpers
		bool                    checkValidationLayerSupport();
		bool                    isDeviceSuitable(VkPhysicalDevice device);		
		bool                    checkDeviceExtensionSupport(VkPhysicalDevice device);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
		QueueFamilyIndices      findQueueFamilies(VkPhysicalDevice device);
		
	private:

		glm::mat4 _camera;
		glm::mat4 _projection;

		uint32_t                       _minImageCount = 0;
		uint32_t                       _currentImageIndex = 0;
		uint32_t                       _graphicsQueueFamily = 0;
		VkImage                        _depthImage;
		VkDeviceMemory                 _depthImageMemory;
		VkImageView                    _depthImageView;
		bool                           _enableValidationLayers = true;
		VkInstance                     _instance;
		VkDebugUtilsMessengerEXT       _debugMessenger;
		VkSurfaceKHR                   _surface;
		VkPhysicalDevice               _physicalDevice = VK_NULL_HANDLE;
		VkDevice                       _device;
		VkCommandPool                  _commandPool;
		VkQueue                        _graphicsQueue;
		VkQueue                        _presentQueue;
		VkSwapchainKHR                 _swapChain;
		std::vector<VkImage>           _swapChainImages;
		std::vector<VkImageView>       _swapChainImageViews;
		VkFormat                       _swapChainImageFormat;
		VkExtent2D                     _swapChainExtent;
		VkRenderPass                   _renderPass;
		std::vector<VkFramebuffer>     _swapChainFramebuffers;
		std::vector<VkSemaphore>       _imageAvailableSemaphores;
		std::vector<VkSemaphore>       _renderFinishedSemaphores;
		std::vector<VkFence>           _inFlightFences;
		std::vector<VkFence>           _imagesInFlight;
		size_t                         _currentFrame = 0;
		const std::vector<const char*> _validationLayers = { "VK_LAYER_KHRONOS_validation"};
		const std::vector<const char*> _deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME};
		const int                      _max_frames_in_flight = 2;
		std::vector<VkCommandBuffer>   _commandBuffers;
		VkDescriptorPool               _descriptorPool;
	};

}