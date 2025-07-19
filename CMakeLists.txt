cmake_minimum_required(VERSION 3.16)
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Source files
file(GLOB_RECURSE SRC_FILES src/*.cpp)
file(GLOB IMGUI_SRC
    libs/imgui/*.cpp
    libs/imgui/backends/imgui_impl_glfw.cpp
    libs/imgui/backends/imgui_impl_opengl3.cpp
)
set(GLAD_SRC libs/glad/src/glad.c)

add_executable(OpenGLProject
    ${SRC_FILES}
    ${IMGUI_SRC}
    ${GLAD_SRC}
)

# Find GLFW manually since pkg-config isn't available
set(GLFW_ROOT "/opt/homebrew/opt/glfw")

find_path(GLFW_INCLUDE_DIR NAMES GLFW/glfw3.h PATHS "${GLFW_ROOT}/include")
find_library(GLFW_LIBRARY NAMES glfw PATHS "${GLFW_ROOT}/lib")

if(NOT GLFW_INCLUDE_DIR OR NOT GLFW_LIBRARY)
    message(FATAL_ERROR "Could not find GLFW. Make sure it's installed with Homebrew.")
endif()


# Include directories
target_include_directories(OpenGLProject PRIVATE
    ${GLFW_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/headerFiles
    ${CMAKE_SOURCE_DIR}/libs/glad/include
    ${CMAKE_SOURCE_DIR}/libs        #all other libraries
    /opt/homebrew/include # GLFW and GLM
)

# Link libraries
target_link_libraries(OpenGLProject
    ${GLFW_LIBRARY}
    "-framework OpenGL"
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
)

# Shader and texture path macro
target_compile_definitions(OpenGLProject PRIVATE
    SHADER_DIR=\"resources/Shaders/\"
    TEXTURE_DIR=\"resources/Textures/\"
)


