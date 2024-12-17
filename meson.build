project(
    'faerie',
    ['cpp'], default_options: ['cpp_std=c++20'],
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
    cl_rt/'util/json_archive.cpp',
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
client_pch = 'src/client/pch/client_pch.h'
client_args = ['-DOPENGL3']

sv_rt = 'src/server'
server_src = [
    sv_rt/'server_main.cpp'
]
server_include = [ sv_rt ]
server_pch = 'src/server/pch/server_pch.h'
server_args = []

# -- Dependencies --
sdl2 = dependency('sdl2')
opengl = dependency('gl')
glew = dependency('glew')
glm = dependency('glm')
enet = dependency('libenet')
entt = dependency('entt')
assimp = dependency('assimp')
vfspp = dependency('vfspp-1.0')
jsoncpp = dependency('jsoncpp')
json5cpp = declare_dependency(include_directories: 'vendor/json5cpp')
stduuid = declare_dependency(include_directories: 'vendor/stduuid')

# ImGui - provided locally in git submodule
im_rt = 'vendor/imgui'
imgui_src = [
    im_rt/'imgui.cpp', im_rt/'imgui_demo.cpp', im_rt/'imgui_draw.cpp', im_rt/'imgui_tables.cpp', im_rt/'imgui_widgets.cpp',
    im_rt/'backends/imgui_impl_sdl2.cpp', im_rt/'backends/imgui_impl_opengl3.cpp'
]
imgui_include = [ im_rt, im_rt/'backends' ]
imgui_dependencies = [ sdl2, opengl ]
imgui = declare_dependency(link_with: static_library('imgui', sources: imgui_src, dependencies: imgui_dependencies, include_directories: imgui_include), include_directories: imgui_include)

# -- Targets --

# Compile Rust portion of the engine as static library

cargo = find_program('cargo', native: true, required: true)
rust_architecture = 'x86_64'
rust_compiler = 'pc-windows-gnu'
rust_toolchain = 'nightly-' + rust_architecture + '-' + rust_compiler
cargo_command = [cargo, '+'+rust_toolchain, 'build', '--lib', '-Z', 'unstable-options', '--target-dir', meson.current_build_dir()/'@PRIVATE_DIR@', '--artifact-dir', meson.current_build_dir()/'@OUTDIR@']
if (not get_option('debug'))
    cargo_command += ['--release']
endif

# found in the output of the command 'cargo +nightly-x86_64-pc-windows-gnu rustc -- --print=native-static-libs' (ignore winapi ones)
faerie_rust_link_args = ['-lstdc++', '-lbcrypt', '-ladvapi32', '-lkernel32', '-ladvapi32', '-lntdll', '-luserenv', '-lws2_32', '-ldbghelp']


faerie_rust_target = custom_target('faerie-rust', command: ['buildscripts/cd_then.cmd', meson.current_source_dir()/'src/rust'] + cargo_command,
    input: ['src/rust/src/lib.rs', 'src/rust/src/debug.rs'], output: 'libfaerie_rust.a', console: true
)

# This doesn't run the first time a project is built -> why?
faerie_rust_gen_bindings_target = custom_target('faerie-rust-generate-cpp-bindings', command: ['buildscripts/rust_cxxbridge.cmd', '@INPUT@', '@OUTPUT0@', '@OUTPUT1@'],
    input:'src/rust/src/lib.rs', output:['faerie_rust_bindings.h', 'faerie_rust_bindings.cc'], console: true, depends: [faerie_rust_target]
)

faerie_rust_include = include_directories('vendor/cxx', 'builddir')

faerie_rust_bindings_lib = static_library('faerie-rust-cxx-bindings',
    link_with: faerie_rust_target, link_args: faerie_rust_link_args,
    sources: [faerie_rust_gen_bindings_target[1]], include_directories: faerie_rust_include,
    cpp_pch: 'src/rust/pch/rust_bindings_pch.h'
)
faerie_rust_dep = declare_dependency(link_with: [faerie_rust_target, faerie_rust_bindings_lib], link_args: faerie_rust_link_args, include_directories: faerie_rust_include)

# Copy resources to build directory
run_target('resources', command: ['xcopy', meson.current_source_dir()/'resources', meson.current_build_dir()/'resources', '/s', '/y', '/i'])

# Compile client executable
executable('client',
    sources: shared_src + client_src, include_directories: shared_include + client_include,
    cpp_pch: client_pch, cpp_args: client_args,
    dependencies: [ faerie_rust_dep, sdl2, opengl, glew, glm, imgui, jsoncpp, json5cpp, vfspp, enet, entt, stduuid, assimp ]
)