#pragma once

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace DearPy3D {

	class mvGraphics;

	class mvAllocator
	{

	public:

		static VmaAllocator& GetVmaAllocator();
		static void          Init();
		static void          Shutdown();

	public:

		mvAllocator() = default;
		~mvAllocator() {}

		VmaAllocation allocateBuffer(VkBufferCreateInfo bufferCreateInfo, VmaMemoryUsage usage, VkBuffer& outBuffer);
		VmaAllocation allocateImage (VkImageCreateInfo imageCreateInfo, VmaMemoryUsage usage, VkImage& outImage);
		void          free          (VmaAllocation allocation);
		void          destroyImage  (VkImage image, VmaAllocation allocation);
		void          destroyBuffer (VkBuffer buffer, VmaAllocation allocation);

		template<typename T>
		T* mapMemory(VmaAllocation allocation)
		{
			T* mappedMemory;
			vmaMapMemory(GetVmaAllocator(), allocation, (void**)&mappedMemory);
			return mappedMemory;
		}

		void unmapMemory(VmaAllocation allocation);


	};

}