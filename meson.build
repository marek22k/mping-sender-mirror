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
    'src/posix_wrapper.cpp',
    'src/configuration.cpp',
    'src/capability_managment.cpp',
    'src/seccomp.cpp',
    'src/landlock.cpp'
)
headers = files(
    'src/mping_sender.hpp',
    'src/multicast_tx_socket.hpp',
    'src/mping_state.hpp',
    'src/posix_wrapper.hpp',
    'src/network_serialization.hpp',
    'src/configuration.hpp',
    'src/capability_managment.hpp',
    'src/seccomp.hpp',
    'src/landlock.hpp'
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
boost_dep = dependency('boost', modules: ['log', 'program_options'], required: true, include_type: 'system')

libcapng_dep = dependency('libcap-ng', required: false)
if libcapng_dep.found()
    add_project_arguments('-DHAVE_LIBCAPNG', language: 'cpp')
endif

seccomp_dep = dependency('libseccomp', required: false)
if seccomp_dep.found() and cpp_compiler.has_header('sys/syscall.h')
    add_project_arguments('-DHAVE_SECCOMP', language: 'cpp')
endif

if cpp_compiler.has_header('linux/landlock.h')
    add_project_arguments('-DHAVE_LANDLOCK', language: 'cpp')

    landlock_functions = [
        'landlock_create_ruleset',
        'landlock_add_rule',
        'landlock_restrict_self',
    ]

    foreach function : landlock_functions
        if not cpp_compiler.has_function(function, prefix: '#include <linux/landlock.h>')
            cpp_compiler.has_header('unistd.h', required: true)
            cpp_compiler.has_function('syscall', prefix: '#include <unistd.h>', required: true)
            cpp_compiler.has_header('sys/syscall.h', required: true)
            syscall_number_macro = 'SYS_' + function
            cpp_compiler.compiles('#include <sys/syscall.h>\nint number = ' + syscall_number_macro + ';', name: syscall_number_macro + ' number is available', required: true)

            macro = 'NO_' + function.to_upper()
            add_project_arguments('-D' + macro, language: 'cpp')
        endif
    endforeach

    # see also https://lore.kernel.org/landlock/20251119212707.71275873@ciel/T/
    cpp_compiler.compiles(
        '''
            #include <linux/landlock.h>
            struct landlock_ruleset_attr attr = {
                .handled_access_fs = 0,
                .handled_access_net = 0,
                .scoped = 0
            };
        ''',
        name: 'Landlock supports scoped access control',
        required: true
    )

    cpp_compiler.compiles(
        '''
            #include <linux/landlock.h>
            struct landlock_path_beneath_attr attr = {
                .allowed_access = 0,
                .parent_fd = 0
            };
        ''',
        name: 'Landlock has landlock_path_beneath_attr',
        required: true
    )

    cpp_compiler.compiles(
        '''
            #include <linux/landlock.h>
            struct landlock_net_port_attr attr = {
                .allowed_access = 0,
                .port = 0
            };
        ''',
        name: 'Landlock has landlock_net_port_attr',
        required: true
    )

    landlock_macros = [
        'LANDLOCK_RULE_PATH_BENEATH',
        'LANDLOCK_RULE_NET_PORT',

        'LANDLOCK_CREATE_RULESET_VERSION',

        'LANDLOCK_ACCESS_FS_EXECUTE',
        'LANDLOCK_ACCESS_FS_WRITE_FILE',
        'LANDLOCK_ACCESS_FS_READ_FILE',
        'LANDLOCK_ACCESS_FS_TRUNCATE',
        'LANDLOCK_ACCESS_FS_READ_DIR',
        'LANDLOCK_ACCESS_FS_REMOVE_DIR',
        'LANDLOCK_ACCESS_FS_REMOVE_FILE',
        'LANDLOCK_ACCESS_FS_MAKE_CHAR',
        'LANDLOCK_ACCESS_FS_MAKE_DIR',
        'LANDLOCK_ACCESS_FS_MAKE_REG',
        'LANDLOCK_ACCESS_FS_MAKE_SOCK',
        'LANDLOCK_ACCESS_FS_MAKE_FIFO',
        'LANDLOCK_ACCESS_FS_MAKE_BLOCK',
        'LANDLOCK_ACCESS_FS_MAKE_SYM',
        'LANDLOCK_ACCESS_FS_REFER',
        'LANDLOCK_ACCESS_FS_IOCTL_DEV',

        'LANDLOCK_ACCESS_NET_BIND_TCP',
        'LANDLOCK_ACCESS_NET_CONNECT_TCP',

        'LANDLOCK_SCOPE_ABSTRACT_UNIX_SOCKET',
        'LANDLOCK_SCOPE_SIGNAL'
    ]

    foreach macro : landlock_macros
        cpp_compiler.compiles('#include <linux/landlock.h>\nint number = ' + macro + ';', name: macro + ' number is available', required: true)
    endforeach

    if cpp_compiler.compiles('#include <linux/landlock.h>\nint number = LANDLOCK_RESTRICT_SELF_LOG_NEW_EXEC_ON;', name: 'LANDLOCK_RESTRICT_SELF_LOG_NEW_EXEC_ON number is available')
        add_project_arguments('-DHAVE_LANDLOCK_RESTRICT_SELF_LOG_NEW_EXEC_ON', language: 'cpp')
    endif
endif

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

deps = [boost_dep, libcapng_dep, seccomp_dep]

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
