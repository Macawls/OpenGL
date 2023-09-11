# Running

This project only works on Windows 64-bit 😎👍

**Download** the latest release on the [here](https://github.com/Macawls/OpenGL/releases/latest)

The exe is **portable**, simply double click it to run

# Setup

## Dependencies

* [GLFW](https://www.glfw.org/)
* [GLM](https://glm.g-truc.net/0.9.9/)
* [GLEW](https://glew.sourceforge.net/)
* [IMGUI](https://github.com/ocornut/imgui)

All dependencies are included in the source

## Compiling

1. Follow these [steps](https://code.visualstudio.com/docs/cpp/config-mingw) to setup MinGW-w64 via [MSYS2](https://www.msys2.org/) on Windows

2. Make sure you have python and g++ installed

3. Use `python compiler.py --help` for more information

### Help Menu

```txt
--run       Run the executable
--clean     Clean the build directory
--build     Build the source
--release   Builds for release
```

### Standard Commands

Standard Build

```shell
python compiler.py --build
```

Run after building

```shell
python compiler.py --build --run
```

Build for release (adds rc files, disables debug flags, etc)

```shell
python compiler.py --build --release
```

## Misc

### Using a Custom Font for IMGUI

[Relevant ImGUI Source](https://github.com/ocornut/imgui/blob/master/misc/fonts/binary_to_compressed_c.cpp)

```powershell
.\binary_to_compressed.exe font.ttf font > font.h
```

Make sure the final file is encoded as UTF-8 👍

```cpp
ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(
        font_compressed_data, 
        font_compressed_size, fontSize);
```
