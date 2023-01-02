# poc-makeswf-04aee52-newBuffer-leak

## Summary
A memory leak occurs when makeswf parse a crafted file with unexpected EOF, and the filename extension is not swf/png/dbl/jpg/jpeg.

## Test Environment
Ubuntu 20.04, 64 bit  
libming (master 04aee52)

## Steps to reproduce
Download the poc file from [here](https://raw.githubusercontent.com/Marsman1996/pocs/master/libming/poc-makeswf-04aee52-newBuffer-leak) and run cmd 
`$ makeswf $POC`

## ASAN report
```
$ ./bin_asan/bin/makeswf poc-makeswf-04aee52-newBuffer-leak
Output file name: out.swf
Output compression level: 9
Output SWF version: 6
Preprocessing poc-04aee52-newBuffer-leak... done.
Parser error: writing empty block

Compiling `out.swf.frame0.pp'... failed:
  Line 1: Reason: 'Unexpected EOF found while looking for input.'


=================================================================
==1424921==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 40 byte(s) in 1 object(s) allocated from:
    #0 0x4c567f in malloc /home/ubuntu178/Downloads/llvm12/projects/compiler-rt/lib/asan/asan_malloc_linux.cpp:145:3
    #1 0x576e0c in newBuffer /home/ubuntu178/cvelibf/test/libming/04aee52/build_asan/src/actioncompiler/../../../code/src/actioncompiler/compile.c:143:23
    #2 0x5baf76 in swf5parse /home/ubuntu178/cvelibf/test/libming/04aee52/build_asan/src/actioncompiler/../../../code/src/actioncompiler/swf5compiler.y:1319:40
    #3 0x511a7d in SWFAction_compile /home/ubuntu178/cvelibf/test/libming/04aee52/build_asan/src/blocks/../../../code/src/blocks/action.c:138:17
    #4 0x5007da in makeswf_compile_source /home/ubuntu178/cvelibf/test/libming/04aee52/build_asan/util/../../code/util/makeswf_utils.c:132:6
    #5 0x4fde3a in main /home/ubuntu178/cvelibf/test/libming/04aee52/build_asan/util/../../code/util/makeswf.c:412:9
    #6 0x7f5a547920b2 in __libc_start_main /build/glibc-eX1tMB/glibc-2.31/csu/../csu/libc-start.c:308:16

Indirect leak of 128 byte(s) in 1 object(s) allocated from:
    #0 0x4c567f in malloc /home/ubuntu178/Downloads/llvm12/projects/compiler-rt/lib/asan/asan_malloc_linux.cpp:145:3
    #1 0x576e65 in newBuffer /home/ubuntu178/cvelibf/test/libming/04aee52/build_asan/src/actioncompiler/../../../code/src/actioncompiler/compile.c:148:23
    #2 0x5baf76 in swf5parse /home/ubuntu178/cvelibf/test/libming/04aee52/build_asan/src/actioncompiler/../../../code/src/actioncompiler/swf5compiler.y:1319:40
    #3 0x511a7d in SWFAction_compile /home/ubuntu178/cvelibf/test/libming/04aee52/build_asan/src/blocks/../../../code/src/blocks/action.c:138:17
    #4 0x5007da in makeswf_compile_source /home/ubuntu178/cvelibf/test/libming/04aee52/build_asan/util/../../code/util/makeswf_utils.c:132:6
    #5 0x4fde3a in main /home/ubuntu178/cvelibf/test/libming/04aee52/build_asan/util/../../code/util/makeswf.c:412:9
    #6 0x7f5a547920b2 in __libc_start_main /build/glibc-eX1tMB/glibc-2.31/csu/../csu/libc-start.c:308:16

SUMMARY: AddressSanitizer: 168 byte(s) leaked in 2 allocation(s).
```

# poc-makeswf-04aee52-openswf-leak.swf

## Summary
A memory leak occurs when makeswf parse a valid swf file, and the filename extension is `.swf`.

## Test Environment
Ubuntu 20.04, 64 bit  
libming (master 04aee52)

## Steps to reproduce
Download the poc file from [here](https://raw.githubusercontent.com/Marsman1996/pocs/master/libming/poc-makeswf-04aee52-openswf-leak.swf) and run cmd 
`$ makeswf $POC`

## ASAN report
```
$ ./bin_asan/bin/makeswf ./poc-makeswf-04aee52-openswf-leak.swf
Output file name: out.swf
Output compression level: 9
Output SWF version: 6
Adding prebuilt clip ./poc-makeswf-04aee52-openswf-leak.swf to frame 0... done.
Saving output to out.swf... done.

=================================================================
==1431036==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 88 byte(s) in 1 object(s) allocated from:
    #0 0x4c567f in malloc /home/ubuntu178/Downloads/llvm12/projects/compiler-rt/lib/asan/asan_malloc_linux.cpp:145:3
    #1 0x523218 in openswf /home/ubuntu178/cvelibf/test/libming/04aee52/build_asan/src/blocks/../../../code/src/blocks/fromswf.c:271:41
    #2 0x5226fe in newSWFPrebuiltClip_fromInput /home/ubuntu178/cvelibf/test/libming/04aee52/build_asan/src/blocks/../../../code/src/blocks/fromswf.c:1302:8
    #3 0x4ff753 in embed_swf /home/ubuntu178/cvelibf/test/libming/04aee52/build_asan/util/../../code/util/makeswf.c:699:14
    #4 0x4fdd89 in main /home/ubuntu178/cvelibf/test/libming/04aee52/build_asan/util/../../code/util/makeswf.c:401:4
    #5 0x7f85ac9be0b2 in __libc_start_main /build/glibc-eX1tMB/glibc-2.31/csu/../csu/libc-start.c:308:16

SUMMARY: AddressSanitizer: 88 byte(s) leaked in 1 allocation(s).
```