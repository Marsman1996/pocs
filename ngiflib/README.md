# poc10-GifIndexToTrueColor-HeapOverFlow
*Developer of ngiflib said that poc11 is same as poc10.*

## Test Environment
Ubuntu 18.04, 64bit  
ngiflib(master c8488d5)

## How to trigger
`$ gif2tga $POC`

## Reference
https://github.com/miniupnp/ngiflib/issues/9

## Credits
Yanhao(unfuzzable123@gmail.com)  
Marsman1996(lqliuyuwei@outlook.com)

## Details
### Asan report

```
=================================================================
==1786==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x603000000036 at pc 0x5578a31e5b4c bp 0x7ffce98362c0 sp 0x7ffce98362b0
READ of size 1 at 0x603000000036 thread T0
    #0 0x5578a31e5b4b in GifIndexToTrueColor /home/ubuntu/Desktop/crashana/ngiflib/ngiflib-c8488d5/ngiflib.c:821
    #1 0x5578a31e7c4f in WritePixel /home/ubuntu/Desktop/crashana/ngiflib/ngiflib-c8488d5/ngiflib.c:124
    #2 0x5578a31e7c4f in DecodeGifImg /home/ubuntu/Desktop/crashana/ngiflib/ngiflib-c8488d5/ngiflib.c:537
    #3 0x5578a31e9d51 in LoadGif /home/ubuntu/Desktop/crashana/ngiflib/ngiflib-c8488d5/ngiflib.c:802
    #4 0x5578a31e4d84 in main /home/ubuntu/Desktop/crashana/ngiflib/ngiflib-c8488d5/gif2tga.c:95
    #5 0x7f8bea6a7b96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)
    #6 0x5578a31e4129 in _start (/home/ubuntu/Desktop/crashana/ngiflib/ngiflib-c8488d5/gif2tga+0x2129)

0x603000000036 is located 14 bytes to the right of 24-byte region [0x603000000010,0x603000000028)
allocated by thread T0 here:
    #0 0x7f8beab55b50 in __interceptor_malloc (/usr/lib/x86_64-linux-gnu/libasan.so.4+0xdeb50)
    #1 0x5578a31e8ced in LoadGif /home/ubuntu/Desktop/crashana/ngiflib/ngiflib-c8488d5/ngiflib.c:645

SUMMARY: AddressSanitizer: heap-buffer-overflow /home/ubuntu/Desktop/crashana/ngiflib/ngiflib-c8488d5/ngiflib.c:821 in GifIndexToTrueColor
Shadow bytes around the buggy address:
  0x0c067fff7fb0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c067fff7fc0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c067fff7fd0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c067fff7fe0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c067fff7ff0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x0c067fff8000: fa fa 00 00 00 fa[fa]fa fa fa fa fa fa fa fa fa
  0x0c067fff8010: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c067fff8020: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c067fff8030: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c067fff8040: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c067fff8050: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
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
==1786==ABORTING
```

# poc11-GifIndexToTrueColor-SEGV
## Test Environment
Ubuntu 18.04, 64bit  
ngiflib(master c8488d5)

## How to trigger
`$ gif2tga $POC`

## Reference
https://github.com/miniupnp/ngiflib/issues/9

## Credits
Yanhao(unfuzzable123@gmail.com)  
Marsman1996(lqliuyuwei@outlook.com)

## Details
### Asan report

```
==2123==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000005 (pc 0x55e059633aec bp 0x607000000090 sp 0x7ffef8b0a370 T0)
==2123==The signal is caused by a READ memory access.
==2123==Hint: address points to the zero page.
    #0 0x55e059633aeb in GifIndexToTrueColor /home/ubuntu/Desktop/crashana/ngiflib/ngiflib-c8488d5/ngiflib.c:821
    #1 0x55e059635c4f in WritePixel /home/ubuntu/Desktop/crashana/ngiflib/ngiflib-c8488d5/ngiflib.c:124
    #2 0x55e059635c4f in DecodeGifImg /home/ubuntu/Desktop/crashana/ngiflib/ngiflib-c8488d5/ngiflib.c:537
    #3 0x55e059637d51 in LoadGif /home/ubuntu/Desktop/crashana/ngiflib/ngiflib-c8488d5/ngiflib.c:802
    #4 0x55e059632d84 in main /home/ubuntu/Desktop/crashana/ngiflib/ngiflib-c8488d5/gif2tga.c:95
    #5 0x7fe721399b96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)
    #6 0x55e059632129 in _start (/home/ubuntu/Desktop/crashana/ngiflib/ngiflib-c8488d5/gif2tga+0x2129)

AddressSanitizer can not provide additional info.
SUMMARY: AddressSanitizer: SEGV /home/ubuntu/Desktop/crashana/ngiflib/ngiflib-c8488d5/ngiflib.c:821 in GifIndexToTrueColor
==2123==ABORTING
```

### GDB report

```
Program received signal SIGSEGV, Segmentation fault.
0x0000555555555665 in GifIndexToTrueColor (palette=<optimized out>, v=v@entry=1 '\001') at ngiflib.c:821
821		return palette[v].b | (palette[v].g << 8) | (palette[v].r << 16);
(gdb) bt
#0  0x0000555555555665 in GifIndexToTrueColor (palette=<optimized out>, v=v@entry=1 '\001') at ngiflib.c:821
#1  0x0000555555555ee7 in WritePixel (v=1 '\001', context=0x7fffffff95a0, i=0x55555575b510) at ngiflib.c:124
#2  DecodeGifImg (i=0x55555575b510) at ngiflib.c:537
#3  0x000055555555695e in LoadGif (g=0x555555759260) at ngiflib.c:802
#4  0x0000555555555258 in main (argc=<optimized out>, argv=<optimized out>) at gif2tga.c:95
```

# poc12-DecodeGifImg-stackoverflow
## Test Environment
Ubuntu 16.04, 64bit  
ngiflib(master c8488d5)

## How to trigger
`$ gif2tga $POC`

## Reference
https://github.com/miniupnp/ngiflib/issues/10

## Credits
Yanhao(unfuzzable123@gmail.com)  
Marsman1996(lqliuyuwei@outlook.com)

## Details
### Asan report

```
==122055==ERROR: AddressSanitizer: stack-buffer-overflow on address 0x7ffddb48cc0f at pc 0x56351fd192cd bp 0x7ffddb4899e0 sp 0x7ffddb4899d0
WRITE of size 1 at 0x7ffddb48cc0f thread T0
    #0 0x56351fd192cc in DecodeGifImg /home/ubuntu/Desktop/crashana/ngiflib/ngiflib-c8488d5/ngiflib.c:554
    #1 0x56351fd1ad51 in LoadGif /home/ubuntu/Desktop/crashana/ngiflib/ngiflib-c8488d5/ngiflib.c:802
    #2 0x56351fd15d84 in main /home/ubuntu/Desktop/crashana/ngiflib/ngiflib-c8488d5/gif2tga.c:95
    #3 0x7f14eb381b96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)
    #4 0x56351fd15129 in _start (/home/ubuntu/Desktop/crashana/ngiflib/ngiflib-c8488d5/gif2tga+0x2129)

Address 0x7ffddb48cc0f is located in stack of thread T0 at offset 12703 in frame
    #0 0x56351fd1756d in DecodeGifImg /home/ubuntu/Desktop/crashana/ngiflib/ngiflib-c8488d5/ngiflib.c:387

  This frame has 4 object(s):
    [32, 320) 'context'
    [352, 8544) 'ab_prfx'
    [8576, 12672) 'ab_suffx'
    [12704, 16800) 'ab_stack' <== Memory access at offset 12703 underflows this variable
HINT: this may be a false positive if your program uses some custom stack unwind mechanism or swapcontext
      (longjmp and C++ exceptions *are* supported)
SUMMARY: AddressSanitizer: stack-buffer-overflow /home/ubuntu/Desktop/crashana/ngiflib/ngiflib-c8488d5/ngiflib.c:554 in DecodeGifImg
Shadow bytes around the buggy address:
  0x10003b689930: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x10003b689940: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x10003b689950: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x10003b689960: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x10003b689970: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 f2 f2
=>0x10003b689980: f2[f2]00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x10003b689990: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x10003b6899a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x10003b6899b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x10003b6899c0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x10003b6899d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
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
==122055==ABORTING
```

# poc13-WritePixel-heapoverflow (CVE-2019-16346)
## Test Environment
Ubuntu 16.04, 64bit  
ngiflib(master 3bb9980)

## How to trigger
`$ gif2tga $POC`

## Reference
https://github.com/miniupnp/ngiflib/issues/11  
https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-16346

## Credits
Yanhao(unfuzzable123@gmail.com)  
Marsman1996(lqliuyuwei@outlook.com)

## Details
### Asan report

```
==122089==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x621000002500 at pc 0x55dc592dfd89 bp 0x7ffc11226c20 sp 0x7ffc11226c10
WRITE of size 4 at 0x621000002500 thread T0
    #0 0x55dc592dfd88 in WritePixel /home/ubuntu/Desktop/crashana/ngiflib/ngiflib/ngiflib.c:123
    #1 0x55dc592dfd88 in DecodeGifImg /home/ubuntu/Desktop/crashana/ngiflib/ngiflib/ngiflib.c:544
    #2 0x55dc592e1e3b in LoadGif /home/ubuntu/Desktop/crashana/ngiflib/ngiflib/ngiflib.c:809
    #3 0x55dc592dcdc4 in main /home/ubuntu/Desktop/crashana/ngiflib/ngiflib/gif2tga.c:95
    #4 0x7f29ccd05b96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)
    #5 0x55dc592dc169 in _start (/home/ubuntu/Desktop/crashana/ngiflib/ngiflib/gif2tga+0x2169)

0x621000002500 is located 0 bytes to the right of 4096-byte region [0x621000001500,0x621000002500)
allocated by thread T0 here:
    #0 0x7f29cd1b3b50 in __interceptor_malloc (/usr/lib/x86_64-linux-gnu/libasan.so.4+0xdeb50)
    #1 0x55dc592e0a38 in LoadGif /home/ubuntu/Desktop/crashana/ngiflib/ngiflib/ngiflib.c:627

SUMMARY: AddressSanitizer: heap-buffer-overflow /home/ubuntu/Desktop/crashana/ngiflib/ngiflib/ngiflib.c:123 in WritePixel
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
==122089==ABORTING
```

# poc14-WritePixels-heapoverflow (CVE-2019-16347)
## Test Environment
Ubuntu 16.04, 64bit  
ngiflib(master 3bb9980)

## How to trigger
`$ gif2tga $POC`

## Reference
https://github.com/miniupnp/ngiflib/issues/12  
https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-16347

## Credits
Yanhao(unfuzzable123@gmail.com)  
Marsman1996(lqliuyuwei@outlook.com)

## Details
### Asan report

```
==122092==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x6150000002a0 at pc 0x55f220dc8d87 bp 0x7ffeb9b2bb80 sp 0x7ffeb9b2bb70
WRITE of size 4 at 0x6150000002a0 thread T0
    #0 0x55f220dc8d86 in WritePixels /home/ubuntu/Desktop/crashana/ngiflib/ngiflib/ngiflib.c:213
    #1 0x55f220dcb5d1 in DecodeGifImg /home/ubuntu/Desktop/crashana/ngiflib/ngiflib/ngiflib.c:568
    #2 0x55f220dcce3b in LoadGif /home/ubuntu/Desktop/crashana/ngiflib/ngiflib/ngiflib.c:809
    #3 0x55f220dc7dc4 in main /home/ubuntu/Desktop/crashana/ngiflib/ngiflib/gif2tga.c:95
    #4 0x7f1dcc2b9b96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)
    #5 0x55f220dc7169 in _start (/home/ubuntu/Desktop/crashana/ngiflib/ngiflib/gif2tga+0x2169)

Address 0x6150000002a0 is a wild pointer.
SUMMARY: AddressSanitizer: heap-buffer-overflow /home/ubuntu/Desktop/crashana/ngiflib/ngiflib/ngiflib.c:213 in WritePixels
Shadow bytes around the buggy address:
  0x0c2a7fff8000: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c2a7fff8010: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c2a7fff8020: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c2a7fff8030: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c2a7fff8040: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x0c2a7fff8050: fa fa fa fa[fa]fa fa fa fa fa fa fa fa fa fa fa
  0x0c2a7fff8060: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c2a7fff8070: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c2a7fff8080: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c2a7fff8090: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c2a7fff80a0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
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
==122092==ABORTING
```

# poc-ngiflib-0245fd4-GetByte-overflow
This Overflow problem is because in NGIFLIB_NO_FILE mode, `GetByte()` reads memory buffer without checking the boundary.
## Test Environment
Ubuntu 16.04, 64bit  
ngiflib(master 0245fd4)

## How to trigger
1. Compile the program with AddressSanitizer in NGIFLIB_NO_FILE mode `CC="clang -fsanitize=address -g" CFLAGS+=-DNGIFLIB_NO_FILE make`
2. run the compiled program `$ ./gif2tga --outbase /dev/null $POC`

## Reference
https://github.com/miniupnp/ngiflib/issues/18  
https://cve.mitre.org/cgi-bin/cvename.cgi?name=2021-36531
## Credits
Yanhao(unfuzzable123@gmail.com)  
Marsman1996(lqliuyuwei@outlook.com)

## Details

### ASAN report
```
==8923==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x606000000058 at pc 0x00000051a564 bp 0x7fff7f5aeba0 sp 0x7fff7f5aeb98
READ of size 1 at 0x606000000058 thread T0
    #0 0x51a563 in GetByte /opt/disk/marsman/test/ngiflib/build_asan/ngiflib.c:70:10
    #1 0x51902b in LoadGif /opt/disk/marsman/test/ngiflib/build_asan/ngiflib.c:680:23
    #2 0x51696e in main /opt/disk/marsman/test/ngiflib/build_asan/gif2tga.c:95:10
    #3 0x7fa15acde83f in __libc_start_main /build/glibc-S7Ft5T/glibc-2.23/csu/../csu/libc-start.c:291
    #4 0x419fa8 in _start (/opt/disk/marsman/test/ngiflib/build_asan/gif2tga+0x419fa8)

0x606000000058 is located 0 bytes to the right of 56-byte region [0x606000000020,0x606000000058)
allocated by thread T0 here:
    #0 0x4de1d8 in __interceptor_malloc /home/mcgrady/wyh/llvm/llvm-6.0.0.src/projects/compiler-rt/lib/asan/asan_malloc_linux.cc:88
    #1 0x5166c0 in main /opt/disk/marsman/test/ngiflib/build_asan/gif2tga.c:75:11
    #2 0x7fa15acde83f in __libc_start_main /build/glibc-S7Ft5T/glibc-2.23/csu/../csu/libc-start.c:291

SUMMARY: AddressSanitizer: heap-buffer-overflow /opt/disk/marsman/test/ngiflib/build_asan/ngiflib.c:70:10 in GetByte
Shadow bytes around the buggy address:
  0x0c0c7fff7fb0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c0c7fff7fc0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c0c7fff7fd0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c0c7fff7fe0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c0c7fff7ff0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x0c0c7fff8000: fa fa fa fa 00 00 00 00 00 00 00[fa]fa fa fa fa
  0x0c0c7fff8010: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c0c7fff8020: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c0c7fff8030: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c0c7fff8040: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c0c7fff8050: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
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
==8923==ABORTING
```

# poc-ngiflib-0245fd4-GetByteStr-overflow
Similar to #xx, this Overflow problem is because in NGIFLIB_NO_FILE mode, `GetByteStr()` copy memory buffer without checking the boundary.

## Test Environment
Ubuntu 16.04, 64bit  
ngiflib(master 0245fd4)

## How to trigger
1. Compile the program with AddressSanitizer in NGIFLIB_NO_FILE mode `CC="clang -fsanitize=address -g" CFLAGS+=-DNGIFLIB_NO_FILE make`
2. run the compiled program `$ ./gif2tga --outbase /dev/null $POC`

## Reference
https://github.com/miniupnp/ngiflib/issues/19  
https://cve.mitre.org/cgi-bin/cvename.cgi?name=2021-36530
## Credits
Yanhao(unfuzzable123@gmail.com)  
Marsman1996(lqliuyuwei@outlook.com)

## Details

### ASAN report
```
==19652==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x606000000118 at pc 0x0000004dd09d bp 0x7ffd61fa7590 sp 0x7ffd61fa6d40
READ of size 132 at 0x606000000118 thread T0
    #0 0x4dd09c in __asan_memcpy /home/mcgrady/wyh/llvm/llvm-6.0.0.src/projects/compiler-rt/lib/asan/asan_interceptors_memintrinsics.cc:23
    #1 0x51771a in GetByteStr /opt/disk/marsman/test/ngiflib/build_asan/ngiflib.c:108:3
    #2 0x51771a in LoadGif /opt/disk/marsman/test/ngiflib/build_asan/ngiflib.c:716
    #3 0x5161b5 in main /opt/disk/marsman/test/ngiflib/build_asan/gif2tga.c:95:10
    #4 0x7f3de58ea83f in __libc_start_main /build/glibc-S7Ft5T/glibc-2.23/csu/../csu/libc-start.c:291
    #5 0x419fe8 in _start (/opt/disk/marsman/test/ngiflib/bin_asan/bin/gif2tga+0x419fe8)

0x606000000118 is located 0 bytes to the right of 56-byte region [0x6060000000e0,0x606000000118)
allocated by thread T0 here:
    #0 0x4de218 in __interceptor_malloc /home/mcgrady/wyh/llvm/llvm-6.0.0.src/projects/compiler-rt/lib/asan/asan_malloc_linux.cc:88
    #1 0x516021 in main /opt/disk/marsman/test/ngiflib/build_asan/gif2tga.c:75:11
    #2 0x7f3de58ea83f in __libc_start_main /build/glibc-S7Ft5T/glibc-2.23/csu/../csu/libc-start.c:291

SUMMARY: AddressSanitizer: heap-buffer-overflow /home/mcgrady/wyh/llvm/llvm-6.0.0.src/projects/compiler-rt/lib/asan/asan_interceptors_memintrinsics.cc:23 in __asan_memcpy
Shadow bytes around the buggy address:
  0x0c0c7fff7fd0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c0c7fff7fe0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c0c7fff7ff0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c0c7fff8000: fa fa fa fa fd fd fd fd fd fd fd fa fa fa fa fa
  0x0c0c7fff8010: 00 00 00 00 00 00 00 01 fa fa fa fa 00 00 00 00
=>0x0c0c7fff8020: 00 00 00[fa]fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c0c7fff8030: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c0c7fff8040: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c0c7fff8050: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c0c7fff8060: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c0c7fff8070: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
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
==19652==ABORTING
```

# driver-SDL_LoadGIF-4557142-segv
SEGV in API `SDL_LoadGIF` caused by failure of `SDL_CreateRGBSurface`

## Summary
When calling `SDL_LoadGIF` with specific gif file, `SDL_CreateRGBSurface` in ngiflibSDL.c:77 fails and returns a NULL pointer, and triggers SEGV in ngiflibSDL.c:87:

https://github.com/miniupnp/ngiflib/blob/4557142d84bc0dedbf7ba1c2956f13f7321c89f2/ngiflibSDL.c#L77-L87

## Test Environment
Ubuntu 24.04.1, 64bit  
ngiflib(master 4557142)

## How to trigger
1. Compile the program with AddressSanitizer `CC="clang -fsanitize=address,fuzzer-no-link -g " CFLAGS="-g -O0" make`
2. Compile the fuzz driver which calls API `SDL_LoadGIF`:
    ```C
    // This fuzz driver is generated for library ngiflib, aiming to fuzz the
    // following functions: CheckGif at ngiflib.c:408:5 in ngiflib.h LoadGif at
    // ngiflib.c:647:5 in ngiflib.h GifIndexToTrueColor at ngiflib.c:901:5 in
    // ngiflib.h SDL_LoadAnimatedGif at ngiflibSDL.c:103:31 in ngiflibSDL.h
    // SDL_LoadGIF at ngiflibSDL.c:9:15 in ngiflibSDL.h
    #include "ngiflib.h"
    #include "ngiflibSDL.h"
    #include <stddef.h>
    #include <stdint.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
      if (Size == 0)
        return 0;

      // Test GifIndexToTrueColor
      struct ngiflib_rgb palette[256];
      for (int i = 0; i < 256; i++) {
        palette[i].r = Data[i % Size];
        palette[i].g = Data[(i + 1) % Size];
        palette[i].b = Data[(i + 2) % Size];
      }
      u8 index = Data[0];
      GifIndexToTrueColor(palette, index);

      // Test CheckGif
      u8 gif_header[6];
      for (int i = 0; i < 6 && i < Size; i++) {
        gif_header[i] = Data[i];
      }
      CheckGif(gif_header);

      if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
      }

      // Test SDL_LoadGIF
      FILE *file = fopen("./dummy_file", "wb");
      if (file) {
        fwrite(Data, 1, Size, file);
        fclose(file);
        SDL_Surface *surface = SDL_LoadGIF("./dummy_file");
        if (surface) {
          SDL_FreeSurface(surface);
        }
      }

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
    }
    ```

    ```bash
    $ clang ./driver-SDL_LoadGIF-4557142-segv.c -fsanitize=address,undefined -I./ ./ngiflibSDL.o ./ngiflib.o -lSDL -I/usr/include/SDL  -o driver-SDL_LoadGIF-4557142-segv -g
    ```

3. Download the [poc file](https://github.com/Marsman1996/pocs/blob/master/ngiflib/poc-SDL_LoadGIF-4557142-segv?raw=true) and run the fuzz driver program

## Reference
https://github.com/miniupnp/ngiflib/issues/  

## Credits
Marsman1996(lqliuyuwei@outlook.com)

## Details

### ASAN report
```
$ ./driver-SDL_LoadGIF-4557142-segv poc-SDL_LoadGIF-4557142-segv
GIF87a
64160x144 8bits 256 couleurs  bg=0
BLOCK SIGNATURE 0x2C ','
img pos(0,0) size 160x144 palbits=8 imgbits=8 ncolors=256
Code clear (256) (free=258) npix=23040
assez de pixels, On se casse !
ZERO TERMINATOR 0xFF
AddressSanitizer:DEADLYSIGNAL
=================================================================
==485850==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008 (pc 0x5989b3da336f bp 0x7ffc79841830 sp 0x7ffc798415c0 T0)
==485850==The signal is caused by a READ memory access.
==485850==Hint: address points to the zero page.
    #0 0x5989b3da336f in SDL_LoadGIF /home/yuwei/afgen/afgenllm/database/ngiflib/ngiflib/ngiflibSDL.c:87:12
    #1 0x5989b3da2714 in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/ngiflib/ngiflib/./driver-SDL_LoadGIF-4557142-segv.c:45:28
    #2 0x5989b3da2d05 in main /home/yuwei/afgen/afgenllm/database/ngiflib/ngiflib/./driver-SDL_LoadGIF-4557142-segv.c:71:3
    #3 0x7c9f1422a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #4 0x7c9f1422a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #5 0x5989b3cc8424 in _start (/home/yuwei/afgen/afgenllm/database/ngiflib/ngiflib/driver-SDL_LoadGIF-4557142-segv+0x2f424) (BuildId: 000a0124e10c7081ddbdb5e6357a5882555cea1e)

AddressSanitizer can not provide additional info.
SUMMARY: AddressSanitizer: SEGV /home/yuwei/afgen/afgenllm/database/ngiflib/ngiflib/ngiflibSDL.c:87:12 in SDL_LoadGIF
==485850==ABORTING
```