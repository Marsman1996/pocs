# poc15-packet2tree-heapoverflow (CVE-2018-20552)
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


# poc16-get_l2len-heapoverflow (CVE-2018-20553)
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

# poc-tcpreplay-0ca82e3-add_tree_ipv4-assertion (CVE-2021-45387)

## Test Environment
Ubuntu 16.04, 64bit  
tcpreplay (master 0ca82e3)

## How to trigger
`$ tcpprep --auto=bridge --pcap=$POC --cachefile=/dev/null`

## Reference
https://github.com/appneta/tcpreplay/issues/687  
https://www.cve.org/CVERecord?id=CVE-2021-45387  

## Details

### GDB report
```
Breakpoint 1, add_tree_ipv4 (ip=327679, data=<optimized out>, len=<optimized out>) at ../../code/src/tree.c:538
538             assert(ip == newnode->u.ip);
(gdb) p ip
$6 = 327679
(gdb) p newnode->u.ip
$7 = 0
(gdb) n
tcpprep: ../../code/src/tree.c:538: add_tree_ipv4: Assertion `ip == newnode->u.ip' failed.

Program received signal SIGABRT, Aborted.
0x00007ffff7801438 in __GI_raise (sig=sig@entry=6) at ../sysdeps/unix/sysv/linux/raise.c:54
54      ../sysdeps/unix/sysv/linux/raise.c: No such file or directory.
(gdb) bt
#0  0x00007ffff7801438 in __GI_raise (sig=sig@entry=6) at ../sysdeps/unix/sysv/linux/raise.c:54
#1  0x00007ffff780303a in __GI_abort () at abort.c:89
#2  0x00007ffff77f9be7 in __assert_fail_base (fmt=<optimized out>, assertion=assertion@entry=0x41a3ba "ip == newnode->u.ip", file=file@entry=0x41a3a4 "../../code/src/tree.c", 
    line=line@entry=538, function=function@entry=0x41a720 <__PRETTY_FUNCTION__.7785> "add_tree_ipv4") at assert.c:92
#3  0x00007ffff77f9c92 in __GI___assert_fail (assertion=assertion@entry=0x41a3ba "ip == newnode->u.ip", file=file@entry=0x41a3a4 "../../code/src/tree.c", line=line@entry=538, 
    function=function@entry=0x41a720 <__PRETTY_FUNCTION__.7785> "add_tree_ipv4") at assert.c:101
#4  0x00000000004052db in add_tree_ipv4 (ip=327679, data=<optimized out>, len=<optimized out>) at ../../code/src/tree.c:538
#5  0x0000000000402f64 in process_raw_packets (pcap=0x648c10) at ../../code/src/tcpprep.c:463
#6  main (argc=<optimized out>, argv=<optimized out>) at ../../code/src/tcpprep.c:144
```

# poc-tcpreplay-0ca82e3-add_tree_ipv6-assertion (CVE-2021-45386)

## Test Environment
Ubuntu 16.04, 64bit  
tcpreplay (master 0ca82e3)

## How to trigger
`$ tcpprep --auto=bridge --pcap=$POC --cachefile=/dev/null`

## Reference
https://github.com/appneta/tcpreplay/issues/687  
https://www.cve.org/CVERecord?id=CVE-2021-45386  

## Details
### GDB report
```
tcpprep: ../../code/src/tree.c:561: add_tree_ipv6: Assertion `ipv6_cmp(addr, &newnode->u.ip6) == 0' failed.

Program received signal SIGABRT, Aborted.
0x00007ffff7801438 in __GI_raise (sig=sig@entry=6) at ../sysdeps/unix/sysv/linux/raise.c:54
54      ../sysdeps/unix/sysv/linux/raise.c: No such file or directory.
(gdb) bt
#0  0x00007ffff7801438 in __GI_raise (sig=sig@entry=6) at ../sysdeps/unix/sysv/linux/raise.c:54
#1  0x00007ffff780303a in __GI_abort () at abort.c:89
#2  0x00007ffff77f9be7 in __assert_fail_base (fmt=<optimized out>, assertion=assertion@entry=0x41a630 "ipv6_cmp(addr, &newnode->u.ip6) == 0", file=file@entry=0x41a3a4 "../../code/src/tree.c", 
    line=line@entry=561, function=function@entry=0x41a6f0 <__PRETTY_FUNCTION__.7792> "add_tree_ipv6") at assert.c:92
#3  0x00007ffff77f9c92 in __GI___assert_fail (assertion=assertion@entry=0x41a630 "ipv6_cmp(addr, &newnode->u.ip6) == 0", file=file@entry=0x41a3a4 "../../code/src/tree.c", line=line@entry=561, 
    function=function@entry=0x41a6f0 <__PRETTY_FUNCTION__.7792> "add_tree_ipv6") at assert.c:101
#4  0x0000000000405359 in add_tree_ipv6 (addr=0x7ffff7f81018, data=<optimized out>, len=<optimized out>) at ../../code/src/tree.c:561
#5  0x00000000004031fb in process_raw_packets (pcap=0x648c10) at ../../code/src/tcpprep.c:465
#6  main (argc=<optimized out>, argv=<optimized out>) at ../../code/src/tcpprep.c:144
```

# poc-tcpreplay-09f0774-packet2tree-assertion
The assertion `assert(l2len > 0);` in packet2tree() at tree.c is reachable when the user uses tcpprep to open a crafted pcap file.
The variable `l2len` is assigned in get_l2len_protocol() at get.c. 

https://github.com/appneta/tcpreplay/blob/09f07748dcabe3d58961f123f31dd0f75198a389/src/tree.c#L733-L746

However, when the `datalink` is `DLT_RAW` or `DLT_JUNIPER_ETHER`, `l2len` is not assigned, and the assertion is triggered.

https://github.com/appneta/tcpreplay/blob/09f07748dcabe3d58961f123f31dd0f75198a389/src/common/get.c#L268-L282

## Test Environment
Ubuntu 16.04, 64bit  
tcpreplay (master 09f0774)

## How to trigger
1. Get the Tcpreplay source code (master 09f0774) and compile it.
2. Run command: `$ tcpprep --auto=bridge --pcap=$POC --cachefile=/dev/null`  
   The POC file could be downloaded here:  
   [POC_file]()

## Reference


## Details
### GDB report
```
Breakpoint 6, packet2tree (data=0x7ffff7ef8010 "@", len=33, datalink=12) at ../../code/src/tree.c:733
733         res = get_l2len_protocol(data,
(gdb) p datalink 
$8 = 12
(gdb) n
741         if (res == -1)
(gdb) 
744         node = new_tree();
(gdb) 

Breakpoint 1, packet2tree (data=0x7ffff7ef8010 "@", len=33, datalink=<optimized out>) at ../../code/src/tree.c:746
746         assert(l2len > 0);
(gdb) p l2len 
$9 = 0
(gdb) c
Continuing.
tcpprep: ../../code/src/tree.c:746: tcpr_tree_t *packet2tree(const u_char *, const int, int): Assertion `l2len > 0' failed.

Program received signal SIGABRT, Aborted.
0x00007ffff7194438 in __GI_raise (sig=sig@entry=6) at ../sysdeps/unix/sysv/linux/raise.c:54
54      ../sysdeps/unix/sysv/linux/raise.c: No such file or directory.
```