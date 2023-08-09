# Project options (such as param:list_dynamic) must be set to the same as in CSH.
project('apm_example', 'c', subproject_dir: 'lib', default_options: [
	'buildtype=debug', 
	'c_std=gnu11', 
	'b_lto=false',
    'b_lundef=false',
    'param:list_dynamic=true',
   	'default_library=shared'])

add_global_arguments('-I../include', language: 'c')

dependencies = []
dependencies += dependency('apm_csh', fallback: ['apm_csh', 'apm_csh_dep'], required: true)

apm_sources = [
	'src/main_param.c',
	'src/main_slash.c',
]

apm_lib = library('csh_example',
                  apm_sources,
                  dependencies : dependencies,
                  link_args : ['-Wl,-Map=cshmodtest.map', '-lm'],
                  install : true)
