project(
    'untitled_tabletop_engine',
    'cpp', default_options: ['cpp_std=c++17'],
    version: '0.0.1'
)

# Program source
shared_src = []
shared_include_dir = 'src/shared'

client_src = [
    'src/client/client_main.cpp'
]
client_include_dir = 'src/client'

server_src = [
    'src/server/server_main.cpp'
]
server_include_dir = 'src/server'

# ImGui source
imgui_src = [
    'imgui/imgui.cpp', 'imgui/imgui_demo.cpp', 'imgui/imgui_draw.cpp', 'imgui/imgui_tables.cpp', 'imgui/imgui_widgets.cpp',
    'imgui/backends/imgui_impl_allegro5.cpp', 'src/imconfig_allegro5.h'
]
imgui_include_dirs = ['imgui', 'imgui/backends']

imgui_args = ['-IMGUI_USER_CONFIG="src/imconfig_allegro5.h"']

# Dependencies
lua = dependency('lua', version:'>=5.4')
enet = dependency('libenet')
allegro_deps = [
    dependency('allegro-5'),
    dependency('allegro_image-5'),
    dependency('allegro_primitives-5')
]
opengl = dependency('gl')

executable('client',
    sources: shared_src + client_src + imgui_src,
    include_directories: [ shared_include_dir, client_include_dir ] + imgui_include_dirs,
    dependencies: allegro_deps + [ opengl, lua, enet ],
    cpp_args: imgui_args
)
