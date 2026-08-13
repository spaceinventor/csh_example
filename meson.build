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
