include(FetchContent)

message(STATUS "Fetching glm...")
FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/0.9.9.8
)

message(STATUS "Fetching glfw...")
FetchContent_Declare(
	glfw
	GIT_REPOSITORY	https://github.com/glfw/glfw.git
	GIT_TAG 	e2c92645460f680fd272fd2eed591efb2be7dc31 #refs/tags/3.3.9
)

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)

FetchContent_MakeAvailable(glm glfw)
find_package(Vulkan REQUIRED)

message(STATUS "Vulkan_INCLUDE_DIR = ${Vulkan_INCLUDE_DIR}")
message(STATUS "Vulkan_LIBRARY = ${Vulkan_LIBRARY}")

set(SHADERS_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

function(add_shaders TARGET_NAME)
	set(SHADER_SOURCE_FILES ${ARGN}) # the rest of arguments to this function will be assigned as shader source files

	# Validate that source files have been passed
	list(LENGTH SHADER_SOURCE_FILES FILE_COUNT)
	if(FILE_COUNT EQUAL 0)
		message(FATAL_ERROR "Cannot create a shaders target without any source files")
	endif()


	foreach(SHADER_SOURCE IN LISTS SHADER_SOURCE_FILES)
		set(SHADER_COMMAND_LIST)
		cmake_path(ABSOLUTE_PATH SHADER_SOURCE NORMALIZE)
		cmake_path(GET SHADER_SOURCE FILENAME SHADER_NAME)

		# Build command
		list(APPEND SHADER_COMMAND_LIST COMMAND)
		list(APPEND SHADER_COMMAND_LIST Vulkan::glslc)
		list(APPEND SHADER_COMMAND_LIST "${SHADER_SOURCE}")
		list(APPEND SHADER_COMMAND_LIST "-o")
		list(APPEND SHADER_COMMAND_LIST "${SHADERS_OUTPUT_PATH}/${SHADER_NAME}.spv")

		# Add product
		set(SHADER_PRODUCT "${SHADERS_OUTPUT_PATH}/${SHADER_NAME}.spv")

		add_custom_target(${SHADER_NAME} ALL
			${SHADER_COMMAND_LIST}
			COMMENT "Compiling shader ${SHADER_NAME}"
			SOURCES ${SHADER_SOURCE}
			BYPRODUCTS ${SHADER_PRODUCT}
		)
	endforeach()
endfunction()


add_definitions("-O1")
add_compile_definitions(SHADERS_PATH="${SHADERS_OUTPUT_PATH}/")

add_executable(vulkan_triangles main.cpp device.cpp pipeline.cpp swap_chain.cpp first_app.cpp model.cpp renderer.cpp render_system.cpp camera.cpp keyboard_controller.cpp)

add_shaders(vulkan_triangles shaders/simple.frag shaders/simple.vert)

target_include_directories(vulkan_triangles PUBLIC ${Vulkan_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(vulkan_triangles glm glfw ${Vulkan_LIBRARY})
