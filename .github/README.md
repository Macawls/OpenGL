
# Setup
This project only works on Windows 64-bit 😎👍

## Dependencies
* [GLFW](https://www.glfw.org/)
* [GLM](https://glm.g-truc.net/0.9.9/)
* [GLEW](https://glew.sourceforge.net/)
* [ImGUI](https://github.com/ocornut/imgui)

### Compiling

1. Follow these [steps](https://code.visualstudio.com/docs/cpp/config-mingw) to setup MinGW-w64 via [MSYS2](https://www.msys2.org/) on Windows

2. Make sure you have python and g++ installed

3. Use `python compiler.py --help` for more information

Here are a few commands

```txt
--run       Run the executable after building
--clean     Clean the build directory before building
--build     Build the source
--mwindows  Build with -mwindows flag (no console)
--release   Builds for release
  ```

### Running

The exe is **portable**, simply double click it to run

You can **download** the latest release on the [here](https://github.com/Macawls/OpenGL/releases/latest)

# Misc
## Using a Custom Font for IMGUI

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

