# Porject options set same as in CSH, except b_lundef=false.
project('addin', 'c', subproject_dir: 'lib', default_options: [
	'buildtype=debug', 
	'c_std=gnu11', 
	'b_lto=false',
    'b_lundef=false'])

add_global_arguments('-I../include', language: 'c')

# Position-independent code is required, as lib may be loaded anywhere.
add_global_arguments('-fPIC', language: 'c')

# Undefined functions must be allowed to defer linking to load time.
add_project_arguments('-Db_lundef=false', language: 'c')

csp_dep = dependency('csp', fallback: ['csp', 'csp_dep'], required: true)
slash_dep = dependency('slash', fallback: ['slash', 'slash_dep'], required: true).as_link_whole()
param_dep = dependency('param', fallback: ['param', 'param_dep'], required: true).as_link_whole()

addin_csp_sources = [
	'src/main_csp.c',
]

addin_csp_lib = shared_library('csh_example_csp',
                         addin_csp_sources,
                         dependencies : [ csp_dep ],
                         link_args : ['-Wl,-Map=cshmodtest.map', '-lm'],
                         install : true)

addin_slash_sources = [
	'src/main_slash.c',
]

addin_slash_lib = shared_library('csh_example_slash',
                         addin_slash_sources,
                         dependencies : [ csp_dep, slash_dep ],
                         link_args : ['-Wl,-Map=cshmodtest.map', '-lm'],
                         install : true)

addin_param_sources = [
	'src/main_param.c',
]

addin_param_lib = shared_library('csh_example_param',
                         addin_param_sources,
                         dependencies : [ csp_dep, slash_dep, param_dep ],
                         link_args : ['-Wl,-Map=cshmodtest.map', '-lm'],
                         install : true)
