# CSH APM examples

## Slash example, csh_slash

src/main_slash.c illustrates how to create a command, in this case aping that calls csp_ping. 
When csh_csp is loaded into CSH the aping command appear in the set of CSH commands.

## Param example, csh_param

src/main_param.c illustrates how to use parameters. Some static RAM and VMEM in RAM parameters are created. 
When csh_param is loaded into CSH the parameters appear in output from the list command, and set/get works.

# Test

The `init/zmq.csh` file initializes CSP, adds ZMQ device and loads the three example APMs. Observe that `zmqproxy` must be running.

Run `csh -i init/zmq.csh` and observe that the APM is loaded.

## Verification

Run `apm info` to list the loaded APMs along with information output from libinfo function, when defined.

## Param

Run `list` that should list ram_U8, ram_U16, vmem_U8 and vmem_U16 parameters.

## Slash

Run `help` and observe that the `aping` command is listed.

Run `aping 10`.
