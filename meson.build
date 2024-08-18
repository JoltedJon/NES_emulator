project('nesEmulator', 'cpp', default_options: ['default_library=static', 'cpp_std=c++20'])

sdl2_dep = dependency('sdl2')
incdir = include_directories('include')

executable('nes', 'src/main.cpp', dependencies: sdl2_dep, include_directories : incdir)
