
# Faerie Engine

CMP316 Coursework

Finn Else-McCormick | 2200864

## Build Instructions

Install [msys2](https://www.msys2.org/)

Add 'C:\msys64\ucrt64\bin' and 'C:\msys64\ucrt64\lib' (assuming default install location) to PATH.

Install [Rust](https://www.rust-lang.org/tools/install)

Install the nightly toolchain for your architecture and compiler. For GCC, this will be nightly-\<architecture\>-pc-windows-gnu:
```
rustup install nightly-x86_64-pc-windows-gnu
```
> It needs to be the nightly version as we use some nightly-only command line options when integrating with Meson.

Install the `cxxbridge-cmd` package with Cargo:
```
cargo install cxxbridge-cmd
```

Use msys2 to install the following tools:
- [gcc](https://gcc.gnu.org/)
- [Meson](https://mesonbuild.com/)
- [pkgconf](http://pkgconf.org/)
- [CMake](https://cmake.org/)

And the following dependencies:
- [SDL2](https://www.libsdl.org/)
- [GLEW](https://glew.sourceforge.net/)
- [glm](https://github.com/g-truc/glm)
- [assimp](https://www.assimp.org/)
- [Wasmtime](https://github.com/bytecodealliance/wasmtime)
- [enet](http://enet.bespin.org/)
- [ENTT](https://github.com/skypjack/entt)

This can be done by running the following commands in the msys2 ucrt64 console:
```
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-meson pkgconf mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-sed
```
```
pacman -S mingw-w64-ucrt-x86_64-SDL2 mingw-w64-ucrt-x86_64-glew mingw-w64-ucrt-x86_64-glm mingw-w64-ucrt-x86_64-enet mingw-w64-ucrt-x86_64-entt mingw-w64-ucrt-x86_64-libwasmtime mingw-w64-ucrt-x86_64-assimp
```

Once the tools and dependencies have been installed, you can set up Meson by running the following command in the project directory:
```
meson setup builddir
```

Once builddir exists, the project can be built with the following command:
```
meson compile -C builddir
```

Note that it will fail to link with the Rust portion the first time it builds, but will work every time after that. It was that or have the Rust portion rebuild every time whether it had changed or not.

> If Meson cannot find the dependencies, check that it is finding the right pkg-config. It will use the first one available in PATH, and libraries like gstreamer will sometimes have their own one.

## VSCode

If working in VSCode, you should install the [C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) and [Meson](https://marketplace.visualstudio.com/items?itemName=mesonbuild.mesonbuild) extensions.

There is a bug in the C++ extension that messes up the intellisense when Meson has dependencies. Until this is fixed, you should use version 1.21.6 of the C++ extension.

In order to debug from VSCode, you will also need [GDB](https://sourceware.org/gdb/).

If you don't already have it, it can be installed through msys2 with the following command:

```
pacman -S mingw-w64-ucrt-x86_64-gdb
```


## Usage
The engine currently runs the script at 'resources/script.wasm' relative to the executable.

The script should be built with [Cargo Component](https://github.com/bytecodealliance/cargo-component) according to the instructions there. The wit file from 'src/wit' should be copied into your component's wit/dep folder and added as a dependency, then imported.

An example can be found [here](https://github.com/throckmorpheus/rust-module-example-component-model).