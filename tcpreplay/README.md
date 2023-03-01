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

# poc-tcpreplay-09f0774-packet2tree-assertion (CVE-2022-25484)
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
   [POC_file](https://raw.githubusercontent.com/Marsman1996/pocs/master/tcpreplay/poc-tcpreplay-09f0774-packet2tree-assertion)

## Reference
https://github.com/appneta/tcpreplay/issues/715  
https://www.cve.org/CVERecord?id=CVE-2022-25484  

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

# poc-tcprewrite-bcb107a-tcpedit_dlt_cleanup-assertion
There is a reachable assertion in `tcpedit_dlt_cleanup()` when the user uses `tcprewrite` to open a crafted pcap file in `DLT_JUNIPER_ETHER` mode.

## Test Environment
Ubuntu 20.04, 64bit  
tcpreplay (master bcb107a)

```
./bin_normal/bin/tcprewrite -V
tcprewrite version: 4.4.3 (build git:v4.4.3)
Copyright 2013-2022 by Fred Klassen <tcpreplay at appneta dot com> - AppNeta
Copyright 2000-2012 by Aaron Turner <aturner at synfin dot net>
The entire Tcpreplay Suite is licensed under the GPLv3
Cache file supported: 04
Not compiled with libdnet.
Compiled against libpcap: 1.9.1
64 bit packet counters: enabled
Verbose printing via tcpdump: enabled
Fragroute engine: disabled
```

## How to trigger
1. Get the Tcpreplay source code and compile it.
  ```
  $ ./configure
  $ make
  ```
2. Run Command `$ ./tcprewrite --dlt="jnpr_eth" -i $POC -o /dev/null`  
  The POC file could be downloaded here:
  [POC file](https://raw.githubusercontent.com/Marsman1996/pocs/master/tcpreplay/poc-tcprewrite-bcb107a-tcpedit_dlt_cleanup-assertion)

## GDB report
```
$ gdb --args ./bin_normal/bin/tcprewrite --dlt="jnpr_eth" -i ./poc-tcprewrite-bcb107a-tcpedit_dlt_cleanup-assertion -o /dev/null

(gdb) r
Starting program: /home/ubuntu178/cvelibf/test/tcpreplay/latest/bin_normal/bin/tcprewrite --dlt=jnpr_eth -i ./poc-tcprewrite-bcb107a-tcpedit_dlt_cleanup-assertion -o /dev/null
Warning: ./poc-tcprewrite-bcb107a-tcpedit_dlt_cleanup-assertion was captured using a snaplen of 96 bytes.  This may mean you have truncated packets.
tcprewrite: plugins/dlt_plugins.c:462: tcpedit_dlt_cleanup: Assertion `ctx' failed.

Program received signal SIGABRT, Aborted.
__GI_raise (sig=sig@entry=6) at ../sysdeps/unix/sysv/linux/raise.c:50
50      ../sysdeps/unix/sysv/linux/raise.c: No such file or directory.
(gdb) bt
#0  __GI_raise (sig=sig@entry=6) at ../sysdeps/unix/sysv/linux/raise.c:50
#1  0x00007ffff7d6d859 in __GI_abort () at abort.c:79
#2  0x00007ffff7d6d729 in __assert_fail_base (fmt=0x7ffff7f03588 "%s%s%s:%u: %s%sAssertion `%s' failed.\n%n", assertion=0x555555572ae6 "ctx", file=0x555555572ad0 "plugins/dlt_plugins.c", line=462, 
    function=<optimized out>) at assert.c:92
#3  0x00007ffff7d7ef36 in __GI___assert_fail (assertion=0x555555572ae6 "ctx", file=0x555555572ad0 "plugins/dlt_plugins.c", line=462, function=0x555555572fb0 <__PRETTY_FUNCTION__.7740> "tcpedit_dlt_cleanup")
    at assert.c:101
#4  0x000055555556156d in tcpedit_dlt_cleanup (ctx=0x0) at plugins/dlt_plugins.c:462
#5  0x0000555555569aca in dlt_jnpr_ether_cleanup (ctx=0x555555580090) at plugins/dlt_jnpr_ether/jnpr_ether.c:171
#6  0x000055555556158c in tcpedit_dlt_cleanup (ctx=0x555555580090) at plugins/dlt_plugins.c:466
#7  0x000055555555a763 in tcpedit_close (tcpedit_ex=0x55555557db60 <tcpedit>) at tcpedit.c:599
#8  0x0000555555558f02 in main (argc=0, argv=0x7fffffffde38) at tcprewrite.c:154
```

# cmd-tcpprep-bcb107a-macinstring-SEGV
There is a NULL Pointer Dereference in `macinstring()` when the user passes empty mac string to `tcpprep` in Source MAC split mode.  


## Test Environment
Ubuntu 20.04, 64bit  
tcpreplay (master bcb107a)

```
./bin_normal/bin/tcprewrite -V
tcprewrite version: 4.4.3 (build git:v4.4.3)
Copyright 2013-2022 by Fred Klassen <tcpreplay at appneta dot com> - AppNeta
Copyright 2000-2012 by Aaron Turner <aturner at synfin dot net>
The entire Tcpreplay Suite is licensed under the GPLv3
Cache file supported: 04
Not compiled with libdnet.
Compiled against libpcap: 1.9.1
64 bit packet counters: enabled
Verbose printing via tcpdump: enabled
Fragroute engine: disabled
```

## How to trigger
1. Get the Tcpreplay source code and compile it.
  ```
  $ ./configure
  $ make
  ```
2. Run Command `$ ./tcpprep -e "" --pcap=./test.pcap --cachefile=/dev/null`  
  The file `test.pcap` is from tcpreplay codebase, which is located in `test/test.pcap`.

## Details
This crash is due to the program does not check the given mac string.

### ASAN report
```
$ ./bin_asan/bin/tcpprep -e "" --pcap=./code/test/test.pcap --cachefile=/dev/null
AddressSanitizer:DEADLYSIGNAL
=================================================================
==3017398==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000000 (pc 0x000000516fd4 bp 0x7ffe41169c70 sp 0x7ffe41169b80 T0)
==3017398==The signal is caused by a READ memory access.
==3017398==Hint: address points to the zero page.
    #0 0x516fd4 in macinstring /home/ubuntu178/cvelibf/test/tcpreplay/latest/build_asan/src/common/../../../code/src/common/mac.c:120:9
    #1 0x4fe345 in process_raw_packets /home/ubuntu178/cvelibf/test/tcpreplay/latest/build_asan/src/../../code/src/tcpprep.c:450:25
    #2 0x4fe345 in main /home/ubuntu178/cvelibf/test/tcpreplay/latest/build_asan/src/../../code/src/tcpprep.c:144:23
    #3 0x7fb921f310b2 in __libc_start_main /build/glibc-eX1tMB/glibc-2.31/csu/../csu/libc-start.c:308:16
    #4 0x41f4ed in _start (/home/ubuntu178/cvelibf/test/tcpreplay/latest/bin_asan/bin/tcpprep+0x41f4ed)

AddressSanitizer can not provide additional info.
SUMMARY: AddressSanitizer: SEGV /home/ubuntu178/cvelibf/test/tcpreplay/latest/build_asan/src/common/../../../code/src/common/mac.c:120:9 in macinstring
==3017398==ABORTING
```

### GDB report
```
$ gdb --args ./bin_normal/bin/tcpprep -e "" --pcap=./code/test/test.pcap --cachefile=/dev/null

(gdb) r
Starting program: /home/ubuntu178/cvelibf/test/tcpreplay/latest/bin_normal/bin/tcpprep -e '' --pcap=./code/test/test.pcap --cachefile=/dev/null

Program received signal SIGSEGV, Segmentation fault.
0x0000555555562b50 in macinstring (macstring=0x55555558dbf0 "", mac=0x55555558f0e6 "\370\036\337\345\204:\b") at mac.c:120
120         if (strlen(tempstr)) {
(gdb) bt
#0  0x0000555555562b50 in macinstring (macstring=0x55555558dbf0 "", mac=0x55555558f0e6 "\370\036\337\345\204:\b") at mac.c:120
#1  0x00005555555596b6 in process_raw_packets (pcap=0x55555558ee40) at tcpprep.c:450
#2  0x0000555555558d23 in main (argc=5, argv=0x7fffffffc2d8) at tcpprep.c:144
```

# cmd-tcpprep-bcb107a-cidr2cidr-assertion
There is a reachable assertion in `cidr2cidr()` when the user passes empty cidr string to `tcpprep` in CIDR-split mode.  

## Test Environment
Ubuntu 20.04, 64bit  
tcpreplay (master bcb107a)

```
./bin_normal/bin/tcprewrite -V
tcprewrite version: 4.4.3 (build git:v4.4.3)
Copyright 2013-2022 by Fred Klassen <tcpreplay at appneta dot com> - AppNeta
Copyright 2000-2012 by Aaron Turner <aturner at synfin dot net>
The entire Tcpreplay Suite is licensed under the GPLv3
Cache file supported: 04
Not compiled with libdnet.
Compiled against libpcap: 1.9.1
64 bit packet counters: enabled
Verbose printing via tcpdump: enabled
Fragroute engine: disabled
```

## How to trigger
1. Get the Tcpreplay source code and compile it.
  ```
  $ ./configure
  $ make
  ```
2. Run Command `$ ./tcpprep --cidr="" --pcap=./test.pcap --cachefile=/dev/null`  
  The file `test.pcap` is from tcpreplay codebase, which is located in `test/test.pcap`.

## Details
This crash is due to the program does not check the string returned by `strtok_r()`.

### GDB report
```
$ gdb --args ./bin_normal/bin/tcpprep --cidr="" --pcap=./code/test/test.pcap --cachefile=/dev/null

(gdb) r
Starting program: /home/ubuntu178/cvelibf/test/tcpreplay/latest/bin_normal/bin/tcpprep --cidr= --pcap=./code/test/test.pcap --cachefile=/dev/null
tcpprep: cidr.c:178: cidr2cidr: Assertion `cidr' failed.

Program received signal SIGABRT, Aborted.
__GI_raise (sig=sig@entry=6) at ../sysdeps/unix/sysv/linux/raise.c:50
50      ../sysdeps/unix/sysv/linux/raise.c: No such file or directory.
(gdb) bt
#0  __GI_raise (sig=sig@entry=6) at ../sysdeps/unix/sysv/linux/raise.c:50
#1  0x00007ffff7d6d859 in __GI_abort () at abort.c:79
#2  0x00007ffff7d6d729 in __assert_fail_base (fmt=0x7ffff7f03588 "%s%s%s:%u: %s%sAssertion `%s' failed.\n%n", assertion=0x555555565d1e "cidr", file=0x555555565d14 "cidr.c", line=178, function=<optimized out>)
    at assert.c:92
#3  0x00007ffff7d7ef36 in __GI___assert_fail (assertion=0x555555565d1e "cidr", file=0x555555565d14 "cidr.c", line=178, function=0x555555565dd0 <__PRETTY_FUNCTION__.6522> "cidr2cidr") at assert.c:101
#4  0x000055555555daa4 in cidr2cidr (cidr=0x0) at cidr.c:178
#5  0x000055555555de56 in parse_cidr (cidrdata=0x55555556db20, cidrin=0x55555558dbf0 "", delim=0x555555564d61 ",") at cidr.c:299
#6  0x0000555555557e6c in doOptCidr (pOptions=0x55555556bc00 <tcpprepOptions>, pOptDesc=0x55555556b0f0 <optDesc+208>) at tcpprep_opts.c:1217
#7  0x00007ffff7f4111e in ?? () from /lib/x86_64-linux-gnu/libopts.so.25
#8  0x00007ffff7f49964 in ?? () from /lib/x86_64-linux-gnu/libopts.so.25
#9  0x00007ffff7f4c7c8 in optionProcess () from /lib/x86_64-linux-gnu/libopts.so.25
#10 0x000055555555899c in main (argc=4, argv=0x7fffffffc2d8) at tcpprep.c:89
```

# cmd-tcprewrite-bcb107a-parse_endpoints-SEGV
There is a NULL Pointer Dereference in `parse_endpoints()` when the user passes empty endpoints string to `tcprewrite` with option `--endpoints`.  


## Test Environment
Ubuntu 20.04, 64bit  
tcpreplay (master bcb107a)

```
./bin_normal/bin/tcprewrite -V
tcprewrite version: 4.4.3 (build git:v4.4.3)
Copyright 2013-2022 by Fred Klassen <tcpreplay at appneta dot com> - AppNeta
Copyright 2000-2012 by Aaron Turner <aturner at synfin dot net>
The entire Tcpreplay Suite is licensed under the GPLv3
Cache file supported: 04
Not compiled with libdnet.
Compiled against libpcap: 1.9.1
64 bit packet counters: enabled
Verbose printing via tcpdump: enabled
Fragroute engine: disabled
```

## How to trigger
1. Get the Tcpreplay source code and compile it.
  ```
  $ ./configure
  $ make
  ```
2. Generate cache file `$ ./tcpprep --port --pcap=./test.pcap --cachefile=./test.cache`  
   The file `test.pcap` is from tcpreplay codebase, which is located in `test/test.pcap`.
3. Run Command `$ ./tcprewrite --endpoints="" -i ./test.pcap -o /dev/null --cachefile=./test.cache`  

## Details
This crash is due to the program does not check the string returned by `strtok_r()` in `parse_endpoints()`.

### GDB report
```
$ gdb --args ./bin_normal/bin/tcprewrite --endpoints="" -i ./code/test/test.pcap -o /dev/null --cachefile=./test.cache

(gdb) r
Starting program: /home/ubuntu178/cvelibf/test/tcpreplay/latest/bin_normal/bin/tcprewrite --endpoints= -i ./code/test/test.pcap -o /dev/null --cachefile=./test.cache

Program received signal SIGSEGV, Segmentation fault.
0x000055555556fde2 in strlcat (dst=0x7fffffffc0aa "", src=0x0, dsize=92) at strlcat.c:45
45              while (*src != '\0') {
(gdb) bt
#0  0x000055555556fde2 in strlcat (dst=0x7fffffffc0aa "", src=0x0, dsize=92) at strlcat.c:45
#1  0x000055555556b6f0 in parse_endpoints (cidrmap1=0x555555580850, cidrmap2=0x555555580858, optarg=0x7fffffffc654 "") at cidr.c:367
#2  0x000055555555b51c in tcpedit_post_args (tcpedit=0x55555557fff0) at parse_args.c:243
#3  0x0000555555558c23 in main (argc=0, argv=0x7fffffffc2f0) at tcprewrite.c:89
```

# cmd-tcprewrite-bcb107a-ports2PORT-assertion
There is a reachable assertion in `ports2PORT()` when the user passes empty portmap string to `tcprewrite` with option `--portmap`.  


## Test Environment
Ubuntu 20.04, 64bit  
tcpreplay (master bcb107a)

```
./bin_normal/bin/tcprewrite -V
tcprewrite version: 4.4.3 (build git:v4.4.3)
Copyright 2013-2022 by Fred Klassen <tcpreplay at appneta dot com> - AppNeta
Copyright 2000-2012 by Aaron Turner <aturner at synfin dot net>
The entire Tcpreplay Suite is licensed under the GPLv3
Cache file supported: 04
Not compiled with libdnet.
Compiled against libpcap: 1.9.1
64 bit packet counters: enabled
Verbose printing via tcpdump: enabled
Fragroute engine: disabled
```

## How to trigger
1. Get the Tcpreplay source code and compile it.
  ```
  $ ./configure
  $ make
  ```
2. Run Command `$ ./tcprewrite --endpoints="" -i ./test.pcap -o /dev/null --cachefile=./test.cache`  
   The file `test.pcap` is from tcpreplay codebase, which is located in `test/test.pcap`.

## Details
This crash is due to the program does not check the string returned by `strtok_r()` in `parse_portmap()`.

### GDB report
```
$ gdb --args ./bin_normal/bin/tcprewrite --portmap="" -i ./code/test/test.pcap -o /dev/null

(gdb) r
Starting program: /home/ubuntu178/cvelibf/test/tcpreplay/latest/bin_normal/bin/tcprewrite --portmap= -i ./code/test/test.pcap -o /dev/null
tcprewrite: portmap.c:69: ports2PORT: Assertion `ports' failed.

Program received signal SIGABRT, Aborted.
__GI_raise (sig=sig@entry=6) at ../sysdeps/unix/sysv/linux/raise.c:50
50      ../sysdeps/unix/sysv/linux/raise.c: No such file or directory.
(gdb) bt
#0  __GI_raise (sig=sig@entry=6) at ../sysdeps/unix/sysv/linux/raise.c:50
#1  0x00007ffff7d6d859 in __GI_abort () at abort.c:79
#2  0x00007ffff7d6d729 in __assert_fail_base (fmt=0x7ffff7f03588 "%s%s%s:%u: %s%sAssertion `%s' failed.\n%n", assertion=0x55555557242a "ports", file=0x555555572420 "portmap.c", line=69, function=<optimized out>)
    at assert.c:92
#3  0x00007ffff7d7ef36 in __GI___assert_fail (assertion=0x55555557242a "ports", file=0x555555572420 "portmap.c", line=69, function=0x5555555725c0 <__PRETTY_FUNCTION__.6999> "ports2PORT") at assert.c:101
#4  0x000055555555e28d in ports2PORT (ports=0x0) at portmap.c:69
#5  0x000055555555e83a in parse_portmap (portmap=0x555555580848, ourstr=0x55555557e2a0 "") at portmap.c:197
#6  0x000055555555b2e3 in tcpedit_post_args (tcpedit=0x55555557ffc0) at parse_args.c:191
#7  0x0000555555558c23 in main (argc=0, argv=0x7fffffffc2f8) at tcprewrite.c:89
```

# poc-tcprewrite-bcb107a-read_hexstring-SEGV
There is a NULL Pointer Dereference in `read_hexstring()` when the user passes empty user dlink string to `tcprewrite` with option `--user-dlink` when the program process the pcap file whose data link type is DLT_USER0.  


## Test Environment
Ubuntu 20.04, 64bit  
tcpreplay (master bcb107a)

```
./bin_normal/bin/tcprewrite -V
tcprewrite version: 4.4.3 (build git:v4.4.3)
Copyright 2013-2022 by Fred Klassen <tcpreplay at appneta dot com> - AppNeta
Copyright 2000-2012 by Aaron Turner <aturner at synfin dot net>
The entire Tcpreplay Suite is licensed under the GPLv3
Cache file supported: 04
Not compiled with libdnet.
Compiled against libpcap: 1.9.1
64 bit packet counters: enabled
Verbose printing via tcpdump: enabled
Fragroute engine: disabled
```

## How to trigger
1. Get the Tcpreplay source code and compile it.
  ```
  $ ./configure
  $ make
  ```
1. Run Command `$ ./tcprewrite --user-dlink="" -i $POC -o /dev/null`  
   The POC file could be downloaded in [POC file](https://raw.githubusercontent.com/Marsman1996/pocs/master/tcpreplay/poc-tcprewrite-bcb107a-read_hexstring-SEGV)

## Details
This crash is due to the program does not check the string returned by `strtok_r()` in `read_hexstring()`.

### GDB report
```
$ gdb --args ./bin_normal/bin/tcprewrite --user-dlink="" -i ./poc-tcprewrite-bcb107a-read_hexstring-SEGV -o /dev/null

(gdb) r
Starting program: /home/ubuntu178/cvelibf/test/tcpreplay/latest/bin_normal/bin/tcprewrite --user-dlink= -i ./poc-tcprewrite-bcb107a-read_hexstring-SEGV -o /dev/null

Program received signal SIGSEGV, Segmentation fault.
__rawmemchr_avx2 () at ../sysdeps/x86_64/multiarch/memchr-avx2.S:65
65      ../sysdeps/x86_64/multiarch/memchr-avx2.S: No such file or directory.
(gdb) bt
#0  __rawmemchr_avx2 () at ../sysdeps/x86_64/multiarch/memchr-avx2.S:65
#1  0x00007ffff7dddf36 in _IO_str_init_static_internal (sf=sf@entry=0x7fffffffbe30, ptr=ptr@entry=0x0, size=size@entry=0, pstart=pstart@entry=0x0) at strops.c:41
#2  0x00007ffff7dad4c3 in _IO_strfile_read (string=0x0, sf=0x7fffffffbe30) at ../libio/strfile.h:95
#3  __GI___isoc99_sscanf (s=0x0, format=0x555555576598 "%x") at isoc99_sscanf.c:28
#4  0x000055555556e7cf in read_hexstring (l2string=0x55555557e2a0 "", hex=0x555555581367 "", hexlen=255) at utils.c:309
#5  0x00005555555656ae in dlt_user_parse_opts (ctx=0x555555580890) at plugins/dlt_user/user.c:194
#6  0x000055555556169c in tcpedit_dlt_parse_opts (ctx=0x555555580890) at plugins/dlt_utils.c:39
#7  0x0000555555560ca5 in tcpedit_dlt_post_args (tcpedit=0x55555557ffe0) at plugins/dlt_plugins.c:210
#8  0x000055555555b575 in tcpedit_post_args (tcpedit=0x55555557ffe0) at parse_args.c:252
#9  0x0000555555558c23 in main (argc=0, argv=0x7fffffffc2e8) at tcprewrite.c:89
```