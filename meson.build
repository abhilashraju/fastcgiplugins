project(
  'crow_core',
  'cpp',
  version: '0.1',
  meson_version: '>=0.58.0',
  default_options: [
    'warning_level=3',
    'cpp_std=c++20',
  ])
fastcgi_dep=dependency('fcgi')
includes=include_directories('/opt/homebrew/Cellar/fcgi/2.4.2/include')
executable('fastcpp',
  sources: ['fastcgi.cpp'],
  include_directories: includes,
  dependencies: fastcgi_dep
)

