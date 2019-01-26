# poc15-packet2tree-heapoverflow
## Test Environment
Ubuntu 18.04, 64bit  
tcpreplay (master 2d87447)

```
tcpprep version: 4.3.0 (build git:v4.3.0-1-g2d874470)
Copyright 2013-2018 by Fred Klassen <tcpreplay at appneta dot com> - AppNeta
Copyright 2000-2012 by Aaron Turner <aturner at synfin dot net>
The entire Tcpreplay Suite is licensed under the GPLv3
Cache file supported: 04
Not compiled with libdnet.
Compiled against libpcap: 1.8.1
64 bit packet counters: enabled
Verbose printing via tcpdump: enabled
```

## How to trigger
./tcpprep --auto=bridge --pcap=$POC --cachefile=/dev/null

## Reference
https://github.com/appneta/tcpreplay/issues/530  
https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2018-20552

## Credits
Yanhao(unfuzzable123@gmail.com)  
Marsman1996(lqliuyuwei@outlook.com)

## Details
### Asan report

```
ubuntu@ubuntu-virtual-machine:~/Desktop/crashana$ ./tcpreplay/tcpreplay-2d87447/bin_asan/bin/tcpprep --auto=bridge --pcap=./tcpreplay/poc15-packet2tree-heapoverflow --cachefile=/dev/null
Warning: ./tcpreplay/poc15-packet2tree-heapoverflow was captured using a snaplen of 50 bytes.  This may mean you have truncated packets.
=================================================================
==46592==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x60600000005a at pc 0x5625e6ae8e31 bp 0x7ffc780aa970 sp 0x7ffc780aa960
READ of size 4 at 0x60600000005a thread T0
    #0 0x5625e6ae8e30 in packet2tree ../../src/tree.c:749
    #1 0x5625e6aebfe2 in add_tree_ipv4 ../../src/tree.c:536
    #2 0x5625e6ae6fef in process_raw_packets ../../src/tcpprep.c:463
    #3 0x5625e6ae4bed in main ../../src/tcpprep.c:146
    #4 0x7fc706f92b96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)
    #5 0x5625e6ae54e9 in _start (/home/ubuntu/Desktop/crashana/tcpreplay/tcpreplay-2d87447/bin_asan/bin/tcpprep+0x104e9)

0x60600000005a is located 8 bytes to the right of 50-byte region [0x606000000020,0x606000000052)
allocated by thread T0 here:
    #0 0x7fc707681b50 in __interceptor_malloc (/usr/lib/x86_64-linux-gnu/libasan.so.4+0xdeb50)
    #1 0x7fc70738189f  (/usr/lib/x86_64-linux-gnu/libpcap.so.0.8+0x1f89f)

SUMMARY: AddressSanitizer: heap-buffer-overflow ../../src/tree.c:749 in packet2tree
Shadow bytes around the buggy address:
  0x0c0c7fff7fb0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c0c7fff7fc0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c0c7fff7fd0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c0c7fff7fe0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c0c7fff7ff0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x0c0c7fff8000: fa fa fa fa 00 00 00 00 00 00 02[fa]fa fa fa fa
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
==46592==ABORTING
```


# poc16-get_l2len-heapoverflow
## Test Environment
Ubuntu 18.04, 64bit  
tcpreplay (master 2d87447)

## How to trigger
./tcpprep --auto=bridge --pcap=$POC --cachefile=/dev/null

## Reference
https://github.com/appneta/tcpreplay/issues/530  
https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2018-20553

## Credits
Yanhao(unfuzzable123@gmail.com)  
Marsman1996(lqliuyuwei@outlook.com)

## Details
### Asan report

```
ubuntu@ubuntu-virtual-machine:~/Desktop/crashana$ ./tcpreplay/tcpreplay-2d87447/bin_asan/bin/tcpprep --auto=bridge --pcap=./tcpreplay/poc16-get_l2len-heapoverflow --cachefile=/dev/null
Warning: ./tcpreplay/poc16-get_l2len-heapoverflow was captured using a snaplen of 17 bytes.  This may mean you have truncated packets.
=================================================================
==54318==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x603000000020 at pc 0x5637ffc1f43a bp 0x7fffbeb468b0 sp 0x7fffbeb468a0
READ of size 2 at 0x603000000020 thread T0
    #0 0x5637ffc1f439 in get_l2len ../../../src/common/get.c:183
    #1 0x5637ffc1f492 in get_ipv4 ../../../src/common/get.c:247
    #2 0x5637ffc12c79 in process_raw_packets ../../src/tcpprep.c:367
    #3 0x5637ffc10bed in main ../../src/tcpprep.c:146
    #4 0x7f99179dfb96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)
    #5 0x5637ffc114e9 in _start (/home/ubuntu/Desktop/crashana/tcpreplay/tcpreplay-2d87447/bin_asan/bin/tcpprep+0x104e9)

0x603000000021 is located 0 bytes to the right of 17-byte region [0x603000000010,0x603000000021)
allocated by thread T0 here:
    #0 0x7f99180ceb50 in __interceptor_malloc (/usr/lib/x86_64-linux-gnu/libasan.so.4+0xdeb50)
    #1 0x7f9917dce89f  (/usr/lib/x86_64-linux-gnu/libpcap.so.0.8+0x1f89f)

SUMMARY: AddressSanitizer: heap-buffer-overflow ../../../src/common/get.c:183 in get_l2len
Shadow bytes around the buggy address:
  0x0c067fff7fb0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c067fff7fc0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c067fff7fd0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c067fff7fe0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c067fff7ff0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x0c067fff8000: fa fa 00 00[01]fa fa fa fa fa fa fa fa fa fa fa
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
==54318==ABORTING
```