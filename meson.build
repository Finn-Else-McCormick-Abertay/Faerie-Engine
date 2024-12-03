project(
    'faerie',
    ['cpp', 'rust'], default_options: ['cpp_std=c++20', 'rust_std=2021'],
    version: '0.0.1'
)

if get_option('buildtype').startswith('debug')
  add_project_arguments('-DDEBUG', language : 'cpp')
endif

# -- Program source --
sh_rt = 'src/shared'
shared_src = []
shared_include = [ sh_rt ]

cl_rt = 'src/client'
client_src = [
    cl_rt/'client_main.cpp',
    cl_rt/'systems/logger.cpp',
    cl_rt/'systems/debug.cpp',
    cl_rt/'systems/input.cpp',
    cl_rt/'input_action.cpp',
    cl_rt/'systems/window.cpp',
    cl_rt/'systems/renderer.cpp',
    cl_rt/'systems/platform/renderer_opengl3.cpp',
    cl_rt/'systems/resource_manager.cpp',
    cl_rt/'systems/script_engine.cpp',
    cl_rt/'systems/ecs.cpp',
    cl_rt/'util/stb.cpp',
    cl_rt/'resources/script.cpp',
    cl_rt/'resources/shader.cpp',
    cl_rt/'resources/texture.cpp',
    cl_rt/'resources/material.cpp',
    cl_rt/'resources/model.cpp',
    cl_rt/'resources/mesh.cpp',
    cl_rt/'components/uuid_comp.cpp',
    cl_rt/'components/hierarchy.cpp',
    cl_rt/'components/transform.cpp',
    cl_rt/'components/camera.cpp',
    cl_rt/'components/mesh.cpp',
    cl_rt/'components/sprite.cpp',
    cl_rt/'components/material.cpp'
]
client_include = [ cl_rt ]
client_pch = 'pch/client_pch.h'
client_args = ['-DOPENGL3']

sv_rt = 'src/server'
server_src = [
    sv_rt/'server_main.cpp'
]
server_include = [ sv_rt ]
server_pch = 'pch/server_pch.h'
server_args = []

# -- Dependencies --
cpp = meson.get_compiler('cpp')

# Found from pkg-config files in msys64/ucrt64/lib/pkgconfig
sdl2 = dependency('sdl2')
opengl = dependency('gl')
glew = dependency('glew')
glm = dependency('glm')
enet = dependency('libenet')
entt = dependency('entt')
assimp = dependency('assimp')

# Library and C api headers found in msys64/ucrt64/lib and include respectively (it doesn't have a pkg-config file)
# but C++ api header is provided locally as the msys2 package doesn't provide it
wasmtime = declare_dependency(
    dependencies: cpp.find_library('wasmtime.dll'),
    include_directories: 'vendor/wasmtime/include',
    compile_args: '-DWASMTIME'
)

# ImGui - provided locally in git submodule
im_rt = 'vendor/imgui'
imgui_src = [
    im_rt/'imgui.cpp', im_rt/'imgui_demo.cpp', im_rt/'imgui_draw.cpp', im_rt/'imgui_tables.cpp', im_rt/'imgui_widgets.cpp',
    im_rt/'backends/imgui_impl_sdl2.cpp', im_rt/'backends/imgui_impl_opengl3.cpp'
]
imgui_include = [ im_rt, im_rt/'backends' ]
imgui_dependencies = [ sdl2, opengl ]
imgui = declare_dependency(
    link_with: static_library('imgui', sources: imgui_src, dependencies: imgui_dependencies, include_directories: imgui_include),
    include_directories: imgui_include
)

cmake = import('cmake')

# vfspp - provided locally in git submodule, built as CMake subproject
vfspp_opt = cmake.subproject_options()
vfspp_opt.set_override_option('cpp_std', 'c++17')
vfspp_opt.append_compile_args('cpp', '-DVFSPP_ENABLE_MULTITHREADING')
vfspp_subproj = cmake.subproject('vfspp', options: vfspp_opt)

vfspp = vfspp_subproj.dependency('vfspp')

# stduuid - header only library, provided locally
stduuid = declare_dependency(include_directories: 'vendor/stduuid')

cxx = dependency('cxx-1-rs')

# -- Targets --

faerie_rust_lib = static_library('faerie-rust', sources: [ 'src/rust/lib.rs' ], rust_abi: 'c', dependencies: [cxx])
faerie_rust_dep = declare_dependency(link_with: faerie_rust_lib, include_directories: 'src/rust/generated/include')

# Compile Rust portion of the engine as static library

#faerie_rust_gen_cpp_target = custom_target('rust-generate-cpp-bindings', command: ['buildscripts/rust_cxxbridge.cmd', '@INPUT@', '@OUTPUT0@', '@OUTPUT1@'],
#    build_always_stale: true, input:'faerie-rust/src/lib.rs', output:['faerie_rust_bindings.h', 'faerie_rust_bindings.cc'], console: true
#)

#cargo = find_program('cargo', native: true, required: true)
#rust_toolchain = 'nightly-x86_64-pc-windows-gnu'
#cargo_command = [
#    cargo, '+'+rust_toolchain, 'build', '--lib', '-Z', 'unstable-options',
#    '--target-dir', meson.current_build_dir()/'@PRIVATE_DIR@', '--artifact-dir', meson.current_build_dir()/'@OUTDIR@'
#]
#faerie_rust_target = custom_target('rust-build', command: ['buildscripts/cd_then.cmd', meson.current_source_dir()/'faerie-rust'] + cargo_command,
#    build_always_stale: true, output: 'libfaerie_rust.a', console: true,
#    depends: faerie_rust_gen_cpp_target
#)

#faerie_rust = declare_dependency( sources: faerie_rust_gen_cpp_target[1], include_directories: ['builddir', 'vendor/cxx'], link_with: faerie_rust_target )

# Copy resources to build directory
run_target('resources', command: ['xcopy', meson.current_source_dir()/'resources', meson.current_build_dir()/'resources', '/s', '/y', '/i'])

# Compile client executable
executable('client',
    sources: shared_src + client_src, include_directories: shared_include + client_include,
    cpp_pch: client_pch, cpp_args: client_args,
    dependencies: [ faerie_rust_dep, sdl2, opengl, glew, glm, imgui, wasmtime, vfspp, enet, entt, stduuid, assimp ]
)
