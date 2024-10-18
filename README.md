
# Untitled Tabletop Engine

CMP316 Coursework

Finn Else-McCormick | 2200864

## Build Instructions

Install [msys2](https://www.msys2.org/)

Add 'C:\msys64\ucrt64\bin' and 'C:\msys64\ucrt64\lib' (assuming default install location) to PATH.

Use msys2 to install the following tools:
- [gcc](https://gcc.gnu.org/)
- [Meson](https://mesonbuild.com/)
- [pkgconf](http://pkgconf.org/)
- [CMake](https://cmake.org/)

And the following dependencies:
- [SDL2](https://www.libsdl.org/)
- [GLEW](https://glew.sourceforge.net/)
- [glm](https://github.com/g-truc/glm)
- [Wasmtime](https://github.com/bytecodealliance/wasmtime)
- [lua](https://www.lua.org/)
- [sol2](https://github.com/ThePhD/sol2)
- [enet](http://enet.bespin.org/)
- [ENTT](https://github.com/skypjack/entt)

This can be done by running the following commands in the msys2 ucrt64 console:
```
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-meson pkgconf mingw-w64-ucrt-x86_64-cmake
```
```
pacman -S mingw-w64-ucrt-x86_64-SDL2 mingw-w64-ucrt-x86_64-glew mingw-w64-ucrt-x86_64-glm mingw-w64-ucrt-x86_64-lua mingw-w64-ucrt-x86_64-sol2 mingw-w64-ucrt-x86_64-enet mingw-w64-ucrt-x86_64-entt mingw-w64-ucrt-x86_64-wasmtime mingw-w64-ucrt-x86_64-libwasmtime
```

Once the tools and dependencies have been installed, you can set up Meson by running the following command in the project directory:
```
meson setup builddir
```

Once builddir exists, the project can be built with the following command:
```
meson compile -C builddir
```

> If Meson cannot find the dependencies, check that it is finding the right pkg-config. It will use the first one available in PATH, and libraries like gstreamer will sometimes have their own one.

## VSCode

If working in VSCode, you should install the [C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) and [Meson](https://marketplace.visualstudio.com/items?itemName=mesonbuild.mesonbuild) extensions.

There is a bug in the C++ extension that messes up the intellisense when Meson has dependencies. Until this is fixed, you should use version 1.21.6 of the C++ extension.

In order to debug from VSCode, you will also need [GDB](https://sourceware.org/gdb/).

If you don't already have it, it can be installed through msys2 with the following command:

```
pacman -S mingw-w64-ucrt-x86_64-gdb
```