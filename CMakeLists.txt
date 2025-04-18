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

include(${CMAKE_SOURCE_DIR}/cmake/stb.cmake)

file(GLOB_RECURSE LIB_SOURCES src/Partix/*.cpp src/Partix/*.h)
file(GLOB_RECURSE RENDERER_SOURCES src/Renderer/*.cpp src/Renderer/*.h)

add_library(Partix STATIC ${LIB_SOURCES})
target_link_libraries(Partix PUBLIC glm glad stb)
target_include_directories(Partix PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

if (PARTIX_BUILD_EXAMPLE)
    add_library(Renderer STATIC ${RENDERER_SOURCES})
    target_link_libraries(Renderer PUBLIC glfw glad glm stb)
    target_include_directories(Renderer PUBLIC ${CMAKE_SOURCE_DIR}/src)

    add_executable(fountain src/Example/fountain.cpp)
    target_link_libraries(fountain PRIVATE Partix Renderer)

    add_executable(StarRain src/Example/StarRain.cpp)
    target_link_libraries(StarRain PRIVATE Partix Renderer)

    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT fountain)
endif()

add_custom_command(
    TARGET Partix
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Partix/Shaders $<TARGET_FILE_DIR:Partix>/
    COMMENT "Copying shaders to output directory"
)