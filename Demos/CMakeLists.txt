
if(MV_VULKAN)
	add_subdirectory ("Demo_Vulkan")
endif()


if(MV_OPENGL)
	add_subdirectory ("Demo_OpenGL")
endif()

if(WIN32)

	if(MV_D3D11)
		add_subdirectory ("Demo_Directx11")
		add_subdirectory ("Demo_Directx12")
	endif()
	
elseif(APPLE)
	add_subdirectory("Demo_Metal")
endif()
