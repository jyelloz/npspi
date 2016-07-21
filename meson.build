project(
    'npspi',
    'c',
    default_options: [
        'c_std=gnu99',
    ],
)

cc = meson.get_compiler('c')

libm = cc.find_library('m')
libev = dependency('libev')
mpsse = cc.find_library('mpsse')
ftdi1 = dependency('libftdi1')

deps = [
    mpsse,
    ftdi1,
]

lib = library(
    'npspi',
    [
        'npspi.c',
        'nplut.c',
    ],
    dependencies: deps,
)

executable(
    'npspi',
    'main.c',
    dependencies: deps + [libm, libev],
    link_with: [lib],
)
