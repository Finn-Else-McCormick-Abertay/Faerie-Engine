project(
    'untitled_tabletop_engine',
    'cpp', default_options: ['cpp_std=c++17'],
    version: '0.0.1'
)

# Program source
src = [
    'src/main.cpp'
]
include_dirs = [ 'include' ]

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
    dependency('allegro_primitives-5')
]

executable('client',
    sources: src + imgui_src,
    include_directories: include_dirs + imgui_include_dirs,
    dependencies: allegro_deps + [ lua, enet ],
    cpp_args: imgui_args
)
