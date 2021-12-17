# poc20-idct2d8x8-SEGV (CVE-2019-16350)
a NULL Pointer Dereference bug
## Test Environment
Ubuntu 14.04, 64bit, ffjpeg(master 627c8a9)

## How to trigger
1. compile ffjpeg with cmake file from https://github.com/rockcarry/ffjpeg/issues/6
2. `$ ./ffjpeg -d $POC`

## Reference
https://github.com/rockcarry/ffjpeg/issues/10  
https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-16350

## Credits
Yanhao(unfuzzable123@gmail.com)  
Marsman1996(lqliuyuwei@outlook.com)  
Yuanpingyu(cfenicey@gmail.com)

## Details
### Asan report
```
ASAN:SIGSEGV
=================================================================
==21545== ERROR: AddressSanitizer: SEGV on unknown address 0x000000000000 (pc 0x00000040db78 sp 0x7ffd4681f2e0 bp 0x7ffd4681f340 T0)
AddressSanitizer can not provide additional info.
    #0 0x40db77 in idct2d8x8 /home/aota10/MARS_fuzzcompare/test/ffjpeg/code/dct.c:201
    #1 0x40605b in jfif_decode /home/aota10/MARS_fuzzcompare/test/ffjpeg/code/jfif.c:508
    #2 0x401a70 in main /home/aota10/MARS_fuzzcompare/test/ffjpeg/code/ffjpeg.c:25
    #3 0x7f8448218f44 (/lib/x86_64-linux-gnu/libc.so.6+0x21f44)
    #4 0x401858 in _start (/home/aota10/MARS_fuzzcompare/test/ffjpeg/bin_asan/bin/ffjpeg+0x401858)
SUMMARY: AddressSanitizer: SEGV /home/aota10/MARS_fuzzcompare/test/ffjpeg/code/dct.c:201 idct2d8x8
==21545== ABORTING
```

### GDB report
```
Program received signal SIGSEGV, Segmentation fault.
0x0000000000406402 in idct2d8x8 (data=0x7fffffffe070, ftab=0xffffffff)
    at /home/aota10/MARS_fuzzcompare/test/ffjpeg/code/dct.c:201
201             data[ctr] *= ftab[ctr];
(gdb) bt
#0  0x0000000000406402 in idct2d8x8 (data=0x7fffffffe070, ftab=0xffffffff)
    at /home/aota10/MARS_fuzzcompare/test/ffjpeg/code/dct.c:201
#1  0x0000000000403435 in jfif_decode (ctxt=0x60a010, pb=0x7fffffffe1a0)
    at /home/aota10/MARS_fuzzcompare/test/ffjpeg/code/jfif.c:508
#2  0x0000000000401672 in main (argc=3, argv=0x7fffffffe2a8)
    at /home/aota10/MARS_fuzzcompare/test/ffjpeg/code/ffjpeg.c:25
```

# poc21-huffman_decode_step-SEGV (CVE-2019-16351)

## Test Environment
Ubuntu 14.04, 64bit, ffjpeg(master 627c8a9)

## How to trigger
1. compile ffjpeg with cmake file from https://github.com/rockcarry/ffjpeg/issues/6
2. `$ ./ffjpeg -d $POC`

## Reference
https://github.com/rockcarry/ffjpeg/issues/11  
https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-16351

## Credits
Yanhao(unfuzzable123@gmail.com)  
Marsman1996(lqliuyuwei@outlook.com)
Yuanpingyu(cfenicey@gmail.com)

## Details
### Asan report
```
ASAN:SIGSEGV
=================================================================
==19241== ERROR: AddressSanitizer: SEGV on unknown address 0x000000001590 (pc 0x000000410e8b sp 0x7fff54e12780 bp 0x7fff54e127a0 T0)
AddressSanitizer can not provide additional info.
    #0 0x410e8a in huffman_decode_step /home/aota10/MARS_fuzzcompare/test/ffjpeg/code/huffman.c:371
    #1 0x405f04 in jfif_decode /home/aota10/MARS_fuzzcompare/test/ffjpeg/code/jfif.c:493
    #2 0x401a70 in main /home/aota10/MARS_fuzzcompare/test/ffjpeg/code/ffjpeg.c:25
    #3 0x7f8d44273f44 (/lib/x86_64-linux-gnu/libc.so.6+0x21f44)
    #4 0x401858 in _start (/home/aota10/MARS_fuzzcompare/test/ffjpeg/bin_asan/bin/ffjpeg+0x401858)
SUMMARY: AddressSanitizer: SEGV /home/aota10/MARS_fuzzcompare/test/ffjpeg/code/huffman.c:371 huffman_decode_step
==19241== ABORTING
```

### GDB report
```
Program received signal SIGSEGV, Segmentation fault.
0x000000000040775f in huffman_decode_step (phc=0x0)
    at /home/aota10/MARS_fuzzcompare/test/ffjpeg/code/huffman.c:371
371         if (!phc->input) return EOF;
(gdb) bt
#0  0x000000000040775f in huffman_decode_step (phc=0x0)
    at /home/aota10/MARS_fuzzcompare/test/ffjpeg/code/huffman.c:371
#1  0x0000000000403357 in jfif_decode (ctxt=0x60a010, pb=0x7fffffffe190)
    at /home/aota10/MARS_fuzzcompare/test/ffjpeg/code/jfif.c:493
#2  0x0000000000401672 in main (argc=3, argv=0x7fffffffe298)
    at /home/aota10/MARS_fuzzcompare/test/ffjpeg/code/ffjpeg.c:25
```

# poc22-jfif_load-heapoverflow (CVE-2019-16352)
a heap-buffer-overflow bug
## Test Environment
Ubuntu 14.04, 64bit, ffjpeg(master 627c8a9)

## How to trigger
1. compile ffjpeg with cmake file from https://github.com/rockcarry/ffjpeg/issues/6
2. `$ ./ffjpeg -d $POC`

## Reference
https://github.com/rockcarry/ffjpeg/issues/12  
https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-16352

## Credits
Yanhao(unfuzzable123@gmail.com)  
Marsman1996(lqliuyuwei@outlook.com)
Yuanpingyu(cfenicey@gmail.com)

## Details
### Asan report
```
=================================================================
==17308== ERROR: AddressSanitizer: heap-buffer-overflow on address 0x60340000fef8 at pc 0x402fec bp 0x7ffc051db980 sp 0x7ffc051db978
WRITE of size 4 at 0x60340000fef8 thread T0
    #0 0x402feb in jfif_load /home/aota10/MARS_fuzzcompare/test/ffjpeg/code/jfif.c:187
    #1 0x401a59 in main /home/aota10/MARS_fuzzcompare/test/ffjpeg/code/ffjpeg.c:24
    #2 0x7f5291e9bf44 (/lib/x86_64-linux-gnu/libc.so.6+0x21f44)
    #3 0x401858 in _start (/home/aota10/MARS_fuzzcompare/test/ffjpeg/bin_asan/bin/ffjpeg+0x401858)
0x60340000fef8 is located 0 bytes to the right of 504-byte region [0x60340000fd00,0x60340000fef8)
allocated by thread T0 here:
    #0 0x7f52922584e5 (/usr/lib/x86_64-linux-gnu/libasan.so.0+0x154e5)
    #1 0x40293b in jfif_load /home/aota10/MARS_fuzzcompare/test/ffjpeg/code/jfif.c:154
    #2 0x401a59 in main /home/aota10/MARS_fuzzcompare/test/ffjpeg/code/ffjpeg.c:24
    #3 0x7f5291e9bf44 (/lib/x86_64-linux-gnu/libc.so.6+0x21f44)
SUMMARY: AddressSanitizer: heap-buffer-overflow /home/aota10/MARS_fuzzcompare/test/ffjpeg/code/jfif.c:187 jfif_load
Shadow bytes around the buggy address:
  0x0c06ffff9f80: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c06ffff9f90: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c06ffff9fa0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c06ffff9fb0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c06ffff9fc0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x0c06ffff9fd0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00[fa]
  0x0c06ffff9fe0:fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c06ffff9ff0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c06ffffa000: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c06ffffa010: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c06ffffa020: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07 
  Heap left redzone:     fa
  Heap righ redzone:     fb
  Freed Heap region:     fd
  Stack left redzone:    f1
  Stack mid redzone:     f2
  Stack right redzone:   f3
  Stack partial redzone: f4
  Stack after return:    f5
  Stack use after scope: f8
  Global redzone:        f9
  Global init order:     f6
  Poisoned by user:      f7
  ASan internal:         fe
==17308== ABORTING
```

### GDB report
```
Starting program: /home/aota10/MARS_fuzzcompare/test/ffjpeg/build_normal/ffjpeg -d poc22-jfif_load-heapoverflow 
file eof !
*** Error in `/home/aota10/MARS_fuzzcompare/test/ffjpeg/build_normal/ffjpeg': munmap_chunk(): invalid pointer: 0x000000000060a210 ***

Program received signal SIGABRT, Aborted.
0x00007ffff7a47c37 in __GI_raise (sig=sig@entry=6) at ../nptl/sysdeps/unix/sysv/linux/raise.c:56
56      ../nptl/sysdeps/unix/sysv/linux/raise.c: No such file or directory.
(gdb) bt
#0  0x00007ffff7a47c37 in __GI_raise (sig=sig@entry=6) at ../nptl/sysdeps/unix/sysv/linux/raise.c:56
#1  0x00007ffff7a4b028 in __GI_abort () at abort.c:89
#2  0x00007ffff7a842a4 in __libc_message (do_abort=do_abort@entry=1, 
    fmt=fmt@entry=0x7ffff7b96350 "*** Error in `%s': %s: 0x%s ***\n") at ../sysdeps/posix/libc_fatal.c:175
#3  0x00007ffff7a8f007 in malloc_printerr (action=<optimized out>, 
    str=0x7ffff7b966d0 "munmap_chunk(): invalid pointer", ptr=<optimized out>) at malloc.c:4998
#4  0x0000000000402416 in jfif_load (file=0x7fffffffe58c "poc22-jfif_load-heapoverflow")
    at /home/aota10/MARS_fuzzcompare/test/ffjpeg/code/jfif.c:257
#5  0x000000000040165b in main (argc=3, argv=0x7fffffffe298)
    at /home/aota10/MARS_fuzzcompare/test/ffjpeg/code/ffjpeg.c:24
(gdb) 
```

# poc-ffjpeg-d5cfd49-jfif_encode-SEGV
This segment fault error is because in `bmp_load()`, when bmp's size is out of range, it returns without assign memory buffer to `pb->pdata` and did not exit the program. 
So the program crashes when it tries to access the `pb->data`, which is a invalid memory address.

## Test Environment
Ubuntu 16.04, 64bit  
ffjpeg (master d5cfd49)

## How to trigger
`$ ./ffjpeg -e $POC`

## Reference
https://github.com/rockcarry/ffjpeg/issues/47  


## Credits
Yanhao(unfuzzable123@gmail.com)  
Marsman1996(lqliuyuwei@outlook.com)

## Details

### ASAN report
```
./bin_asan/bin/ffjpeg -e poc-ffjpeg-d5cfd49-jfif_encode-SEGV
bmp's width * height is out of range !
AddressSanitizer:DEADLYSIGNAL
=================================================================
==2671==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000002 (pc 0x000000520b78 bp 0x7fffe13204d0 sp 0x7fffe131fc80 T0)
==2671==The signal is caused by a READ memory access.
==2671==Hint: address points to the zero page.
    #0 0x520b77 in jfif_encode /opt/disk/marsman/test/ffjpeg/build_asan/src/jfif.c:763:24
    #1 0x515e57 in main /opt/disk/marsman/test/ffjpeg/build_asan/src/ffjpeg.c:30:16
    #2 0x7fbeea24c83f in __libc_start_main /build/glibc-S7Ft5T/glibc-2.23/csu/../csu/libc-start.c:291
    #3 0x419f68 in _start (/opt/disk/marsman/test/ffjpeg/bin_asan/bin/ffjpeg+0x419f68)

AddressSanitizer can not provide additional info.
SUMMARY: AddressSanitizer: SEGV /opt/disk/marsman/test/ffjpeg/build_asan/src/jfif.c:763:24 in jfif_encode
==2671==ABORTING
```

### gdb report
```
Breakpoint 1, bmp_load (pb=0x7fffffffdae0, file=0x7fffffffe070 "./poc-ffjpeg-d5cfd49-jfif_encode-SEGV") at bmp.c:45
45              printf("bmp's width * height is out of range !\n");
(gdb) n
bmp's width * height is out of range !
46              goto done;
(gdb) c
Continuing.

Breakpoint 2, jfif_encode (pb=0x7fffffffdae0) at jfif.c:757
757         bsrc = pb->pdata;
(gdb) n
758         ydst = yuv_datbuf[0];
(gdb) p bsrc
$2 = (BYTE *) 0x0
(gdb) c
Continuing.

Program received signal SIGSEGV, Segmentation fault.
0x0000000000520b78 in jfif_encode (pb=0x7fffffffdae0) at jfif.c:763
763                 rgb_to_yuv(bsrc[2], bsrc[1], bsrc[0], ydst, udst, vdst);
(gdb) bt
#0  0x0000000000520b78 in jfif_encode (pb=0x7fffffffdae0) at jfif.c:763
#1  0x0000000000515e58 in main (argc=3, argv=0x7fffffffdcf8) at ffjpeg.c:30
```