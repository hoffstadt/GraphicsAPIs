cmake_minimum_required (VERSION 3.13)

add_executable (dearpy3dsandbox)

set_property(TARGET dearpy3dsandbox PROPERTY CXX_STANDARD 17)

target_sources(dearpy3dsandbox

	PRIVATE

		"main.cpp"
)

target_include_directories(dearpy3dsandbox

	PRIVATE

		"../DearPy3D/core/"
		"../DearPy3D/renderer/"
		"../DearPy3D/renderer/drawables/"
		"../DearPy3D/renderer/bindables/"
		"../DearPy3D/renderer/buffers/"
		"../DearPy3D/renderer/pipeline/"

	    "$ENV{VK_SDK_PATH}/Include/"
		"../Dependencies/glfw/include/"
		"../Dependencies/glm/"
		"../Dependencies/stb/"
		"../Dependencies/VulkanMemoryAllocator/"
)

target_link_directories(dearpy3dsandbox

	PRIVATE
		
		"$ENV{VK_SDK_PATH}/Lib"

)

target_link_libraries(dearpy3dsandbox

	PRIVATE

		$<$<PLATFORM_ID:Linux>:vulkan>
		$<$<PLATFORM_ID:Windows>:vulkan-1>
		dearpy3d
		glfw
)
