cmake_minimum_required (VERSION 3.13)

add_executable (dearpy3d)

set_property(TARGET dearpy3d PROPERTY CXX_STANDARD 17)

target_sources(dearpy3d

	PRIVATE

		"main.cpp"

		# core
		"core/mvWindow.cpp"
		"core/mvTimer.cpp"

		"renderer/mvObjMaterial.cpp"
		"renderer/mvGraphics.cpp"
		"renderer/mvCamera.cpp"

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
)

target_include_directories(dearpy3d

	PRIVATE

		"core/"
		"renderer/"
		"renderer/drawables/"
		"renderer/bindables/"
		"renderer/buffers/"
		"renderer/pipeline/"

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
