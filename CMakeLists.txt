cmake_minimum_required(VERSION 3.16)
project(samui)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(
        deps/glad/include
        deps
        src/platforms
        src
)

add_executable(samui src/main.cpp deps/glad/src/glad.c src/platforms/imgui_impl_glfw.cpp src/platforms/imgui_impl_opengl3.cpp
        deps/stb_image/stb_image.cpp deps/stb_image/stb_image.h
        src/platforms/imgui_impl_glfw.h src/platforms/imgui_impl_opengl3.h
        src/common/Camera.cpp src/common/Camera.h
        src/common/gl_errors.cpp src/common/gl_errors.h
        src/common/Application.cpp src/common/Application.h)

target_link_libraries(samui ${CONAN_LIBS})
