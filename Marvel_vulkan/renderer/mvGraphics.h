#pragma once

#include <vector>
#include <array>
#include <optional>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mvDevice.h"
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
		void createImageViews();
		void createRenderPass();
		void createDescriptorSetLayout();
		void createGraphicsPipeline();
		void createFrameBuffers();
		void createUniformBuffers();
		void updateUniformBuffer(uint32_t currentImage);
		void createDescriptorPool();
		void createDescriptorSets();
		void createCommandBuffers();
		void createSyncObjects();

		// helpers
		VkShaderModule          createShaderModule(const std::vector<char>& code);

	private:

		mvDevice _mvdevice;

		mvVertexLayout              _vertexLayout;
		mvVertexBuffer*             _vertexBuffer = nullptr;
		mvIndexBuffer*              _indexBuffer = nullptr;
		std::vector<VkBuffer>       _uniformBuffers;
		std::vector<VkDeviceMemory> _uniformBuffersMemory;

		VkInstance                   _instance;
		VkDebugUtilsMessengerEXT     _debugMessenger;
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


		const int                      _max_frames_in_flight = 2;
	};

}