file(GLOB_RECURSE GLAD_SOURCE 3rdparty/glad/*.c)
add_library(glad ${GLAD_SOURCE})
target_include_directories(glad PUBLIC 3rdparty/glad/include)