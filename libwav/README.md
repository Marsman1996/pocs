# POC18-gain_file-SEGV (CVE-2019-16348)
a NULL Pointer Dereference bug
## Test Environment
Ubuntu 14.04, 64bit
libwav (master 5cc8746)

## How to trigger
`$ ./wav_gain $POC /dev/null`

## Reference
https://github.com/marc-q/libwav/issues/24  
https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-16348

## Credits
Yanhao(unfuzzable123@gmail.com)  
Marsman1996(lqliuyuwei@outlook.com)  
Yuanpingyu(cfenicey@gmail.com)

## Details
### Asan report
```
ASAN:SIGSEGV
=================================================================
==21704== ERROR: AddressSanitizer: SEGV on unknown address 0x000000000000 (pc 0x7faf08a8ce02 sp 0x7ffc93c70b50 bp 0xac4400020001 T0)
AddressSanitizer can not provide additional info.
    #0 0x7faf08a8ce01 (/usr/lib/x86_64-linux-gnu/libasan.so.0+0x7e01)
    #1 0x7faf08a9a367 (/usr/lib/x86_64-linux-gnu/libasan.so.0+0x15367)
    #2 0x400d80 in gain_file /home/aota10/MARS_fuzzcompare/test/wav_gain/build_asan/wav_gain.c:33
    #3 0x400d80 in main /home/aota10/MARS_fuzzcompare/test/wav_gain/build_asan/wav_gain.c:43
    #4 0x7faf086ddf44 (/lib/x86_64-linux-gnu/libc.so.6+0x21f44)
    #5 0x400e24 in _start (/home/aota10/MARS_fuzzcompare/test/wav_gain/bin_asan/bin/wav_gain+0x400e24)
SUMMARY: AddressSanitizer: SEGV ??:0 ??
==21704== ABORTING
```

### GDB report
```
Program received signal SIGSEGV, Segmentation fault.
__GI___libc_free (mem=0x24564157) at malloc.c:2931
2931    malloc.c: No such file or directory.
(gdb) bt
#0  __GI___libc_free (mem=0x24564157) at malloc.c:2931
#1  0x00000000004007e2 in main ()
```

# poc23-libwav-5cc8746-wav_chunk_read-leak
There is a memory leak error when the wav_gain program tries to read the poc file but failed. 
It allocated memory in wav_chunk_read() libwav.c:165, but due to the read failure it directly returns in gain_file() wav_gain.c:23 and misses the memory free process in wav_gain():33.

## Test Environment
Ubuntu 18.04, 64bit
libwav (master 5cc8746)

## How to trigger
1. compile the program with AddressSanitizer
2. run the compiled program `$ ./wav_gain $POC /dev/null`

## Reference

## Credits
Yanhao(unfuzzable123@gmail.com)  
Marsman1996(lqliuyuwei@outlook.com)  
Yuanpingyu(cfenicey@gmail.com)

## Details
### Asan report
```
==12327==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 218129505 byte(s) in 1 object(s) allocated from:
    #0 0x4d9ca0 in malloc /mnt/d/ubuntu/llvm/llvm-6.0.0.src/projects/compiler-rt/lib/asan/asan_malloc_linux.cc:88
    #1 0x512769 in wav_chunk_read /mnt/d/ubuntu/dataset/libwav/libwav/tools/wav_gain/../../libwav.c:165:26

SUMMARY: AddressSanitizer: 218129505 byte(s) leaked in 1 allocation(s).
```
