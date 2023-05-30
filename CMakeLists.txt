cmake_minimum_required(VERSION 3.0)
project(SoftRender)

file(MAKE_DIRECTORY output)
file(MAKE_DIRECTORY bin/Debug)
file(MAKE_DIRECTORY bin/Release)

set(publish_bin_debug  ${PROJECT_SOURCE_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${publish_bin_debug})
set(publish_bin_release ${PROJECT_SOURCE_DIR}/bin/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${publish_bin_release})

set(CMAKE_CXX_STANDARD 17)

add_executable(SoftRender
    include/Render.h
    include/stb_image.h
    include/stb_image_write.h
    include/Triangle.h
    include/OBJ_Loader.h
    include/Camera.h
    include/Ray.h

    src/stb_image_write.cpp
    src/stb_image.cpp
    src/main.cpp
    src/Render.cpp
    src/Triangle.cpp
)

target_include_directories(SoftRender
PUBLIC
    include

    ext/glm/include
)

target_compile_options(SoftRender PUBLIC /utf-8) # MSVC unicode support