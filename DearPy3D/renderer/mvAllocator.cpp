#include "mvAllocator.h"
#include "mvGraphics.h"

namespace DearPy3D {

	VmaAllocator& mvAllocator::GetVmaAllocator()
	{
		static VmaAllocator allocator;
		return allocator;
	}

	void mvAllocator::Init()
	{
		// Initialize VulkanMemoryAllocator
		VmaAllocatorCreateInfo allocatorInfo = {};
		allocatorInfo.vulkanApiVersion = 0;
		allocatorInfo.physicalDevice = mvGraphics::GetContext().getPhysicalDevice();
		allocatorInfo.device = mvGraphics::GetContext().getLogicalDevice();
		allocatorInfo.instance = mvGraphics::GetContext().getInstance();

		vmaCreateAllocator(&allocatorInfo, &GetVmaAllocator());

	}

	void mvAllocator::Shutdown()
	{
		vmaDestroyAllocator(GetVmaAllocator());
	}

	VmaAllocation mvAllocator::allocateBuffer(VkBufferCreateInfo bufferCreateInfo, VmaMemoryUsage usage, VkBuffer& outBuffer)
	{
		VmaAllocationCreateInfo allocCreateInfo = {};
		allocCreateInfo.usage = usage;

		VmaAllocation allocation;
		vmaCreateBuffer(GetVmaAllocator(), &bufferCreateInfo, &allocCreateInfo, &outBuffer, &allocation, nullptr);

		//VmaAllocationInfo allocInfo{};
		//vmaGetAllocationInfo(GetVmaAllocator(), allocation, &allocInfo);

		return allocation;
	}

	VmaAllocation mvAllocator::allocateImage(VkImageCreateInfo imageCreateInfo, VmaMemoryUsage usage, VkImage& outImage)
	{
		VmaAllocationCreateInfo allocCreateInfo = {};
		allocCreateInfo.usage = usage;

		VmaAllocation allocation;
		vmaCreateImage(GetVmaAllocator(), &imageCreateInfo, &allocCreateInfo, &outImage, &allocation, nullptr);

		//VmaAllocationInfo allocInfo;
		//vmaGetAllocationInfo(GetVmaAllocator(), allocation, &allocInfo);

		return allocation;
	}

	void mvAllocator::free(VmaAllocation allocation)
	{
		vmaFreeMemory(GetVmaAllocator(), allocation);
	}

	void mvAllocator::destroyImage(VkImage image, VmaAllocation allocation)
	{
		vmaDestroyImage(GetVmaAllocator(), image, allocation);
	}

	void mvAllocator::destroyBuffer(VkBuffer buffer, VmaAllocation allocation)
	{
		vmaDestroyBuffer(GetVmaAllocator(), buffer, allocation);
	}

	void mvAllocator::unmapMemory(VmaAllocation allocation)
	{
		vmaUnmapMemory(GetVmaAllocator(), allocation);
	}

}