project(
    'npspi',
    'c',
    default_options: [
        'c_std=gnu99',
    ],
)

api_version = '1.0'
abi_version = '1.0.0'

cc = meson.get_compiler('c')

libm = cc.find_library('m')
libev = cc.find_library('ev')
mpsse = cc.find_library('mpsse')
ftdi1 = dependency('libftdi1')

deps = [
    mpsse,
    ftdi1,
]

lib_name = 'npspi-' + api_version

lib = library(
    lib_name,
    [
        'npspi.c',
        'nplut.c',
        'npgammalut.c',
    ],
    dependencies: deps,
    install: true,
    version: abi_version,
)

install_headers('npspi.h', subdir: lib_name)

pkgconfig = import('pkgconfig')

pkgconfig.generate(
    libraries: lib,
    subdirs: [lib_name],
    version: '1.0',
    name: lib_name,
    description: 'npspi',
)

executable(
    'npspi',
    'main.c',
    dependencies: deps + [libm, libev],
    link_with: [lib],
)
