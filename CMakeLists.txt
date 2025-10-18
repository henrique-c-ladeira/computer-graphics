cmake_minimum_required(VERSION 3.16)
project(computer_graphics LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # emits compile_commands.json

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)
pkg_check_modules(GLM REQUIRED glm)

include_directories(${GLM_INCLUDEDIR} ${GLFW_INCLUDEDIR} ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/include/*.h")

add_executable(main ${SRC_FILES})
target_compile_options(main PRIVATE -Wall -Wextra)
target_compile_options(main PRIVATE ${GLM_CFLAGS_OTHER} ${GLFW_CFLAGS_OTHER})
target_link_libraries(main PRIVATE ${GLM_LIBRARIES} ${GLFW_LIBRARIES} ${GLFW_LDFLAGS})

# On macOS you may need frameworks (GLFW pkg-config may already include them)
if(APPLE)
  find_library(COCOA Cocoa REQUIRED)
  find_library(IOKIT IOKit REQUIRED)
  find_library(OpenGL OpenGL REQUIRED)
  target_link_libraries(main PRIVATE ${COCOA} ${IOKIT} ${OpenGL})
endif()
