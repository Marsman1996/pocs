# poc-makeswf-04aee52-newBuffer-leak (CVE-2023-23051)

## Summary
A memory leak occurs when makeswf parse a crafted file with unexpected EOF, and the filename extension is not swf/png/dbl/jpg/jpeg.

## Test Environment
Ubuntu 20.04, 64 bit  
libming (master 04aee52)

## Steps to reproduce
1. compile libming with ASAN
```
$ CC="clang -fsanitize=address,fuzzer-no-link -g" CFLAGS+=" -fcommon" ./configure 
$ make
```
2. Download the poc file from [here](https://raw.githubusercontent.com/Marsman1996/pocs/master/libming/poc-makeswf-04aee52-newBuffer-leak) and run cmd 
```
$ makeswf $POC
```

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

# poc-makeswf-04aee52-openswf-leak.swf (CVE-2023-23052)

## Summary
A memory leak occurs when makeswf parse a valid swf file, and the filename extension is `.swf`.

## Test Environment
Ubuntu 20.04, 64 bit  
libming (master 04aee52)

## Steps to reproduce
1. compile libming with ASAN
```
$ CC="clang -fsanitize=address,fuzzer-no-link -g" CFLAGS+=" -fcommon" ./configure 
$ make
```
2. Download the poc file from [here](https://raw.githubusercontent.com/Marsman1996/pocs/master/libming/poc-makeswf-04aee52-openswf-leak.swf) and run cmd 
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

# poc-makeswf-04aee52-swf5lex-leak (CVE-2023-23053)

## Summary
A memory leak occurs when makeswf parse a valid ActionScript file, and the filename extension is not swf/png/dbl/jpg/jpeg.

## Test Environment
Ubuntu 20.04, 64 bit  
libming (master 04aee52)

## Steps to reproduce
1. compile libming with ASAN
```
$ CC="clang -fsanitize=address,fuzzer-no-link -g" CFLAGS+=" -fcommon" ./configure 
$ make
```
2. Download the poc file from [here](https://raw.githubusercontent.com/Marsman1996/pocs/master/libming/poc-makeswf-04aee52-swf5lex-leak) and run cmd 
`$ makeswf $POC`

## ASAN report
```
$ ./bin_asan/bin/makeswf ./poc-makeswf-04aee52-swf5lex-leak
Output file name: out.swf
Output compression level: 9
Output SWF version: 6
Preprocessing ./poc-makeswf-04aee52-swf5lex-leak... done.
Compiling `out.swf.frame0.pp'... successfully compiled 108 bytes bytecode.
Adding ./poc-makeswf-04aee52-swf5lex-leak to frame 0... done.
Saving output to out.swf... done.

=================================================================
==1439170==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 10 byte(s) in 1 object(s) allocated from:
    #0 0x43e077 in strdup /home/ubuntu178/Downloads/llvm12/projects/compiler-rt/lib/asan/asan_interceptors.cpp:437:3
    #1 0x58c0bc in swf5lex /home/ubuntu178/cvelibf/test/libming/04aee52/build_asan/src/actioncompiler/../../../code/src/actioncompiler/swf5compiler.flex:360:27
    #2 0x5ab109 in swf5parse /home/ubuntu178/cvelibf/test/libming/04aee52/build_asan/src/actioncompiler/swf5compiler.tab.c:3428:16
    #3 0x511a7d in SWFAction_compile /home/ubuntu178/cvelibf/test/libming/04aee52/build_asan/src/blocks/../../../code/src/blocks/action.c:138:17
    #4 0x5007da in makeswf_compile_source /home/ubuntu178/cvelibf/test/libming/04aee52/build_asan/util/../../code/util/makeswf_utils.c:132:6
    #5 0x4fde3a in main /home/ubuntu178/cvelibf/test/libming/04aee52/build_asan/util/../../code/util/makeswf.c:412:9
    #6 0x7f3391ca90b2 in __libc_start_main /build/glibc-eX1tMB/glibc-2.31/csu/../csu/libc-start.c:308:16

SUMMARY: AddressSanitizer: 10 byte(s) leaked in 1 allocation(s).
```

# poc-API-04aee52-SWFShape_drawCubic-infinite_loop (CVE-2023-23054)

## Summary
An infinite loop occurs when API function SWFShape_drawCubic is given crafted arguments.

## Test Environment
Ubuntu 20.04, 64 bit  
libming (master 04aee52)

## Steps to reproduce
1. compile libming with ASAN
```
$ CC="clang -fsanitize=address,fuzzer-no-link -g" CFLAGS+=" -fcommon" ./configure 
$ make
```
2. Download the [fuzz driver](https://raw.githubusercontent.com/Marsman1996/pocs/master/libming/driver-API-04aee52-SWFShape_drawCubic-infinite_loop.c) and [poc file](https://raw.githubusercontent.com/Marsman1996/pocs/master/libming/poc-API-04aee52-SWFShape_drawCubic-infinite_loop)
3. compile the fuzz driver
```
$ clang -g -fsanitize=address,fuzzer driver-API-04aee52-SWFShape_drawCubic-infinite_loop.c $PATH_TO_ASAN/lib/libming.a -I$PATH_TO_ASAN/include -fcommon -fPIC -DSWF_LITTLE_ENDIAN -DHAVE_CONFIG_H -Wno-all -o driver-API-04aee52-SWFShape_drawCubic-infinite_loop -lz -lpng -lgif
```
4. run the compiled driver 
`$ driver-API-04aee52-SWFShape_drawCubic-infinite_loop $POC`

## ASAN report
```
$ ./driver-API-04aee52-SWFShape_drawCubic-infinite_loop poc-API-04aee52-SWFShape_drawCubic-infinite_loop
INFO: Running with entropic power schedule (0xFF, 100).
INFO: Seed: 810116985
INFO: Loaded 1 modules   (5461 inline 8-bit counters): 5461 [0x8d15c0, 0x8d2b15), 
INFO: Loaded 1 PC tables (5461 PCs): 5461 [0x8d2b18,0x8e8068), 
./driver-API-04aee52-SWFShape_drawCubic-infinite_loop: Running 1 inputs 1 time(s) each.
Running: poc-API-04aee52-SWFShape_drawCubic-infinite_loop
ALARM: working on the last Unit for 1201 seconds
       and the timeout value is 1200 (use -timeout=N to change)
==29507== ERROR: libFuzzer: timeout after 1201 seconds
    #0 0x52bc71 in __sanitizer_print_stack_trace /local/mnt/workspace/bcain_clang_vm-bcain-aus_3184/final/llvm-project/compiler-rt/lib/asan/asan_stack.cpp:86:3
    #1 0x473de8 in fuzzer::PrintStackTrace() /local/mnt/workspace/bcain_clang_vm-bcain-aus_3184/final/llvm-project/compiler-rt/lib/fuzzer/FuzzerUtil.cpp:210:5
    #2 0x458de9 in fuzzer::Fuzzer::AlarmCallback() /local/mnt/workspace/bcain_clang_vm-bcain-aus_3184/final/llvm-project/compiler-rt/lib/fuzzer/FuzzerLoop.cpp:301:5
    #3 0x7f62be1a038f  (/lib/x86_64-linux-gnu/libpthread.so.0+0x1138f)
    #4 0x556007 in subdivideCubicLeft /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:84:24
    #5 0x55954d in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:125:2
    #6 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #7 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #8 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #9 0x55955d in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:126:12
    #10 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #11 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #12 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #13 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #14 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #15 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #16 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #17 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #18 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #19 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #20 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #21 0x55955d in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:126:12
    #22 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #23 0x55955d in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:126:12
    #24 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #25 0x55955d in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:126:12
    #26 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #27 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #28 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #29 0x55955d in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:126:12
    #30 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #31 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #32 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #33 0x55955d in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:126:12
    #34 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #35 0x55955d in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:126:12
    #36 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #37 0x55955d in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:126:12
    #38 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #39 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #40 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #41 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #42 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #43 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #44 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #45 0x55955d in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:126:12
    #46 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #47 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #48 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #49 0x55955d in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:126:12
    #50 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #51 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #52 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #53 0x55955d in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:126:12
    #54 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #55 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #56 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #57 0x55955d in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:126:12
    #58 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #59 0x55955d in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:126:12
    #60 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #61 0x55955d in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:126:12
    #62 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #63 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #64 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #65 0x55955d in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:126:12
    #66 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #67 0x55955d in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:126:12
    #68 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #69 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #70 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #71 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #72 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #73 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #74 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #75 0x55955d in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:126:12
    #76 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #77 0x55955d in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:126:12
    #78 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #79 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #80 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #81 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #82 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #83 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #84 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #85 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #86 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #87 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #88 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #89 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #90 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #91 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #92 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #93 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #94 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #95 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #96 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #97 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #98 0x557bd9 in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:187:10
    #99 0x559585 in subdivideCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:129:13
    #100 0x5570ce in SWFShape_approxCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:159:11
    #101 0x5557e2 in SWFShape_drawScaledCubicTo /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:283:13
    #102 0x558fd4 in SWFShape_drawCubic /opt/disk/marsman/libming/04aee52/build_asan/src/shape_cubic.c:304:9
    #103 0x55439f in AFG_func /opt/disk/marsman/libming/04aee52/out/SWFShape_drawCubic/driver-API-04aee52-SWFShape_drawCubic-infinite_loop.c:35:2
    #104 0x554c1f in LLVMFuzzerTestOneInput /opt/disk/marsman/libming/04aee52/out/SWFShape_drawCubic/driver-API-04aee52-SWFShape_drawCubic-infinite_loop.c:96:2
    #105 0x45a611 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) /local/mnt/workspace/bcain_clang_vm-bcain-aus_3184/final/llvm-project/compiler-rt/lib/fuzzer/FuzzerLoop.cpp:599:15
    #106 0x4442d2 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) /local/mnt/workspace/bcain_clang_vm-bcain-aus_3184/final/llvm-project/compiler-rt/lib/fuzzer/FuzzerDriver.cpp:323:6
    #107 0x44a640 in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) /local/mnt/workspace/bcain_clang_vm-bcain-aus_3184/final/llvm-project/compiler-rt/lib/fuzzer/FuzzerDriver.cpp:856:9
    #108 0x4745c2 in main /local/mnt/workspace/bcain_clang_vm-bcain-aus_3184/final/llvm-project/compiler-rt/lib/fuzzer/FuzzerMain.cpp:20:10
    #109 0x7f62bd4ba83f in __libc_start_main /build/glibc-S7Ft5T/glibc-2.23/csu/../csu/libc-start.c:291
    #110 0x41edd8 in _start (/opt/disk/marsman/libming/04aee52/out/SWFShape_drawCubic/driver-API-04aee52-SWFShape_drawCubic-infinite_loop+0x41edd8)

SUMMARY: libFuzzer: timeout
```