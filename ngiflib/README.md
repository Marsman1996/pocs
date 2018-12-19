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