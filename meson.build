# Project options (such as param:list_dynamic) must be set to the same as in CSH.
project('apm_example', 'c', subproject_dir: 'lib')

dependencies = []
dependencies += dependency('apm_csh', fallback: ['apm_csh', 'apm_csh_dep'], required: true).as_link_whole()

apm_sources = [
	'src/main_param.c',
	'src/main_slash.c',
]

apm_lib = shared_library('csh_example',
                  apm_sources,
                  dependencies : dependencies,
                  link_args : ['-Wl,-Map=' + meson.project_name() + '.map', '-lm'],
                  install : true)

cspftp_posix_srv = executable(
    'param_example_server',
    [
        'src/main_param.c',
        'src/param_posix_srv.c',
        vcs_tag(input: files('src/version.c.in'), output: 'version.c', command: ['git', 'describe', '--long', '--always', '--dirty=+'])
    ],
    dependencies: [
        dependency('csp', fallback: ['csp', 'csp_dep'], required: true),
        dependency('param', fallback: ['param', 'param_dep'], required: true),
    ],
    link_args : ['-Wl,-Map=' + meson.project_name()  + '_server' + '.map', '-lm', '-lrt'],
    install: false
)
