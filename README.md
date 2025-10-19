# Computer Graphics (refactor)

Build and run instructions (macOS with Homebrew):

Prerequisites:

- cmake
- pkg-config
- glfw
- glm

Install with Homebrew:

```bash
brew install cmake pkg-config glfw glm
```

Build:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
./build/computer_graphics
```

To generate compile_commands.json for editor support (clangd):

```bash
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

This project is in early refactor state. Focus is on separating window/context from engine and providing a small renderer layer.
Implementation of a shader program using OpenGL

Run on mac OS
- `brew install glm`
- `brew install glfw3`

add to zshrc:
```
export CPATH=/opt/homebrew/include
export LIBRARY_PATH=/opt/homebrew/lib
```
