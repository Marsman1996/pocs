# poc17-xml_on_metadata_tag_only-SEGV
## Test Environment
Ubuntu 18.04, 64bit, gcc 7.3.0  
flvmeta (master 1c9cee8)

## How to trigger
`$ ./flvmeta $POC`

## Reference


## Details
### ASAN report

```
ASAN:DEADLYSIGNAL
=================================================================
==41446==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000000 (pc 0x7fa732fb5e1a bp 0x7ffe6e35d9f0 sp 0x7ffe6e35d150 T0)
==41446==The signal is caused by a READ memory access.
==41446==Hint: address points to the zero page.
    #0 0x7fa732fb5e19  (/usr/lib/x86_64-linux-gnu/libasan.so.4+0x59e19)
    #1 0x55ef074c05ed in xml_on_metadata_tag_only /home/ubuntu/Desktop/crashana/flvmeta/flvmeta-master/src/dump_xml.c:271
    #2 0x55ef074c6f58 in flv_parse /home/ubuntu/Desktop/crashana/flvmeta/flvmeta-master/src/flv.c:506
    #3 0x55ef074bc20c in dump_metadata /home/ubuntu/Desktop/crashana/flvmeta/flvmeta-master/src/dump.c:160
    #4 0x55ef074c908d in main /home/ubuntu/Desktop/crashana/flvmeta/flvmeta-master/src/flvmeta.c:385
    #5 0x7fa732b8cb96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)
    #6 0x55ef074aae19 in _start (/home/ubuntu/Desktop/crashana/flvmeta/flvmeta-master/bin_asan/bin/flvmeta+0x11e19)

AddressSanitizer can not provide additional info.
SUMMARY: AddressSanitizer: SEGV (/usr/lib/x86_64-linux-gnu/libasan.so.4+0x59e19) 
==41446==ABORTING
```

### GDB report

```
Program received signal SIGSEGV, Segmentation fault.
__strcmp_sse2_unaligned () at ../sysdeps/x86_64/multiarch/strcmp-sse2-unaligned.S:31
31	../sysdeps/x86_64/multiarch/strcmp-sse2-unaligned.S: No such file or directory.
(gdb) bt
#0  __strcmp_sse2_unaligned () at ../sysdeps/x86_64/multiarch/strcmp-sse2-unaligned.S:31
#1  0x000055555556363e in xml_on_metadata_tag_only ()
#2  0x0000555555566876 in flv_parse ()
#3  0x0000555555561468 in dump_metadata ()
#4  0x00005555555672f9 in main ()
```