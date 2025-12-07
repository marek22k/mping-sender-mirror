# SPDX-FileCopyrightText: Copyright (C) 2024-2025 Marek Küthe <m.k@mk16.de>
#
# SPDX-License-Identifier: GPL-3.0-or-later

project(
    'mping-sender',
    'cpp',
    version: '1.0.0',
    license: ['GPL-3.0-or-later'],
    default_options: [
        'buildtype=release',
        'optimization=2',
        'warning_level=3',
        # 'werror=true',
        'cpp_std=gnu++26',
        'b_asneeded=true',
        'b_lto=true',
        'b_ndebug=if-release',
        'b_pie=true'
    ],
    meson_version: '>=1.5.0'
)

cpp_compiler = meson.get_compiler('cpp')

main_file = files('src/main.cpp')
sources = files(
    'src/mping_sender.cpp',
    'src/multicast_tx_socket.cpp',
    'src/mping_state.cpp',
    'src/posix_wrapper.cpp'
)
headers = files(
    'src/mping_sender.hpp',
    'src/multicast_tx_socket.hpp',
    'src/mping_state.hpp',
    'src/posix_wrapper.hpp'
)
tests_sources = files(
    'tests/mping_state_test.cpp'
)
markdown_files = files(
    'BUILDING.md',
    'CHANGELOG.md',
    'CONTRIBUTING.md',
    'README.md',
    'SECURITY.md'
)
all_sources = main_file + sources + headers + tests_sources

if get_option('native')
    native_flags = ['-march=native', '-mtune=native']

    add_project_arguments(cpp_compiler.get_supported_arguments(native_flags), language: 'cpp')
endif

cpp_compiler.has_header('net/if.h', required: true)
cpp_compiler.has_function('if_nametoindex', prefix: '#include <net/if.h>', required: true)

# Workaround for https://github.com/boostorg/process/issues/494#issuecomment-3392312248
boost_dep = dependency('boost', modules: ['log'], required: true, include_type: 'system')

gtest_dep = dependency('gtest', main: true, required: false, include_type: 'system')

cppcheck = find_program('cppcheck', required: false)
if cppcheck.found()
    run_target('cppcheck', command: [
        cppcheck,
        # Do not check external libraries
        '--suppress=*:*/tins/*',
        '--suppress=*:*/boost/*',
        '--suppress=*:*/gtest/*',

        # Problems with detecting system libraries
        '--suppress=missingIncludeSystem',
        '--suppress=unmatchedSuppression',

        '--enable=warning,style,information,missingInclude',
        '--error-exitcode=1',
        '--check-level=exhaustive',
        '--inline-suppr',
        '--project=' + join_paths(meson.current_build_dir(), 'compile_commands.json')
    ])
endif

infer = find_program('infer', required: false)
if infer.found()
    run_target('infer', command: [
        infer,
        '--compilation-database',
        join_paths(meson.current_build_dir(), 'compile_commands.json'),
        '--fail-on-issue'
    ])
endif

flawfinder = find_program('flawfinder', required: false)
if flawfinder.found()
    run_target('flawfinder', command: [
        flawfinder,
        '--error-level=0',
        all_sources
    ])
endif

lizard = find_program('lizard', required: false)
if lizard.found()
    run_target('lizard', command: [
        lizard,
        '--CCN', '25',
        '--length', '400',
        '--arguments', '10',
        all_sources
    ])
endif

mdl = find_program('mdl', required: false)
if mdl.found()
    run_target('mdl', command: [
        mdl,
        markdown_files
    ])
endif

reuse = find_program('reuse', required: false)
if reuse.found()
    run_target('reuse-annotate', command: [
        reuse,
        'annotate',
        '--copyright', 'Marek Küthe <m.k@mk16.de>',
        '--license', 'GPL-3.0-or-later',
        '--year', '2025',
        '--copyright-prefix', 'spdx-string-c',
        '--merge-copyrights',
        '--recursive',
        '--skip-unrecognised',
        meson.current_source_dir()
    ])
    run_target('reuse-download', command: [
        reuse,
        'download',
        '--all'
    ])
    run_target('reuse-lint', command: [
        reuse,
        'lint'
    ])
endif

if get_option('install_systemd_unit')
    install_data(
        'systemd/crazytrace@.service',
        install_dir: 'lib/systemd/system'
    )
endif

if get_option('install_apparmor_profile')
    install_data(
        'apparmor/usr.bin.crazytrace',
        install_dir: '/etc/apparmor.d'
    )
endif

deps = [boost_dep]

if gtest_dep.found()
    gtest = executable('gtest', sources + tests_sources, dependencies: [gtest_dep] + deps, include_directories: ['src/'])
    test('gtest', gtest)
endif

executable(
    'mping-sender',
    sources + main_file,
    dependencies: deps,
    install : true
)
