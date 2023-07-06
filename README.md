# CSH addin examples

## CSP example, csh_csp

src/main_csp.c illustrates how to create a task that repeatedly calls csp_ping.
In CSH, when adding a CSP interface, like ZMQ while zmqproxy is running, and loading csh_csp, pings are done.

The addin info command will show statistics.

To display packets use set csp_print_packet 1.

## Slash example, csh_slash

src/main_slash.c illustrates how to create a command, in this case aping that calls csp_ping. 
When csh_csp is loaded into CSH the aping command appear in the set of CSH commands.

## Param example, csh_param

src/main_param.c illustrates how to use parameters. Some static RAM and VMEM in RAM parameters are created. 
When csh_param is loaded into CSH the parameters appear in output from the list command, and set/get works.


## Meson.build file

There are some special settings required when building a dynamic library for defered linking when loaded as a plugin.

b_lundef=false : All functions from libraries, like csp, are undefined when building the addin, which must be allowed.

-PIC : The library may be loaded on any address, so position independent code is required.

All other is the same.

shared_library( ... ) is used to build the library, .so file.

# Test

The `init/zmq.csh` file initializes CSP, adds ZMQ device and loads the three example addins. Observe that `zmqproxy` must be running.

Run `csh -i init/zmq.csh` and observe that the three addins are loaded.

The additional output when loading CSP is output by the libmain function for testing purposes. 

## Verification

Run `addin info` to list the loaded addins along with information output from libinfo function, when defined.

## CSP

Run `addin info csp` to get information on the CSP example that defines a libinfo function that outputs ping statistics.

## Param

Run `list` that should list ram_U8, ram_U16, vmem_U8 and vmem_U16 parameters.

## Slash

Run `help` and observe that the `aping` command is listed.

Run `aping 10`.
