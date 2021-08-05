cmake_minimum_required (VERSION 3.13)

add_library(dearpy3d STATIC)

set_property(TARGET dearpy3d PROPERTY CXX_STANDARD 17)

target_sources(dearpy3d

	PRIVATE

		# core
		"core/mvWindow.cpp"
		"core/mvTimer.cpp"

		# utilities
		"utilities/mvImporter.cpp"

		# renderer
		"renderer/mvDeletionQueue.cpp"
		"renderer/mvAllocator.cpp"
		"renderer/mvObjMaterial.cpp"
		"renderer/mvGraphics.cpp"
		"renderer/mvCamera.cpp"
		"renderer/mvImGuiManager.cpp"

		# pipeline
		"renderer/pipeline/mvPipeline.cpp"
		"renderer/pipeline/mvShader.cpp"
		"renderer/pipeline/mvSampler.cpp"
		"renderer/pipeline/mvDescriptorSet.cpp"
		
		# buffers
		"renderer/buffers/mvTransformUniform.cpp"
		"renderer/buffers/mvVertexBuffer.cpp"
		"renderer/buffers/mvIndexBuffer.cpp"
		"renderer/buffers/mvVertexLayout.cpp"

		# bindables
		"renderer/bindables/mvTexture.cpp"
		
		# drawables
		"renderer/drawables/mvDrawable.cpp"
		"renderer/drawables/mvTexturedQuad.cpp"

		# Vulkan Memory Allocator
		"../Dependencies/VulkanMemoryAllocator/vk_mem_alloc.cpp"

		# imgui
		"../Dependencies/imgui/misc/cpp/imgui_stdlib.cpp"
		"../Dependencies/imgui/imgui.cpp"
		"../Dependencies/imgui/imgui_demo.cpp"
		"../Dependencies/imgui/imgui_draw.cpp"
		"../Dependencies/imgui/imgui_widgets.cpp"
		"../Dependencies/imgui/imgui_tables.cpp"
		"../Dependencies/imgui/backends/imgui_impl_glfw.cpp"
		"../Dependencies/imgui/backends/imgui_impl_vulkan.cpp"
)

target_include_directories(dearpy3d

	PRIVATE

		"core/"
		"utilities/"
		"renderer/"
		"renderer/drawables/"
		"renderer/bindables/"
		"renderer/buffers/"
		"renderer/pipeline/"

	    "$ENV{VK_SDK_PATH}/Include/"
		"../Dependencies/glfw/include/"
		"../Dependencies/glm/"
		"../Dependencies/stb/"
		"../Dependencies/VulkanMemoryAllocator/"
		"../Dependencies/json/"
		"../Dependencies/imgui/"
		"../Dependencies/imgui/backends/"
)

target_link_directories(dearpy3d

	PRIVATE
		
		"$ENV{VK_SDK_PATH}/Lib"

)

target_link_libraries(dearpy3d

	PRIVATE

		$<$<PLATFORM_ID:Linux>:vulkan>
		$<$<PLATFORM_ID:Windows>:vulkan-1>
		glfw
)
