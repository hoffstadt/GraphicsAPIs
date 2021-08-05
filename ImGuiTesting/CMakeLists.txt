cmake_minimum_required (VERSION 3.13)

add_executable (imguitesting)

target_sources(imguitesting

	PRIVATE

		"main.cpp"

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

target_include_directories(imguitesting

	PRIVATE

	    "$ENV{VK_SDK_PATH}/Include/"
		"../Dependencies/glfw/include/"
		"../Dependencies/glm/"
		"../Dependencies/stb/"
		"../Dependencies/VulkanMemoryAllocator/"
		"../Dependencies/json/"
		"../Dependencies/imgui/"
		"../Dependencies/imgui/backends/"
)

target_link_directories(imguitesting

	PRIVATE
		
		"$ENV{VK_SDK_PATH}/Lib"

)

target_link_libraries(imguitesting

	PRIVATE

		$<$<PLATFORM_ID:Linux>:vulkan>
		$<$<PLATFORM_ID:Windows>:vulkan-1>
		glfw
)