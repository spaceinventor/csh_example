# CSH addin examples

An addin, aping, that is built independent og CSH.

## Addin interface

### main

An addin defines a main function, same as in a program, i.e. int main(int argc, char ** argv).

The CSH addin load command passes the dynamic library (.so) file name as first argument. The command allows for passing additional arguments.

### info

The CSH addin info command lists the addins that have been loded. The info function, void info(void), is called for each addin to allow output of addin specific information.

## Meson.build file

There are some special settings required when building a dynamic library for defered linking when loaded as a plugin.

b_lundef=false : All functions from libraries, like csp, are undefined when building the addin, which must be allowed.

-PIC : The library may be loaded on any address, so position independent code is required.

All other is the same.

shared_library( ... ) is used to build the library, .so file.

# Example:

The aping example illustrates callback to library functions found in CSH. In this case the csp_ping function is called. 

## Addin task

A task is created that calls csp_ping every two seconds.

## Slash command

The slash command aping is defined as an example of an addin that extends the set of CSH commands.









