#! /bin/sh -e

make
echo
./bin/debugger -b 0x20112b,0x201147 resources/helloworld
echo
./bin/debugger -b 0x20112b,0x201147,0x201151 resources/helloworld
echo
./bin/debugger -b 0x201151 resources/helloworld


