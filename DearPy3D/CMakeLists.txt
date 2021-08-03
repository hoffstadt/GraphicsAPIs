cmake_minimum_required (VERSION 3.13)

add_executable (dearpy3d)

set_property(TARGET dearpy3d PROPERTY CXX_STANDARD 17)

target_sources(dearpy3d

	PRIVATE

		"main.cpp"

		"core/mvWindow.cpp"

		"renderer/mvSampler.cpp"
		"renderer/mvTexture.cpp"
		"renderer/mvDescriptorPool.cpp"
		"renderer/mvDescriptorSet.cpp"
		"renderer/mvGraphics.cpp"
		"renderer/mvPipeline.cpp"
		"renderer/mvShader.cpp"
		"renderer/mvVertexBuffer.cpp"
		"renderer/mvIndexBuffer.cpp"
		"renderer/mvVertexLayout.cpp"
)

target_include_directories(dearpy3d

	PRIVATE
		"core/"
		"renderer/"
	    "$ENV{VK_SDK_PATH}/Include/"
		"../Dependencies/glfw/include/"
		"../Dependencies/glm/"
		"../Dependencies/stb/"
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
