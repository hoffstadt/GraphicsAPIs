cmake_minimum_required (VERSION 3.13)

function(add_shader SHADER)
    find_program(GLSLC glslc)

    set(current-shader-path "${CMAKE_CURRENT_SOURCE_DIR}/../DearPy3D/shaders/${SHADER}")
    set(current-output-path "${CMAKE_CURRENT_SOURCE_DIR}/../DearPy3D/shaders/${SHADER}.spv")

    # Add a custom command to compile GLSL to SPIR-V.
    get_filename_component(current-output-dir ${current-output-path} DIRECTORY)
    file(MAKE_DIRECTORY ${current-output-dir})

    add_custom_command(
           OUTPUT ${current-output-path}
           COMMAND ${GLSLC} -o ${current-output-path} ${current-shader-path}
           DEPENDS ${current-shader-path}
           IMPLICIT_DEPENDS CXX ${current-shader-path}
           VERBATIM)

    # Make sure our build depends on this output.
    set_source_files_properties(${current-output-path} PROPERTIES GENERATED TRUE)
    target_sources(dearpy3dsandbox PRIVATE ${current-output-path})
endfunction(add_shader)

add_executable (dearpy3dsandbox)

set_property(TARGET dearpy3dsandbox PROPERTY CXX_STANDARD 17)

if(MV_NO_SHADERS)
else()
	add_shader(ps_shader.frag)
	add_shader(ps_solid.frag)
	add_shader(vs_shader.vert)
endif()

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
		"../Dependencies/imgui/"
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
