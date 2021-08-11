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
#include "mvSwapChain.h"

namespace DearPy3D {

	class mvGraphics
	{

	public:

		static void        Init(GLFWwindow* window);
		static void        Shutdown();
		static mvGraphics& GetContext();

	public:

		void init(GLFWwindow* window);
		void cleanup();
		void recreateSwapChain(float width, float height);

		mvSwapChain&      getSwapChain()           { return _swapChain; }
		mvLogicalDevice&  getLogicalDevice()       { return _logicalDevice; }
		mvPhysicalDevice& getPhysicalDevice()      { return _physicalDevice; }
		VkInstance        getInstance()            { return _instance; }
		VkDescriptorPool  getDescriptorPool()      { return _descriptorPool; }
		mvDeletionQueue&  getDeletionQueue()       { return _deletionQueue; }

	private:

		mvGraphics() {};

		// initialization
		void createVulkanInstance();
		void setupDebugMessenger();
		void createDescriptorPool();

		// internal helpers
		bool checkValidationLayerSupport();
		
	private:

		mvLogicalDevice                _logicalDevice;
		mvPhysicalDevice               _physicalDevice;
		mvSwapChain                    _swapChain;
		mvDeletionQueue                _deletionQueue;
		bool                           _enableValidationLayers = true;
		VkInstance                     _instance;
		VkDebugUtilsMessengerEXT       _debugMessenger;
		VkDescriptorPool               _descriptorPool;
		const std::vector<const char*> _validationLayers = { "VK_LAYER_KHRONOS_validation" };
	};

}