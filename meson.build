project(
    'untitled_tabletop_engine',
    'cpp', default_options: ['cpp_std=c++20'],
    version: '0.0.1'
)

# Program source
shared_src = []
shared_include_dir = 'src/shared'

client_src = [
    'src/client/client_main.cpp',
    'src/client/systems/system.cpp',
    'src/client/systems/window_system.cpp',
    'src/client/systems/ecs.cpp',
    'src/client/systems/resource_manager.cpp',
    'src/client/systems/render_system.cpp',
    'src/client/systems/impl/render_system_impl_opengl3.cpp',
    'src/client/systems/impl/scripting_system_impl_lua.cpp',
    'src/client/components/transform.cpp',
    'src/client/components/model.cpp',
    'src/client/components/sprite.cpp',
    'src/client/resources/shader.cpp',
    'src/client/resources/texture.cpp',
    'src/client/util/stb.cpp'
]
client_include_dirs = [ 'src/client' ]
client_pch_path = 'pch/client_pch.h'

server_src = [
    'src/server/server_main.cpp'
]
server_include_dir = 'src/server'
server_pch_path = 'pch/server_pch.h'

# ImGui source
imgui_src = [
    'imgui/imgui.cpp',
    'imgui/imgui_demo.cpp', 'imgui/imgui_draw.cpp',
    'imgui/imgui_tables.cpp', 'imgui/imgui_widgets.cpp',
    'imgui/backends/imgui_impl_sdl2.cpp', 'imgui/backends/imgui_impl_opengl3.cpp'
]
imgui_include_dirs = ['imgui', 'imgui/backends']

cmake = import('cmake')
vfspp_proj = cmake.subproject('vfspp')
vfspp = vfspp_proj.dependency('vfspp')

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
    include_directories: [ shared_include_dir ] + client_include_dirs + imgui_include_dirs,
    cpp_pch: client_pch_path,
    #win_subsystem: 'windows',
    dependencies: [ sdl2, opengl, glew, glm, lua, sol2, enet, entt, vfspp ],
    cpp_args: ['-DOPENGL3 -DSOL_ALL_SAFETIES_ON -DVFSPP_ENABLE_MULTITHREADING']
)
