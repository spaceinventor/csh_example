# CSH addin examples

An addin, aping, that is built independent og CSH.

## CSP example

src/main_csp.c illustrates how to create a task that repeatedly calls csp_ping.
This may be tested by loading it into csh that is setup to connect for instance via zmqproxy.

## Slash example

src/main_slash.c illustrates how to create a command, in this case aping that calls csp_ping. When loaded into CSH the aping command appear in the set of CSH commands.


## Meson.build file

There are some special settings required when building a dynamic library for defered linking when loaded as a plugin.

b_lundef=false : All functions from libraries, like csp, are undefined when building the addin, which must be allowed.

-PIC : The library may be loaded on any address, so position independent code is required.

All other is the same.

shared_library( ... ) is used to build the library, .so file.









