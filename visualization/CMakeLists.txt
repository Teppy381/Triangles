# add_custom_command(
# 	OUTPUT "/shaders/simple.frag.spv"
# 	COMMAND "C:/Proga/PACKAGES/VulkanSDK-1.3.275.0/Bin/glslc.exe" shaders/simple.frag
# 					 -o "/shaders/simple.frag.spv"
# 	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/shaders/simple.frag
# 	VERBATIM)

# add_custom_command(TARGET foo
#     PRE_BUILD
#     COMMAND ${CMAKE_COMMAND} -E echo hello
#     COMMENT "This command will be executed before building foo"
#     VERBATIM
#     # no support for depends
#     # no support for output
# )

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

add_compile_definitions(SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/shaders/")

add_executable(vulkan_triangles main.cpp device.cpp)

target_include_directories(vulkan_triangles PUBLIC ${Vulkan_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(vulkan_triangles glm glfw ${Vulkan_LIBRARY})
