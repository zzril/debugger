debugger
========

A (very) simple debugger built in `C` using `ptrace(2)`.

Usage
-----

```sh
./bin/debugger [-b breakpoints] pathname [arguments]
```
* `breakpoints` must be a comma-separated list of addresses.  
* `arguments` are the debugged program's command-line arguments (excluding the  
  0th argument).

Build
-----

Build with `make`, or force a clean build with `make clean all`.

Examples/Tests
--------------

`resources/helloworld` contains a (very basic) helloworld program hand-written  
in assembler.  
* `./run.sh` builds and runs the debugger on that program, with some breakpoints  
set
* `./test.sh` does the same, but tests a few more edge cases


