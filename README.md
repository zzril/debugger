debugger
========

A (very) simple debugger built using ptrace.

Usage
-----

* Build with `make`
* Run with `./bin/debugger [-b breakpoints] pathname [arguments]`

Tests
-----

`resources/helloworld` contains a (very basic) helloworld program hand-written in assembler.  
`./run.sh` runs the debugger on that program, with some breakpoints set.  
`./test.sh` does the same, but tests a few more edge cases.


