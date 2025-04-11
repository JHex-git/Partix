cmake_minimum_required(VERSION 3.11)
project(Partix)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(PARTIX_BUILD_EXAMPLE "Build example of Partix" ON)

include(cmake/glad.cmake)

find_package(glm QUIET)
if (NOT GLM_FOUND)
    message(STATUS "GLM not found, build from source")
    add_subdirectory(3rdparty/glm)
endif()

find_package(glfw QUIET)
if (NOT glfw_FOUND)
    message(STATUS "glfw not found, build from source")
    include(cmake/glfw.cmake)
    add_subdirectory(3rdparty/glfw)
endif()

file(GLOB_RECURSE LIB_SOURCES src/Partix/*.cpp src/Partix/*.h)
file(GLOB_RECURSE RENDERER_SOURCES src/Renderer/*.cpp src/Renderer/*.h)

add_library(Partix STATIC ${LIB_SOURCES})
target_link_libraries(Partix PUBLIC glm glad)
target_include_directories(Partix PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

if (PARTIX_BUILD_EXAMPLE)
    add_library(Renderer STATIC ${RENDERER_SOURCES})
    target_link_libraries(Renderer PUBLIC glfw glad glm)
    target_include_directories(Renderer PUBLIC ${CMAKE_SOURCE_DIR}/src)

    add_executable(rain src/Example/rain.cpp)
    target_link_libraries(rain PRIVATE Partix Renderer)

    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT rain)
endif()

set(GLSLC ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glslc.exe)
set(SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/Partix/Shaders)
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND PowerShell -ExecutionPolicy Bypass -File
            "${CMAKE_CURRENT_SOURCE_DIR}/src/Partix/Scripts/PreprocessShader.ps1"
            -shaderPath "${SHADER_DIR}"
            -outputRootPath "$<TARGET_FILE_DIR:Partix>/Shaders"
            -cl "${CMAKE_C_COMPILER}"
    COMMENT "Preprocessing macro and include in shaders"
)