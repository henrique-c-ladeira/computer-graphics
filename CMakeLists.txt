cmake_minimum_required(VERSION 3.16)
project(computer_graphics LANGUAGES C CXX OBJCXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable Objective-C++ for Metal support
set(CMAKE_OBJCXX_STANDARD 17)
set(CMAKE_OBJCXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)
pkg_check_modules(GLM REQUIRED glm)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SHADER_DIR ${CMAKE_SOURCE_DIR}/shaders)

# We explicitly list our source files to avoid including removed OpenGL files
set(SRC_FILES
    ${SRC_DIR}/engine.mm
    ${SRC_DIR}/main.mm
    ${SRC_DIR}/MetalRenderer.mm
    ${SRC_DIR}/Window.cpp
)

# Copy Metal shaders to build directory
file(GLOB METAL_SHADERS "${SHADER_DIR}/*.metal")
file(COPY ${METAL_SHADERS} DESTINATION ${CMAKE_BINARY_DIR}/shaders)

add_executable(computer_graphics ${SRC_FILES})

target_include_directories(computer_graphics PRIVATE ${INCLUDE_DIR} ${GLM_INCLUDEDIR} ${GLFW_INCLUDEDIR})

target_compile_options(computer_graphics PRIVATE -Wall -Wextra)
target_compile_options(computer_graphics PRIVATE ${GLM_CFLAGS_OTHER} ${GLFW_CFLAGS_OTHER})

target_link_libraries(computer_graphics PRIVATE ${GLM_LIBRARIES} ${GLFW_LIBRARIES} ${GLFW_LDFLAGS})

if(APPLE)
    find_library(COCOA Cocoa REQUIRED)
    find_library(IOKIT IOKit REQUIRED)
    find_library(METAL Metal REQUIRED)
    find_library(METALKIT MetalKit REQUIRED)
    find_library(QUARTZ_CORE QuartzCore REQUIRED)
    target_link_libraries(computer_graphics PRIVATE 
        ${COCOA} 
        ${IOKIT} 
        ${METAL}
        ${METALKIT}
        ${QUARTZ_CORE}
    )

    # Create shaders directory in build
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/shaders)

    # Add custom target to compile Metal shaders
    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/shaders/shader.metallib
        COMMAND xcrun -sdk macosx metal -c ${SHADER_DIR}/shader.metal -o ${CMAKE_BINARY_DIR}/shaders/shader.air
        COMMAND xcrun -sdk macosx metallib ${CMAKE_BINARY_DIR}/shaders/shader.air -o ${CMAKE_BINARY_DIR}/shaders/shader.metallib
        DEPENDS ${SHADER_DIR}/shader.metal
        COMMENT "Compiling Metal shaders"
        VERBATIM
    )

    add_custom_target(compile_shaders ALL
        DEPENDS ${CMAKE_BINARY_DIR}/shaders/shader.metallib
    )

    # Make sure shaders are compiled before the main executable
    add_dependencies(computer_graphics compile_shaders)
endif()
