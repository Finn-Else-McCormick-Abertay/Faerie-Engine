project(
    'untitled_tabletop_engine',
    'cpp', default_options: ['cpp_std=c++20'],
    version: '0.0.1'
)

# Program source
sh_rt = 'src/shared'
shared_src = []
shared_include_dir = sh_rt

cl_rt = 'src/client'
client_src = [
    cl_rt/'client_main.cpp',
    cl_rt/'systems/system.cpp',
    cl_rt/'systems/window_system.cpp',
    cl_rt/'systems/ecs.cpp',
    cl_rt/'systems/resource_manager.cpp',
    cl_rt/'systems/render_system.cpp',
    cl_rt/'systems/impl/render_system_impl_opengl3.cpp',
    cl_rt/'systems/impl/scripting_system_impl_lua.cpp',
    cl_rt/'components/transform.cpp',
    cl_rt/'components/model.cpp',
    cl_rt/'components/sprite.cpp',
    cl_rt/'resources/shader.cpp',
    cl_rt/'resources/texture.cpp',
    cl_rt/'util/stb.cpp'
]
client_include_dirs = cl_rt
client_pch_path = 'pch/client_pch.h'

sv_rt = 'src/server'
server_src = [
    sv_rt/'server_main.cpp'
]
server_include_dir = sv_rt
server_pch_path = 'pch/server_pch.h'

# ImGui source
im_rt = 'vendor/imgui'
imgui_src = [
    im_rt/'imgui.cpp',
    im_rt/'imgui_demo.cpp',
    im_rt/'imgui_draw.cpp',
    im_rt/'imgui_tables.cpp',
    im_rt/'imgui_widgets.cpp',
    im_rt/'backends/imgui_impl_sdl2.cpp',
    im_rt/'backends/imgui_impl_opengl3.cpp'
]
imgui_include_dirs = [im_rt, im_rt/'backends']

cpp = meson.get_compiler('cpp')
wasmtime = cpp.find_library('wasmtime.dll')
wasmtime_include_dir = 'vendor/wasmtime/include'

# Dependencies
sdl2 = dependency('sdl2')
#sdl2_main = dependency('sdl2main')
opengl = dependency('gl')
glew = dependency('glew')
glm = dependency('glm')
lua = dependency('lua', version:'>=5.4')
sol2 = dependency('sol2')
enet = dependency('libenet')
entt = dependency('entt')

# Copy assets to build directory
run_target('resources', command: ['buildscripts/copy_resources.cmd'])

executable('client',
    sources: shared_src + client_src + imgui_src,
    include_directories: [ shared_include_dir ] + client_include_dirs + imgui_include_dirs + wasmtime_include_dir,
    cpp_pch: client_pch_path,
    #win_subsystem: 'windows',
    dependencies: [ sdl2, opengl, glew, glm, lua, sol2, enet, entt, wasmtime ],
    cpp_args: ['-DOPENGL3 -DSOL_ALL_SAFETIES_ON']
)
