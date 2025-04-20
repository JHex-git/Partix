cmake_minimum_required(VERSION 3.11)
project(Partix)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(PARTIX_BUILD_EXAMPLE "Build example of Partix" ON)
option(PARTIX_BUILD_DEBUG "Build tests of Partix" OFF)

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

add_library(Partix STATIC ${LIB_SOURCES})
target_link_libraries(Partix PUBLIC glm glad stb)
target_include_directories(Partix PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

if (PARTIX_BUILD_EXAMPLE)
    file(GLOB_RECURSE RENDERER_SOURCES src/Renderer/*.cpp src/Renderer/*.h)
    add_library(Renderer STATIC ${RENDERER_SOURCES})
    target_link_libraries(Renderer PUBLIC glfw glad glm stb)
    target_include_directories(Renderer PUBLIC ${CMAKE_SOURCE_DIR}/src)

    if (NOT tinyxml2_FOUND)
        message(STATUS "tinyxml2 not found, build from source")
        include(${CMAKE_SOURCE_DIR}/cmake/tinyxml2.cmake)
        add_subdirectory(3rdparty/tinyxml2)
    endif()
    file(GLOB Emittribute_SOURCES src/Emittribute/*.cpp src/Emittribute/*.h)
    add_library(Emittribute STATIC ${Emittribute_SOURCES})
    target_link_libraries(Emittribute PUBLIC Partix tinyxml2)
    
    # static library will be ignored by linker as these symbols are not referenced
    # so use OBJECT library to compile them into the executable
    file(GLOB Emittribute_Attributes_SOURCES src/Emittribute/Attributes/*.cpp)
    add_library(Emittribute_Attributes OBJECT ${Emittribute_Attributes_SOURCES})
    target_link_libraries(Emittribute_Attributes PUBLIC Emittribute)
    
    add_executable(PartixRenderer src/App/PartixRenderer.cpp)
    target_link_libraries(PartixRenderer PRIVATE Partix Renderer Emittribute Emittribute_Attributes)

    add_executable(StarRain src/App/StarRain.cpp)
    target_link_libraries(StarRain PRIVATE Partix Renderer)

    add_executable(Fountain src/App/Fountain.cpp)
    target_link_libraries(Fountain PRIVATE Partix Renderer)

    if (PARTIX_BUILD_DEBUG)
        add_executable(Sprite src/App/Sprite.cpp)
        target_link_libraries(Sprite PRIVATE Partix Renderer)
    endif()

    # set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT fountain)
endif()

add_custom_command(
    TARGET Partix
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Partix/Shaders $<TARGET_FILE_DIR:Partix>/Shaders
    COMMENT "Copying shaders to output directory"
)

add_custom_command(
    TARGET Partix
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:Partix>/resources
    COMMENT "Copying resources to output directory"
)