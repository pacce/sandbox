project('sandbox', 'cpp', version : '0.1.0', default_options : ['warning_level=3', 'cpp_std=c++20'])

dependencies  = [
    dependency('geometry')
  , dependency('libpng', required: true)
  # , dependency('openmp')
  ]
sources       = [
  'source/sandbox-png.cpp'
  ]
include       = include_directories('include')
install_headers([])

sandbox = library(
  meson.project_name()
  , sources
  , dependencies        : dependencies
  , include_directories : include
  , install             : true
  )

sandbox_dep = declare_dependency(
  link_with             : sandbox
  , dependencies        : dependencies
  , include_directories : include
  )

if not meson.is_subproject()
  subdir('example')
endif
