project(
    'npspi',
    'c',
    default_options: [
        'c_std=gnu99',
    ],
)

cc = meson.get_compiler('c')

mpsse = cc.find_library('mpsse')
ftdi1 = dependency('libftdi1')

deps = [
    mpsse,
    ftdi1,
]

lib = library(
    'npspi',
    'npspi.c',
    dependencies: deps,
)

executable(
    'npspi',
    'main.c',
    dependencies: deps,
    link_with: [lib],
)
