project('nesEmulator', 'cpp', default_options: ['default_library=static', 'cpp_std=c++20'])

sdl2_dep = dependency('sdl2')
incdir = include_directories('include')

srcs = [
  'src/ppu.cpp',
  'src/nesMemory.cpp',
  'src/cpu.cpp',
  'src/window.cpp',
  'src/main.cpp',
]

executable('nes', srcs, dependencies: sdl2_dep, include_directories : incdir)
