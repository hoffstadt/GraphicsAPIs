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
#include "mvDeletionQueue.h"
#include "mvImGuiManager.h"
#include "mvPhysicalDevice.h"
#include "mvLogicalDevice.h"

namespace DearPy3D {

	class mvGraphics
	{

		struct SwapChainSupportDetails {
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};

	public:

		mvGraphics(GLFWwindow* window);

		void cleanup();
		void recreateSwapChain(float width, float height);

		mvPhysicalDevice& getPhysicalDevice()      { return _physicalDevice; }
		mvLogicalDevice&  getLogicalDevice()       { return _logicalDevice; }
		VkInstance        getInstance()            { return _instance; }
		VkDescriptorPool  getDescriptorPool()      { return _descriptorPool; }
		uint32_t          getMinImageCount()       { return _minImageCount; }
		VkRenderPass      getRenderPass()          { return _renderPass; }
		VkExtent2D        getSwapChainExtent()     { return _swapChainExtent; }
		mvDeletionQueue&  getDeletionQueue()       { return _deletionQueue; }

		// command buffers

		VkRenderPassBeginInfo getMainRenderPassInfo();
		VkCommandBuffer       getCurrentCommandBuffer() { return _logicalDevice.getCommandBuffers()[_currentImageIndex]; }

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
		std::uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		
	private:

		// initialization
		void createVulkanInstance();
		void setupDebugMessenger();
		void createSurface(GLFWwindow* window);
		void createSwapChain(float width, float height);
		void createImageViews();
		void createRenderPass();
		void createFrameBuffers();
		void createSyncObjects();
		void createDepthResources();
		void createDescriptorPool();

		// internal helpers
		bool                    checkValidationLayerSupport();
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
		
	private:

		glm::mat4 _camera;
		glm::mat4 _projection;
		std::unique_ptr<mvImGuiManager> _imgui = nullptr;

		mvLogicalDevice                _logicalDevice;
		mvPhysicalDevice               _physicalDevice;
		mvDeletionQueue                _deletionQueue;
		uint32_t                       _minImageCount = 0;
		uint32_t                       _currentImageIndex = 0;
		VkImage                        _depthImage;
		VkDeviceMemory                 _depthImageMemory;
		VkImageView                    _depthImageView;
		bool                           _enableValidationLayers = true;
		VkInstance                     _instance;
		VkDebugUtilsMessengerEXT       _debugMessenger;
		VkSurfaceKHR                   _surface;
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
		const int                      _max_frames_in_flight = 2;
		VkDescriptorPool               _descriptorPool;
		const std::vector<const char*> _validationLayers = { "VK_LAYER_KHRONOS_validation" };
	};

}