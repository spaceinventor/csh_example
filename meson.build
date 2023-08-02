# Project options (such as param:list_dynamic) must be set to the same as in CSH.
project('addin', 'c', subproject_dir: 'lib', default_options: [
	'buildtype=debug', 
	'c_std=gnu11', 
	'b_lto=false',
    'b_lundef=false',
    'param:list_dynamic=true',
   	'default_library=shared'])

add_global_arguments('-I../include', language: 'c')

# It appears that dependencies on 'csp' and 'param' are not strictly required here, when required by 'apm_csh'.
dependencies = []
# dependencies += dependency('csp', fallback: ['csp', 'csp_dep'], required: true).partial_dependency(links: false, includes: true)
# dependencies += dependency('param', fallback: ['param', 'param_dep'], required: true).partial_dependency(links: false, includes: true)
dependencies += dependency('apm_csh', fallback: ['apm_csh', 'apm_csh_dep'], required: true)

addin_param_sources = [
	'src/main_param.c',
]

addin_param_lib = library('csh_example_param',
                         addin_param_sources,
                         dependencies : dependencies,
                         link_args : ['-Wl,-Map=cshmodtest.map', '-lm'],
                         install : true)
