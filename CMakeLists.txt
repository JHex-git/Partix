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

find_package(assimp QUIET)
if (NOT assimp_FOUND)
    message(STATUS "assimp not found, build from source")
    include(cmake/assimp.cmake)
    add_subdirectory(3rdparty/assimp)
    target_compile_options(assimp PUBLIC /wd4819)
    # if (MSVC)
    # endif()
endif()


include(${CMAKE_SOURCE_DIR}/cmake/stb.cmake)

file(GLOB_RECURSE LIB_SOURCES src/Partix/*.cpp src/Partix/*.h)

add_library(Partix STATIC ${LIB_SOURCES})
target_link_libraries(Partix PUBLIC glm glad stb assimp)
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
    file(GLOB Attrix_SOURCES src/Attrix/*.cpp src/Attrix/*.h)
    add_library(Attrix STATIC ${Attrix_SOURCES})
    target_link_libraries(Attrix PUBLIC Partix tinyxml2)
    
    # static library will be ignored by linker as these symbols are not referenced
    # so use OBJECT library to compile them into the executable
    file(GLOB Attrix_Emitter_SOURCES src/Attrix/Emitter/*.cpp)
    file(GLOB Attrix_Particle_SOURCES src/Attrix/Particle/*.cpp)
    add_library(Attrix_Attributes OBJECT ${Attrix_Emitter_SOURCES} ${Attrix_Particle_SOURCES})
    target_link_libraries(Attrix_Attributes PUBLIC Attrix)
    
    add_executable(PartixRenderer src/App/PartixRenderer.cpp)
    target_link_libraries(PartixRenderer PRIVATE Partix Renderer Attrix Attrix_Attributes)

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

set(SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/Partix/Shaders/)
# Another Option : Use glslangValidator to do preprocess
# glslangValidator.exe $file  -P"#extension GL_GOOGLE_include_directive:enable" -I${SHADER_DIR} -E 1>$<TARGET_FILE_DIR:Partix>/Shaders/$file
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND PowerShell -ExecutionPolicy Bypass -File
                "${CMAKE_CURRENT_SOURCE_DIR}/src/Partix/Scripts/PreprocessShader.ps1"
                -shaderPath "${SHADER_DIR}"
                -outputRootPath "$<TARGET_FILE_DIR:Partix>/Shaders"
                -cl "${CMAKE_C_COMPILER}"
        COMMENT "Preprocessing macro and include in shaders"
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND sh "${CMAKE_CURRENT_SOURCE_DIR}/src/Partix/Scripts/PreprocessShader.sh" "${SHADER_DIR}" "$<TARGET_FILE_DIR:Partix>/"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT "Preprocessing macro and include in shaders"
        VERBATIM
    )
endif()

add_custom_command(
    TARGET Partix
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:Partix>/resources
    COMMENT "Copying resources to output directory"
)