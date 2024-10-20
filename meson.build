project(
    'untitled_tabletop_engine',
    'cpp', default_options: ['cpp_std=c++20'],
    version: '0.0.1'
)

# -- Program source --
sh_rt = 'src/shared'
shared_src = []
shared_include = [ sh_rt ]

cl_rt = 'src/client'
client_src = [
    cl_rt/'client_main.cpp',
    cl_rt/'systems/system.cpp',
    cl_rt/'systems/window_system.cpp',
    cl_rt/'systems/render_system.cpp',
    cl_rt/'platform/render_system_opengl3.cpp',
    cl_rt/'systems/resource_manager.cpp',
    cl_rt/'systems/scripting_system.cpp',
    cl_rt/'platform/wasmtime/scripting_system_wasmtime.cpp',
    cl_rt/'systems/ecs.cpp',
    cl_rt/'util/stb.cpp',
    cl_rt/'resources/shader.cpp',
    cl_rt/'resources/texture.cpp',
    cl_rt/'resources/script.cpp',
    cl_rt/'components/transform.cpp',
    cl_rt/'components/model.cpp',
    cl_rt/'components/sprite.cpp'
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
    link_with: library('imgui', sources: imgui_src, dependencies: imgui_dependencies, include_directories: imgui_include),
    include_directories: imgui_include
)

# vfspp - provided locally in git submodule, built as CMake subproject
cmake = import('cmake')

vfspp_opt = cmake.subproject_options()
vfspp_opt.set_override_option('cpp_std', 'c++17')
vfspp_opt.append_compile_args('cpp', '-DVFSPP_ENABLE_MULTITHREADING')
vfspp_subproj = cmake.subproject('vfspp', options: vfspp_opt)

vfspp = vfspp_subproj.dependency('vfspp')


# -- Targets --

# Copy resources to build directory
run_target('resources', command: ['buildscripts/copy_resources.cmd'])

# Compile client executable
executable('client',
    sources: shared_src + client_src, include_directories: shared_include + client_include,
    cpp_pch: client_pch, cpp_args: client_args,
    dependencies: [ sdl2, opengl, glew, glm, imgui, wasmtime, vfspp, enet, entt ]
)
