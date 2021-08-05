cmake_minimum_required (VERSION 3.13)

project ("DearPy3D")

# various settings for third party libs
set(GLFW_BUILD_DOCS OFF)
set(BUILD_SHARED_LIBS OFF)
add_definitions(-D_USE_MATH_DEFINES)

add_subdirectory ("Dependencies/glfw")
add_subdirectory ("Dependencies/glm")
add_subdirectory ("DearPy3D")
add_subdirectory ("DearSandbox")

if(WIN32)	
elseif(APPLE)
else() # linux
	add_definitions(-DGLFW_EXPOSE_NATIVE_X11)
endif()