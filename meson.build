# Porject options set same as in CSH, except b_lundef=false.
project('addin', 'c', subproject_dir: 'lib', default_options: [
	'buildtype=debug', 
	'c_std=gnu11', 
	'b_lto=false',
    'b_lundef=false',
    'param:list_dynamic=true',
   	'default_library=shared'])

add_global_arguments('-I../include', language: 'c')

csp_dep = dependency('csp', fallback: ['csp', 'csp_dep'], required: true).partial_dependency(links: false, includes: true)
param_dep = dependency('param', fallback: ['param', 'param_dep'], required: true).partial_dependency(links: false, includes: true)

addin_param_sources = [
	'src/main_param.c',
]

addin_param_lib = library('csh_example_param',
                         addin_param_sources,
                         dependencies : [ csp_dep, param_dep ],
                         link_args : ['-Wl,-Map=cshmodtest.map', '-lm'],
                         install : true)
