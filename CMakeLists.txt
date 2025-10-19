cmake_minimum_required(VERSION 3.16)
project(computer_graphics LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)
pkg_check_modules(GLM REQUIRED glm)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.cpp")

add_executable(computer_graphics ${SRC_FILES})

target_include_directories(computer_graphics PRIVATE ${INCLUDE_DIR} ${GLM_INCLUDEDIR} ${GLFW_INCLUDEDIR})

target_compile_options(computer_graphics PRIVATE -Wall -Wextra)
target_compile_options(computer_graphics PRIVATE ${GLM_CFLAGS_OTHER} ${GLFW_CFLAGS_OTHER})

target_link_libraries(computer_graphics PRIVATE ${GLM_LIBRARIES} ${GLFW_LIBRARIES} ${GLFW_LDFLAGS})

if(APPLE)
  find_library(COCOA Cocoa REQUIRED)
  find_library(IOKIT IOKit REQUIRED)
  find_library(OpenGL OpenGL REQUIRED)
  target_link_libraries(computer_graphics PRIVATE ${COCOA} ${IOKIT} ${OpenGL})
endif()
