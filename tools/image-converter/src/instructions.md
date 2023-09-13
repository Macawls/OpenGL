### Converting Images

From the glfw [documentation](https://www.glfw.org/docs/3.3/group__window.html#gadd7ccd39fe7a7d1f0904666ae5932dc5),

The pixels are 32-bit, little-endian, non-premultiplied RGBA, i.e. eight bits per channel with the red channel first. They are arranged canonically as packed sequential rows, starting from the top-left corner"

I figured since ```stbi_load``` works seamlessly you can use the following binary to write image data to a .h file.

```bash
.\image_to_header.exe name.png output.h
```
