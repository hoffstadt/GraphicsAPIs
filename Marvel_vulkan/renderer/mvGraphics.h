#pragma once

#include <vector>
#include <array>
#include <optional>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mvVertexBuffer.h"
#include "mvIndexBuffer.h"

namespace Marvel {

	struct UniformBufferObject 
	{
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};

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
		~mvGraphics();

		void present();

		VkDevice      getDevice();
		std::uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);


	private:

		void createVulkanInstance();
		void setupDebugMessenger();
		void createSurface(GLFWwindow* window);
		void pickPhysicalDevice();
		void createLogicalDevice();
		void createSwapChain(GLFWwindow* window);
		void createImageViews();
		void createRenderPass();
		void createDescriptorSetLayout();
		void createGraphicsPipeline();
		void createFrameBuffers();
		void createCommandPool();
		void createUniformBuffers();
		void updateUniformBuffer(uint32_t currentImage);
		void createDescriptorPool();
		void createDescriptorSets();
		void createCommandBuffers();
		void createSyncObjects();

		// helpers
		bool                    checkValidationLayerSupport();
		bool                    isDeviceSuitable(VkPhysicalDevice device);
		QueueFamilyIndices      findQueueFamilies(VkPhysicalDevice device);
		bool                    checkDeviceExtensionSupport(VkPhysicalDevice device);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
		VkShaderModule          createShaderModule(const std::vector<char>& code);

	private:

		mvVertexBuffer*             _vertexBuffer = nullptr;
		mvIndexBuffer*              _indexBuffer = nullptr;
		std::vector<VkBuffer>       _uniformBuffers;
		std::vector<VkDeviceMemory> _uniformBuffersMemory;

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
		VkShaderModule               _vertShaderModule;
		VkShaderModule               _fragShaderModule;
		VkPipelineLayout             _pipelineLayout;
		VkPipeline                   _graphicsPipeline;
		std::vector<VkFramebuffer>   _swapChainFramebuffers;
		VkCommandPool                _commandPool;
		std::vector<VkCommandBuffer> _commandBuffers;
		std::vector<VkSemaphore>     _imageAvailableSemaphores;
		std::vector<VkSemaphore>     _renderFinishedSemaphores;
		std::vector<VkFence>         _inFlightFences;
		std::vector<VkFence>         _imagesInFlight;
		VkDescriptorSetLayout        _descriptorSetLayout;
		VkDescriptorPool             _descriptorPool;
		std::vector<VkDescriptorSet> _descriptorSets;

		size_t                       _currentFrame = 0;

		const std::vector<const char*> _validationLayers = { "VK_LAYER_KHRONOS_validation"};
		const std::vector<const char*> _deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME};
		const int                      _max_frames_in_flight = 2;
	};

}