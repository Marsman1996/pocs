# cmd-osdk-b1ea422-get_src_path-panic
Reachable panic in `get_src_path()` caused by unexpected conversion from `-` to `_`

### Describe the bug
If user executes the command as the original README.md, it will panic in src/commands/new/mod.rs:177

https://github.com/asterinas/asterinas/blob/b1ea422efaf6b0becd7d6cd99d270ae01fcd12de/osdk/src/commands/new/mod.rs#L163-L178

This is caused by the convert from `-` to `_`, as a result, OSDK cannot match crate_name in the loop.

The conversion happened in `get_cargo_metadata()`:
https://github.com/asterinas/asterinas/blob/b1ea422efaf6b0becd7d6cd99d270ae01fcd12de/osdk/src/util.rs#L41-L64  

Here OSDK runs `cargo metadata --no-deps --format-version 1`, and get the result as:
```json
{
      "name": "my-first-os",
      "version": "0.1.0",
      "id": "path+file:///root/asterinas/my-first-os#0.1.0",
      "license": null,
      "license_file": null,
      "description": null,
      "source": null,
      "dependencies": [],
      "targets": [
        {
          "kind": [
            "lib"
          ],
          "crate_types": [
            "lib"
          ],
          "name": "my_first_os",
          "src_path": "/root/asterinas/my-first-os/src/lib.rs",
          "edition": "2021",
          "doc": true,
          "doctest": true,
          "test": true
        }
      ],
```

Finally, in `get_src_path`, the `name` variable is from the `targets`, `name` keywords, which is `my_first_os` here.

### To Reproduce
1. Create a OS crate containing `-`, such as `cargo osdk new --kernel my-first-os`

### Expected behavior
OSDK reports panic and is terminated.


# cmd-asterinas-subtract with overflow
There is a subtract with overflow in `print_stack_trace()` when print stack trace of previous panic.

### Reference
https://github.com/asterinas/asterinas/issues/1167

### Describe the bug
Asterinas calls print_stack_trace() when panic, and the `pc` could be `0`, which causes a subtract with overflow panic in `_Unwind_FindEnclosingFunction()`.

https://github.com/asterinas/asterinas/blob/4844e7ca7ca6d78896a51a71487a6fdfe9ca6654/ostd/src/panicking.rs#L75-L76

```rust
pub extern "C" fn _Unwind_FindEnclosingFunction(pc: *mut c_void) -> *mut c_void {
    find_fde::get_finder()
        .find_fde(pc as usize - 1) // <- subtract with overflow here
        .map(|r| r.fde.initial_address() as usize as _)
        .unwrap_or(ptr::null_mut())
}
```

```
Breakpoint 1, ostd::panicking::print_stack_trace::callback (unwind_ctx=0xffff80007f4a9b48, arg=0xffff80007f4aa370) at src/panicking.rs:76
76              let fde_initial_address = _Unwind_FindEnclosingFunction(pc as *mut c_void) as usize;
1: pc = 0
(gdb) b _Unwind_FindEnclosingFunction
Breakpoint 2 at 0xffffffff888dc9e4: file src/unwinder/mod.rs, line 126.
(gdb) c
Continuing.

Breakpoint 2, unwinding::unwinder::_Unwind_FindEnclosingFunction (pc=0x0) at src/unwinder/mod.rs:126
126         find_fde::get_finder()
(gdb) n
127             .find_fde(pc as usize - 1)
(gdb) p pc
$1 = (*mut core::ffi::c_void) 0x0
```

### To Reproduce

1. To trigger a panic, for example run `$ ping www`.

### Expected behavior

During the stack trace print process, Asterinas panic again.


### Environment
- Official docker asterinas/asterinas:0.6.2
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 4844e7c


### Logs

```
BdsDxe: loading Boot0001 "UEFI QEMU DVD-ROM QM00005 " from PciRoot(0x0)/Pci(0x1F,0x2)/Sata(0x2,0xFFFF,0x0)
BdsDxe: starting Boot0001 "UEFI QEMU DVD-ROM QM00005 " from PciRoot(0x0)/Pci(0x1F,0x2)/Sata(0x2,0xFFFF,0x0)
WARNING: no console will be available to OS
error: no suitable video mode found.
[kernel] finish init ostd
Loopback ipaddr: 127.0.0.1
DHCP update IP address: Address([10, 0, 2, 15])
Default router address: Address([10, 0, 2, 2])
[kernel] unpacking the initramfs.cpio.gz to rootfs ...
[kernel] rootfs is ready
[kernel] Spawn init thread, tid = 0
[kernel] Mount Ext2 fs at FsPath { inner: Absolute("/ext2") } 
[kernel] Mount ExFat fs at FsPath { inner: Absolute("/exfat") } 
[kernel] Hello world from kernel!


   _   ___ _____ ___ ___ ___ _  _   _   ___
  /_\ / __|_   _| __| _ \_ _| \| | /_\ / __|
 / _ \\__ \ | | | _||   /| || .` |/ _ \\__ \
/_/ \_\___/ |_| |___|_|_\___|_|\_/_/ \_\___/


~ # ping www
panicked at /root/asterinas/kernel/aster-nix/src/util/net/options/mod.rs:137:14:
not yet implemented
Printing stack trace:
   1: fn 0xffffffff88837ff0 - pc 0xffffffff88838008 / registers:

     rax               0x12; rdx 0xffffffff889edf28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4aa370;

   2: fn 0xffffffff88837dd0 - pc 0xffffffff88837f48 / registers:

     rax               0x12; rdx 0xffffffff889edf28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4aa380;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax               0x12; rdx 0xffffffff889edf28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4aa500;

   4: fn 0xffffffff8895ce60 - pc 0xffffffff8895cee2 / registers:

     rax               0x12; rdx 0xffffffff889edf28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4aa510;

   5: fn 0xffffffff8895d000 - pc 0xffffffff8895d040 / registers:

     rax               0x12; rdx 0xffffffff889edf28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4aa5a0;

   6: fn 0xffffffff884c4c10 - pc 0xffffffff884c4c5c / registers:

     rax               0x12; rdx 0xffffffff889edf28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4aa610;

   7: fn 0xffffffff88354c10 - pc 0xffffffff8835509b / registers:

     rax               0x12; rdx 0xffffffff889edf28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4aa640;

   8: fn 0xffffffff88407d50 - pc 0xffffffff88418694 / registers:

     rax               0x12; rdx 0xffffffff889edf28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4aab40;

   9: fn 0xffffffff8834bd20 - pc 0xffffffff8834bdae / registers:

     rax               0x12; rdx 0xffffffff889edf28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4c03d0;

  10: fn 0xffffffff88520700 - pc 0xffffffff8852127f / registers:

     rax               0x12; rdx 0xffffffff889edf28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4c0570;

  11: fn 0xffffffff88075a80 - pc 0xffffffff88075a8e / registers:

     rax               0x12; rdx 0xffffffff889edf28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4c0f90;

  12: fn 0xffffffff8882cba0 - pc 0xffffffff8882cbb6 / registers:

     rax               0x12; rdx 0xffffffff889edf28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4c0fb0;

  13: fn 0xffffffff88798ad0 - pc 0xffffffff88798b39 / registers:

     rax               0x12; rdx 0xffffffff889edf28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4c0fd0;

panicked at /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.2/src/unwinder/mod.rs:127:19:
attempt to subtract with overflow
Printing stack trace:
   1: fn 0xffffffff88837ff0 - pc 0xffffffff88838008 / registers:

     rax               0x12; rdx 0xffffffff889edf28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4a7f90;

   2: fn 0xffffffff88837dd0 - pc 0xffffffff88837f48 / registers:

     rax               0x12; rdx 0xffffffff889edf28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4a7fa0;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax               0x12; rdx 0xffffffff889edf28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4a8120;

   4: fn 0xffffffff8895ce60 - pc 0xffffffff8895cee2 / registers:

     rax               0x12; rdx 0xffffffff889edf28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4a8130;

   5: fn 0xffffffff889386a0 - pc 0xffffffff889386ce / registers:

     rax               0x12; rdx 0xffffffff889edf28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4a81c0;

   6: fn 0xffffffff888dc9d0 - pc 0xffffffff888dca68 / registers:

     rax               0x12; rdx 0xffffffff889edf28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4a8210;

   7: fn 0xffffffff88838010 - pc 0xffffffff88838074 / registers:

     rax               0x12; rdx 0xffffffff889edf28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4a8370;

   8: fn 0xffffffff888dd4a0 - pc 0xffffffff888dd644 / registers:

     rax               0x12; rdx 0xffffffff889edf28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4a8a40;

   9: fn 0xffffffff888dc6a0 - pc 0xffffffff888dc6dd / registers:

     rax               0x12; rdx 0xffffffff889edf28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4aa220;

  10: fn 0xffffffff888ed720 - pc 0xffffffff888ed779 / registers:

     rax               0x12; rdx 0xffffffff889edf28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4aa260;

  11: fn                0x0 - pc               0x12 / registers:

     rax               0x12; rdx 0xffffffff889edf28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4aa268;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:104
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) au2ndt6r5ksv2yyhocg58xyv7:?
(  4) ??:?
(  5) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/panicking.rs:220
(  6) bbb49vpic3vkzib2z19iplvgo:?
(  7) /root/asterinas/kernel/aster-nix/src/syscall/setsockopt.rs:31
(  8) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:193
(  9) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:323
( 10) /root/asterinas/kernel/aster-nix/src/thread/task.rs:69
( 11) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 12) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 13) /root/asterinas/ostd/src/task/task.rs:295
[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
( 14) /root/asterinas/ostd/src/panicking.rs:104
( 15) /root/asterinas/ostd/src/panicking.rs:59
( 16) au2ndt6r5ksv2yyhocg58xyv7:?
( 17) ??:?
( 18) ??:?
( 19) ??:?
( 20) /root/asterinas/ostd/src/panicking.rs:76
( 21) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.2/src/unwinder/mod.rs:404
( 22) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.2/src/unwinder/mod.rs:34
( 23) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.2/src/unwinder/arch/x86_64.rs:64
```

# syscall-mmap-4844e7c-get_intersected_range-panic
Reachable assertion in `get_intersected_range()` caused by invalid `size` of `mmap`/`mprotect`/`munmap`/`madvise` syscall

### Reference
https://github.com/asterinas/asterinas/issues/1172

### Describe the bug
There is a reachable assertion in `get_intersected_range()` at kernel/aster-nix/src/vm/vmar/mod.rs:844 when make a `mmap` syscall with specific arguments.

https://github.com/asterinas/asterinas/blob/4844e7ca7ca6d78896a51a71487a6fdfe9ca6654/kernel/aster-nix/src/vm/vmar/mod.rs#L841-L846

### To Reproduce
1. Compile a program which calls `mmap` with specific arguments:
```C
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

int main(void)
{
  int fd = open("/proc/meminfo", O_RDONLY);
  mmap(NULL, 0, 3, 460833, fd, 0);
  return 0;
}

```
2. Run the compiled program in Asterinas.


### Expected behavior
Asterinas reports assertion failure and is terminated.

### Environment

- Official docker asterinas/asterinas:0.6.2
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 4844e7c

### Logs

```
~ # /root/mmap_test 
panicked at /root/asterinas/kernel/aster-nix/src/vm/vmar/mod.rs:844:5:
assertion failed: is_intersected(range1, range2)
Printing stack trace:
   1: fn 0xffffffff88837ff0 - pc 0xffffffff88838008 / registers:

     rax               0x12; rdx 0xffffffff889edf38; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4a8b10;

   2: fn 0xffffffff88837dd0 - pc 0xffffffff88837f48 / registers:

     rax               0x12; rdx 0xffffffff889edf38; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4a8b20;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax               0x12; rdx 0xffffffff889edf38; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4a8ca0;

   4: fn 0xffffffff8895ce70 - pc 0xffffffff8895cef2 / registers:

     rax               0x12; rdx 0xffffffff889edf38; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4a8cb0;

   5: fn 0xffffffff8895d010 - pc 0xffffffff8895d050 / registers:

     rax               0x12; rdx 0xffffffff889edf38; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4a8d40;

   6: fn 0xffffffff883fdc90 - pc 0xffffffff883fdcc9 / registers:

     rax               0x12; rdx 0xffffffff889edf38; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4a8db0;

   7: fn 0xffffffff883fafa0 - pc 0xffffffff883fbd1c / registers:

     rax               0x12; rdx 0xffffffff889edf38; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4a8de0;

   8: fn 0xffffffff8804ba60 - pc 0xffffffff8804bc05 / registers:

     rax               0x12; rdx 0xffffffff889edf38; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4a98d0;

   9: fn 0xffffffff8804c350 - pc 0xffffffff8804c4f4 / registers:

     rax               0x12; rdx 0xffffffff889edf38; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4a9cd0;

  10: fn 0xffffffff88516010 - pc 0xffffffff88516c98 / registers:

     rax               0x12; rdx 0xffffffff889edf38; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4aa000;

  11: fn 0xffffffff88515d50 - pc 0xffffffff88515e95 / registers:

     rax               0x12; rdx 0xffffffff889edf38; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4aa9b0;

  12: fn 0xffffffff88407d50 - pc 0xffffffff8840c8ae / registers:

     rax               0x12; rdx 0xffffffff889edf38; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4aab40;

  13: fn 0xffffffff8834bd20 - pc 0xffffffff8834bdae / registers:

     rax               0x12; rdx 0xffffffff889edf38; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4c03d0;

  14: fn 0xffffffff88520700 - pc 0xffffffff8852127f / registers:

     rax               0x12; rdx 0xffffffff889edf38; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4c0570;

  15: fn 0xffffffff88075a80 - pc 0xffffffff88075a8e / registers:

     rax               0x12; rdx 0xffffffff889edf38; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4c0f90;

  16: fn 0xffffffff8882cba0 - pc 0xffffffff8882cbb6 / registers:

     rax               0x12; rdx 0xffffffff889edf38; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4c0fb0;

  17: fn 0xffffffff88798ad0 - pc 0xffffffff88798b39 / registers:

     rax               0x12; rdx 0xffffffff889edf38; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4c0fd0;

  18: fn                0x0 - pc                0x0 / registers:

     rax               0x12; rdx 0xffffffff889edf38; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4c1000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:107
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) au2ndt6r5ksv2yyhocg58xyv7:?
(  4) ??:?
(  5) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/panicking.rs:220
(  6) ??:?
(  7) /root/asterinas/kernel/aster-nix/src/vm/vmar/mod.rs:661
(  8) /root/asterinas/kernel/aster-nix/src/vm/vmar/vm_mapping.rs:88
(  9) /root/asterinas/kernel/aster-nix/src/vm/vmar/vm_mapping.rs:741
( 10) /root/asterinas/kernel/aster-nix/src/syscall/mmap.rs:89
( 11) /root/asterinas/kernel/aster-nix/src/syscall/mmap.rs:29
( 12) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:215
( 13) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:323
( 14) /root/asterinas/kernel/aster-nix/src/thread/task.rs:69
( 15) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 16) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 17) /root/asterinas/ostd/src/task/task.rs:295
make: *** [Makefile:153: run] Error 1
```

# syscall-rt_sigprocmask-4844e7c-sys_rt_sigprocmask-panic
Reachable unwrap in `sys_rt_sigprocmask()` by invalid `how`

### Reference
https://github.com/asterinas/asterinas/issues/1173

### Describe the bug
There is a reachable unwrap in `sys_rt_sigprocmask()` at kernel/aster-nix/src/syscall/rt_sigprocmask.rs:21 when make a `rt_sigprocmask ` syscall with specific arguments.

https://github.com/asterinas/asterinas/blob/4844e7ca7ca6d78896a51a71487a6fdfe9ca6654/kernel/aster-nix/src/syscall/rt_sigprocmask.rs#L14-L21

### To Reproduce
1. Compile a program which calls `mmap` with specific arguments:
```C
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

int main() {
    int how = 0xfffc;
    sigset_t set;
    sigset_t oset;
    size_t sigsetsize = 128;

    memset(&set, 0, sizeof(set));
    memset(&oset, 0, sizeof(oset));
    sigaddset(&set, 3);

    long result = syscall(SYS_rt_sigprocmask, how, &set, &oset, sigsetsize);

    return 0;
}
```
2. Run the compiled program in Asterinas.

### Expected behavior

Asterinas reports panic and is terminated.

### Environment

- Official docker asterinas/asterinas:0.6.2
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 4844e7c

### Logs
```
~ # /root/rt_sigprocmask-test 
panicked at /root/asterinas/kernel/aster-nix/src/syscall/rt_sigprocmask.rs:21:41:
called `Result::unwrap()` on an `Err` value: InvalidValue
Printing stack trace:
   1: fn 0xffffffff88838040 - pc 0xffffffff88838058 / registers:

     rax               0x12; rdx 0xffffffff889ede68; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f72a540;

   2: fn 0xffffffff88837e20 - pc 0xffffffff88837f98 / registers:

     rax               0x12; rdx 0xffffffff889ede68; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f72a550;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax               0x12; rdx 0xffffffff889ede68; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f72a6d0;

   4: fn 0xffffffff8895cec0 - pc 0xffffffff8895cf42 / registers:

     rax               0x12; rdx 0xffffffff889ede68; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f72a6e0;

   5: fn 0xffffffff8893a1f0 - pc 0xffffffff8893a31b / registers:

     rax               0x12; rdx 0xffffffff889ede68; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f72a770;

   6: fn 0xffffffff883c4690 - pc 0xffffffff883c471b / registers:

     rax               0x12; rdx 0xffffffff889ede68; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f72a870;

   7: fn 0xffffffff8840ade0 - pc 0xffffffff88411190 / registers:

     rax               0x12; rdx 0xffffffff889ede68; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f72ab40;

   8: fn 0xffffffff8840ab00 - pc 0xffffffff8840ab8e / registers:

     rax               0x12; rdx 0xffffffff889ede68; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f7403d0;

   9: fn 0xffffffff88524040 - pc 0xffffffff88524bbf / registers:

     rax               0x12; rdx 0xffffffff889ede68; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f740570;

  10: fn 0xffffffff882259d0 - pc 0xffffffff882259de / registers:

     rax               0x12; rdx 0xffffffff889ede68; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f740f90;

  11: fn 0xffffffff8882cbf0 - pc 0xffffffff8882cc06 / registers:

     rax               0x12; rdx 0xffffffff889ede68; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f740fb0;

  12: fn 0xffffffff88798b20 - pc 0xffffffff88798b89 / registers:

     rax               0x12; rdx 0xffffffff889ede68; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f740fd0;

  13: fn                0x0 - pc                0x0 / registers:

     rax               0x12; rdx 0xffffffff889ede68; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f741000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:107
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) au2ndt6r5ksv2yyhocg58xyv7:?
(  4) ??:?
(  5) ??:?
(  6) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/result.rs:1102
(  7) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:173
(  8) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:323
(  9) /root/asterinas/kernel/aster-nix/src/thread/task.rs:69
( 10) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 11) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 12) /root/asterinas/ostd/src/task/task.rs:295
make: *** [Makefile:153: run] Error 1
```

# syscall-rt_sigprocmask-4844e7c-sys_rt_sigprocmask-panic-2
Reachable unwrap in `sys_rt_sigprocmask()` by invalid `oldset_ptr`

### Reference
https://github.com/asterinas/asterinas/issues/1175

### Describe the bug

There is a reachable unwrap caused by Bad user space pointer specified in `sys_rt_sigprocmask()` at kernel/aster-nix/src/syscall/rt_sigprocmask.rs:29 when make a `rt_sigprocmask` syscall with specific arguments.
https://github.com/asterinas/asterinas/blob/0291b5dc6bb142b9c6165c1cb29b7658eefdaa63/kernel/aster-nix/src/syscall/rt_sigprocmask.rs#L29-L30


### To Reproduce
1. Compile a program which calls `rt_sigprocmask` with specific arguments:
```C
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

int main() {
    syscall(SYS_rt_sigprocmask, 0x0, 0x8, 0x9, 128);

    return 0;
}
```
2. Run the compiled program in Asterinas.

### Expected behavior
Asterinas reports panic and is terminated.

### Environment
- Official docker asterinas/asterinas:0.6.2
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 0291b5dc

### Logs
```
~ # /root/rt_sigprocmask-test.c 
[    11.721] ERROR: sigset size is not equal to 8
panicked at /root/asterinas/kernel/aster-nix/src/syscall/rt_sigprocmask.rs:29:58:
called `Result::unwrap()` on an `Err` value: Error { errno: EFAULT, msg: Some("Bad user space pointer specified") }
Printing stack trace:
   1: fn 0xffffffff887cc8c0 - pc 0xffffffff887cc8d8 / registers:

     rax               0x12; rdx 0xffffffff889e5000; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6aa560;

   2: fn 0xffffffff887cc6a0 - pc 0xffffffff887cc818 / registers:

     rax               0x12; rdx 0xffffffff889e5000; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6aa570;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax               0x12; rdx 0xffffffff889e5000; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6aa6f0;

   4: fn 0xffffffff8895b780 - pc 0xffffffff8895b802 / registers:

     rax               0x12; rdx 0xffffffff889e5000; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6aa700;

   5: fn 0xffffffff88938ab0 - pc 0xffffffff88938bdb / registers:

     rax               0x12; rdx 0xffffffff889e5000; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6aa790;

   6: fn 0xffffffff884809b0 - pc 0xffffffff88480d7d / registers:

     rax               0x12; rdx 0xffffffff889e5000; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6aa890;

   7: fn 0xffffffff883b1520 - pc 0xffffffff883b78d0 / registers:

     rax               0x12; rdx 0xffffffff889e5000; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6aab40;

   8: fn 0xffffffff88352cf0 - pc 0xffffffff88352d7e / registers:

     rax               0x12; rdx 0xffffffff889e5000; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6c03d0;

   9: fn 0xffffffff88446ab0 - pc 0xffffffff8844762f / registers:

     rax               0x12; rdx 0xffffffff889e5000; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6c0570;

  10: fn 0xffffffff8808cc00 - pc 0xffffffff8808cc0e / registers:

     rax               0x12; rdx 0xffffffff889e5000; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6c0f90;

  11: fn 0xffffffff887a6ce0 - pc 0xffffffff887a6cf6 / registers:

     rax               0x12; rdx 0xffffffff889e5000; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6c0fb0;

  12: fn 0xffffffff888435b0 - pc 0xffffffff88843619 / registers:

     rax               0x12; rdx 0xffffffff889e5000; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6c0fd0;

  13: fn                0x0 - pc                0x0 / registers:

     rax               0x12; rdx 0xffffffff889e5000; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6c1000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:107
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) a3wadqd69fx2whpv190t0ri68:?
(  4) ??:?
(  5) ??:?
(  6) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/result.rs:1102
(  7) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:173
(  8) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:323
(  9) /root/asterinas/kernel/aster-nix/src/thread/task.rs:69
( 10) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 11) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 12) /root/asterinas/ostd/src/task/task/mod.rs:310
make: *** [Makefile:153: run] Error 1
```


# syscall-futex-0291b5d-sys_futex-unwrap-panic
Reachable unwrap in `sys_futex()` at kernel/aster-nix/src/syscall/futex.rs:22 by invalid `futex_op`

### Reference
https://github.com/asterinas/asterinas/issues/1179

### Describe the bug
There is a reachable unwrap caused by value convertion in `sys_futex()` at kernel/aster-nix/src/syscall/futex.rs:22 when make a `futex` syscall with specific arguments.
https://github.com/asterinas/asterinas/blob/0291b5dc6bb142b9c6165c1cb29b7658eefdaa63/kernel/aster-nix/src/syscall/futex.rs#L22

### To Reproduce
1. Compile a program which calls `rt_sigprocmask` with specific arguments:
```C
#include <errno.h>
#include <linux/futex.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <unistd.h>

int main() {
  uint32_t *uaddr = (uint32_t *)0x100068a0;
  uint32_t *uaddr2 = (uint32_t *)0x10006580;
  int op = 0xc;
  int val = 0x7fffffff;
  struct timespec timeout = {.tv_sec = 0, .tv_nsec = 1};
  int64_t val3 = 0xc0570d106a01;

  int result = syscall(SYS_futex, uaddr, op, val, &timeout, uaddr2, val3);

  return 0;
}
```
2. Run the compiled program in Asterinas.

### Expected behavior
Asterinas reports panic and is terminated.

### Environment
- Official docker asterinas/asterinas:0.6.2
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 0291b5d


### Logs

```
~ # /root/futex 
panicked at /root/asterinas/kernel/aster-nix/src/syscall/futex.rs:22:78:
called `Result::unwrap()` on an `Err` value: Error { errno: EINVAL, msg: Some("Unknown futex op") }
Printing stack trace:
   1: fn 0xffffffff887cc190 - pc 0xffffffff887cc1a8 / registers:

     rax               0x12; rdx 0xffffffff889e4728; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6aa490;

   2: fn 0xffffffff887cbf70 - pc 0xffffffff887cc0e8 / registers:

     rax               0x12; rdx 0xffffffff889e4728; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6aa4a0;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax               0x12; rdx 0xffffffff889e4728; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6aa620;

   4: fn 0xffffffff8895b050 - pc 0xffffffff8895b0d2 / registers:

     rax               0x12; rdx 0xffffffff889e4728; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6aa630;

   5: fn 0xffffffff88938380 - pc 0xffffffff889384ab / registers:

     rax               0x12; rdx 0xffffffff889e4728; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6aa6c0;

   6: fn 0xffffffff884636d0 - pc 0xffffffff884637ac / registers:

     rax               0x12; rdx 0xffffffff889e4728; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6aa7c0;

   7: fn 0xffffffff88394fa0 - pc 0xffffffff883bbff2 / registers:

     rax               0x12; rdx 0xffffffff889e4728; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6aab40;

   8: fn 0xffffffff8811a680 - pc 0xffffffff8811a70e / registers:

     rax               0x12; rdx 0xffffffff889e4728; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6c03d0;

   9: fn 0xffffffff880539e0 - pc 0xffffffff8805455f / registers:

     rax               0x12; rdx 0xffffffff889e4728; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6c0570;

  10: fn 0xffffffff8815ff90 - pc 0xffffffff8815ff9e / registers:

     rax               0x12; rdx 0xffffffff889e4728; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6c0f90;

  11: fn 0xffffffff887a65b0 - pc 0xffffffff887a65c6 / registers:

     rax               0x12; rdx 0xffffffff889e4728; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6c0fb0;

  12: fn 0xffffffff88842e80 - pc 0xffffffff88842ee9 / registers:

     rax               0x12; rdx 0xffffffff889e4728; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6c0fd0;

  13: fn                0x0 - pc                0x0 / registers:

     rax               0x12; rdx 0xffffffff889e4728; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6c1000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:107
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) a3wadqd69fx2whpv190t0ri68:?
(  4) ??:?
(  5) ??:?
(  6) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/result.rs:1102
(  7) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:215
(  8) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:323
(  9) /root/asterinas/kernel/aster-nix/src/thread/task.rs:69
( 10) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 11) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 12) /root/asterinas/ostd/src/task/task/mod.rs:310
make: *** [Makefile:153: run] Error 1
```


# syscall-futex-0291b5d-sys_futex-unwrap-panic-2
Reachable unwrap in `sys_futex()` by invalid `futex_val`

### Reference
https://github.com/asterinas/asterinas/issues/1179

### Describe the bug
There is a reachable unwrap caused by value convertion in `sys_futex()` at kernel/aster-nix/src/syscall/futex.rs:57 when make a `futex` syscall with specific arguments.
https://github.com/asterinas/asterinas/blob/0291b5dc6bb142b9c6165c1cb29b7658eefdaa63/kernel/aster-nix/src/syscall/futex.rs#L57

### To Reproduce
1. Compile a program which calls `rt_sigprocmask` with specific arguments:
```C
#include <errno.h>
#include <linux/futex.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <unistd.h>

int main() {
  struct timespec timeout = {.tv_sec = 0, .tv_nsec = 1};
  syscall(SYS_futex, (uint32_t *)0x53000, 0xa, -9, &timeout, (uint32_t *)0x10007580, 0x40404040);

  return 0;
}
```
2. Run the compiled program in Asterinas.

### Expected behavior
Asterinas reports panic and is terminated.

### Environment
- Official docker asterinas/asterinas:0.6.2
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 0291b5d


### Logs
```log
/root/futex.c 
panicked at /root/asterinas/kernel/aster-nix/src/syscall/futex.rs:57:61:
Invalid futex val: Error { errno: EINVAL, msg: Some("the futex val must not be negative") }
Printing stack trace:
   1: fn 0xffffffff887cc960 - pc 0xffffffff887cc978 / registers:

     rax               0x12; rdx 0xffffffff889e5040; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f02a310;

   2: fn 0xffffffff887cc740 - pc 0xffffffff887cc8b8 / registers:

     rax               0x12; rdx 0xffffffff889e5040; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f02a320;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax               0x12; rdx 0xffffffff889e5040; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f02a4a0;

   4: fn 0xffffffff8895b820 - pc 0xffffffff8895b8a2 / registers:

     rax               0x12; rdx 0xffffffff889e5040; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f02a4b0;

   5: fn 0xffffffff88938b50 - pc 0xffffffff88938c7b / registers:

     rax               0x12; rdx 0xffffffff889e5040; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f02a540;

   6: fn 0xffffffff883cb8a0 - pc 0xffffffff883cb928 / registers:

     rax               0x12; rdx 0xffffffff889e5040; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f02a640;

   7: fn 0xffffffff884550e0 - pc 0xffffffff88455707 / registers:

     rax               0x12; rdx 0xffffffff889e5040; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f02a6b0;

   8: fn 0xffffffff883ea380 - pc 0xffffffff884113d2 / registers:

     rax               0x12; rdx 0xffffffff889e5040; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f02ab40;

   9: fn 0xffffffff883ea0a0 - pc 0xffffffff883ea12e / registers:

     rax               0x12; rdx 0xffffffff889e5040; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f0403d0;

  10: fn 0xffffffff885a9320 - pc 0xffffffff885a9e9f / registers:

     rax               0x12; rdx 0xffffffff889e5040; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f040570;

  11: fn 0xffffffff8816caf0 - pc 0xffffffff8816cafe / registers:

     rax               0x12; rdx 0xffffffff889e5040; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f040f90;

  12: fn 0xffffffff887a6d80 - pc 0xffffffff887a6d96 / registers:

     rax               0x12; rdx 0xffffffff889e5040; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f040fb0;

  13: fn 0xffffffff88843650 - pc 0xffffffff888436b9 / registers:

     rax               0x12; rdx 0xffffffff889e5040; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f040fd0;

  14: fn                0x0 - pc                0x0 / registers:

     rax               0x12; rdx 0xffffffff889e5040; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f041000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:107
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) a3wadqd69fx2whpv190t0ri68:?
(  4) ??:?
(  5) ??:?
(  6) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/result.rs:1059
(  7) /root/asterinas/kernel/aster-nix/src/syscall/futex.rs:57
(  8) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:215
(  9) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:323
( 10) /root/asterinas/kernel/aster-nix/src/thread/task.rs:69
( 11) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 12) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 13) /root/asterinas/ostd/src/task/task/mod.rs:310
make: *** [Makefile:153: run] Error 1
```

# syscall-mprotect-0291b5d-sys_mprotect-panic
Reachable assert in `sys_mprotect()` by invalid `addr`

### Reference
https://github.com/asterinas/asterinas/issues/1184

### Describe the bug

There is a reachable assert caused by invalid `addr` value in `sys_mprotect()` at kernel/aster-nix/src/syscall/mprotect.rs:15 when make a `mprotect` syscall with specific arguments.
https://github.com/asterinas/asterinas/blob/0291b5dc6bb142b9c6165c1cb29b7658eefdaa63/kernel/aster-nix/src/syscall/mprotect.rs#L15


### To Reproduce
1. Compile a program which calls `mprotect` with specific arguments:
```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>

int main() {
  mprotect(0xffffffff, 0, 0x5);

  return EXIT_SUCCESS;
}
```
2. Run the compiled program in Asterinas.

### Expected behavior
Asterinas reports panic and is terminated.

### Environment
- Official docker asterinas/asterinas:0.6.2
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 0291b5dc

### Logs
```
~ # /root/mprotect.c 
panicked at /root/asterinas/kernel/aster-nix/src/syscall/mprotect.rs:15:5:
assertion failed: addr % PAGE_SIZE == 0
Printing stack trace:
   1: fn 0xffffffff887ccd10 - pc 0xffffffff887ccd28 / registers:

     rax               0x12; rdx 0xffffffff889e5488; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f2aa660;

   2: fn 0xffffffff887ccaf0 - pc 0xffffffff887ccc68 / registers:

     rax               0x12; rdx 0xffffffff889e5488; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f2aa670;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax               0x12; rdx 0xffffffff889e5488; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f2aa7f0;

   4: fn 0xffffffff8895bbd0 - pc 0xffffffff8895bc52 / registers:

     rax               0x12; rdx 0xffffffff889e5488; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f2aa800;

   5: fn 0xffffffff8895bd70 - pc 0xffffffff8895bdb0 / registers:

     rax               0x12; rdx 0xffffffff889e5488; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f2aa890;

   6: fn 0xffffffff884e8350 - pc 0xffffffff884e862d / registers:

     rax               0x12; rdx 0xffffffff889e5488; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f2aa900;

   7: fn 0xffffffff88519340 - pc 0xffffffff8851e37b / registers:

     rax               0x12; rdx 0xffffffff889e5488; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f2aab40;

   8: fn 0xffffffff8815cab0 - pc 0xffffffff8815cb3e / registers:

     rax               0x12; rdx 0xffffffff889e5488; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f2c03d0;

   9: fn 0xffffffff88567100 - pc 0xffffffff88567c7f / registers:

     rax               0x12; rdx 0xffffffff889e5488; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f2c0570;

  10: fn 0xffffffff880c9ef0 - pc 0xffffffff880c9efe / registers:

     rax               0x12; rdx 0xffffffff889e5488; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f2c0f90;

  11: fn 0xffffffff887a7130 - pc 0xffffffff887a7146 / registers:

     rax               0x12; rdx 0xffffffff889e5488; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f2c0fb0;

  12: fn 0xffffffff88843a00 - pc 0xffffffff88843a69 / registers:

     rax               0x12; rdx 0xffffffff889e5488; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f2c0fd0;

  13: fn                0x0 - pc                0x0 / registers:

     rax               0x12; rdx 0xffffffff889e5488; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f2c1000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:107
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) 86161ztg45fpd9b1tjbn4gsl1:?
(  4) ??:?
(  5) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/panicking.rs:220
(  6) ao06edr9di1q0vhz8iz1v26bt:?
(  7) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:166
(  8) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:323
(  9) /root/asterinas/kernel/aster-nix/src/thread/task.rs:69
( 10) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 11) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 12) /root/asterinas/ostd/src/task/task/mod.rs:310
make: *** [Makefile:153: run] Error 1
```


# syscall-madvise-0291b5dc-madv_free-panic
Reachable assert in `madv_free()` by invalid `start` and `len` of syscall `madvise`

### Reference
https://github.com/asterinas/asterinas/pull/1188

### Describe the bug

There is a reachable assert caused by invalid `start` and `len` value in `madv_free()` at kernel/aster-nix/src/syscall/madvise.rs:36 when make a `madvise` syscall with specific arguments.

https://github.com/asterinas/asterinas/blob/0291b5dc6bb142b9c6165c1cb29b7658eefdaa63/kernel/aster-nix/src/syscall/madvise.rs#36


### To Reproduce
1. Compile a program which calls `mprotect` with specific arguments:
```C
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

int main() {
  madvise((void *)0x291d002, 0, 0x8);
  void *addr = mmap(NULL, 4096, 0x3, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  madvise(addr, 0, 0x8);

  return EXIT_SUCCESS;
}
```
2. Run the compiled program in Asterinas.

### Expected behavior
Asterinas reports panic and is terminated.

### Environment
- Official docker asterinas/asterinas:0.6.2
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 0291b5dc

### logs
```
~ # /root/madvise.c 
panicked at /root/asterinas/kernel/aster-nix/src/syscall/madvise.rs:36:5:
assertion failed: start % PAGE_SIZE == 0
Printing stack trace:
   1: fn 0xffffffff887ccdf0 - pc 0xffffffff887cce08 / registers:

     rax               0x12; rdx 0xffffffff889e54a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f3aa4c0;

   2: fn 0xffffffff887ccbd0 - pc 0xffffffff887ccd48 / registers:

     rax               0x12; rdx 0xffffffff889e54a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f3aa4d0;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax               0x12; rdx 0xffffffff889e54a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f3aa650;

   4: fn 0xffffffff8895bcb0 - pc 0xffffffff8895bd32 / registers:

     rax               0x12; rdx 0xffffffff889e54a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f3aa660;

   5: fn 0xffffffff8895be50 - pc 0xffffffff8895be90 / registers:

     rax               0x12; rdx 0xffffffff889e54a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f3aa6f0;

   6: fn 0xffffffff880b2330 - pc 0xffffffff880b2397 / registers:

     rax               0x12; rdx 0xffffffff889e54a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f3aa760;

   7: fn 0xffffffff880b1c20 - pc 0xffffffff880b21b2 / registers:

     rax               0x12; rdx 0xffffffff889e54a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f3aa7f0;

   8: fn 0xffffffff8852ef80 - pc 0xffffffff885388ae / registers:

     rax               0x12; rdx 0xffffffff889e54a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f3aab40;

   9: fn 0xffffffff8852eb50 - pc 0xffffffff8852ebde / registers:

     rax               0x12; rdx 0xffffffff889e54a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f3c03d0;

  10: fn 0xffffffff880f4360 - pc 0xffffffff880f4edf / registers:

     rax               0x12; rdx 0xffffffff889e54a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f3c0570;

  11: fn 0xffffffff884084b0 - pc 0xffffffff884084be / registers:

     rax               0x12; rdx 0xffffffff889e54a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f3c0f90;

  12: fn 0xffffffff887a7210 - pc 0xffffffff887a7226 / registers:

     rax               0x12; rdx 0xffffffff889e54a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f3c0fb0;

  13: fn 0xffffffff88843ae0 - pc 0xffffffff88843b49 / registers:

     rax               0x12; rdx 0xffffffff889e54a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f3c0fd0;

  14: fn                0x0 - pc                0x0 / registers:

     rax               0x12; rdx 0xffffffff889e54a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f3c1000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:107
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) 86161ztg45fpd9b1tjbn4gsl1:?
(  4) ??:?
(  5) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/panicking.rs:220
(  6) 015v570fhs2wfmf9h68d8bdrd:?
(  7) /root/asterinas/kernel/aster-nix/src/syscall/madvise.rs:29
(  8) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:166
(  9) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:323
( 10) /root/asterinas/kernel/aster-nix/src/thread/task.rs:69
( 11) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 12) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 13) /root/asterinas/ostd/src/task/task/mod.rs:310
make: *** [Makefile:153: run] Error 1
```

# syscall-timer_settime-0291b5dc-find_posix_timer-outofbounds
index out of bounds in `find_posix_timer()` caused by invalid `timer_id` of `timer_settime`/`timer_delete`/`timer_gettime` syscalls

### Reference
https://github.com/asterinas/asterinas/issues/1189

### Describe the bug
There is a index out of bounds bug in `find_posix_timer()` at kernel/aster-nix/src/process/process/timer_manager.rs:190 when make a `timer_settime` syscall with specific arguments.
https://github.com/asterinas/asterinas/blob/0291b5dc6bb142b9c6165c1cb29b7658eefdaa63/kernel/aster-nix/src/process/process/timer_manager.rs#L189-L191

1. Compile a program which calls `timer_settime` with specific arguments:
```C
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>

int main() {
    char* ptr1 = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    int result = timer_settime(206, 0x1, ptr1, ptr1+1);

    return 0;
}
```
2. Run the compiled program in Asterinas.

### Expected behavior
Asterinas reports panic and is terminated.

### Environment

- Official docker asterinas/asterinas:0.6.2
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 0291b5dc

### Logs
```
~ # /root/timer_set.c 
panicked at /root/asterinas/kernel/aster-nix/src/process/process/timer_manager.rs:190:33:
index out of bounds: the len is 0 but the index is 206
Printing stack trace:
   1: fn 0xffffffff887cced0 - pc 0xffffffff887ccee8 / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4aa330;

   2: fn 0xffffffff887cccb0 - pc 0xffffffff887cce28 / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4aa340;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4aa4c0;

   4: fn 0xffffffff8895bd90 - pc 0xffffffff8895be12 / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4aa4d0;

   5: fn 0xffffffff8895c0b0 - pc 0xffffffff8895c1c8 / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4aa560;

   6: fn 0xffffffff884a5a70 - pc 0xffffffff884a5ac4 / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4aa650;

   7: fn 0xffffffff884b8310 - pc 0xffffffff884b8389 / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4aa690;

   8: fn 0xffffffff88444ef0 - pc 0xffffffff88444f54 / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4aa700;

   9: fn 0xffffffff88090470 - pc 0xffffffff880908cb / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4aa750;

  10: fn 0xffffffff88136b80 - pc 0xffffffff8815f8fe / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4aab40;

  11: fn 0xffffffff88121f60 - pc 0xffffffff88121fee / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4c03d0;

  12: fn 0xffffffff88515020 - pc 0xffffffff88515b9f / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4c0570;

  13: fn 0xffffffff884522a0 - pc 0xffffffff884522ae / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4c0f90;

  14: fn 0xffffffff887a72f0 - pc 0xffffffff887a7306 / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4c0fb0;

  15: fn 0xffffffff88843bc0 - pc 0xffffffff88843c29 / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4c0fd0;

  16: fn                0x0 - pc                0x0 / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f4c1000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:107
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) 86161ztg45fpd9b1tjbn4gsl1:?
(  4) ??:?
(  5) ??:?
(  6) ??:?
(  7) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/slice/index.rs:16
(  8) /root/asterinas/kernel/aster-nix/src/process/process/timer_manager.rs:190
(  9) /root/asterinas/kernel/aster-nix/src/syscall/timer_settime.rs:27
( 10) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:173
( 11) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:323
( 12) /root/asterinas/kernel/aster-nix/src/thread/task.rs:69
( 13) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 14) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 15) /root/asterinas/ostd/src/task/task/mod.rs:310
make: *** [Makefile:153: run] Error 1
```


# syscall-fstat-0291b5dc-FileLike::metadata-panic
Reachable panic in `FileLike::metadata()`

### Reference
https://github.com/asterinas/asterinas/issues/1193

### Describe the bug
There is a reachable panic in `FileLike::metadata()` at kernel/aster-nix/src/fs/file_handle.rs:58 when make a `fstat` syscall.

https://github.com/asterinas/asterinas/blob/0291b5dc6bb142b9c6165c1cb29b7658eefdaa63/kernel/aster-nix/src/fs/file_handle.rs#L57-L59


### To Reproduce
1. Compile a program which calls `fstat`:
```C
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>

int main() {
  void *addr = mmap(NULL, 4096, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  int fd = socket(1, 0x5, 0);
  fstat(fd, addr);
  return EXIT_SUCCESS;
}

```
2. Run the compiled program in Asterinas.

### Expected behavior
Asterinas reports panic and is terminated.

### Environment
- Official docker asterinas/asterinas:0.6.2
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 0291b5dc

### Logs
```
~ # /root/fstat.c 
panicked at /root/asterinas/kernel/aster-nix/src/fs/file_handle.rs:58:9:
metadata unsupported
Printing stack trace:
   1: fn 0xffffffff887cced0 - pc 0xffffffff887ccee8 / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f629e80;

   2: fn 0xffffffff887cccb0 - pc 0xffffffff887cce28 / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f629e90;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f62a010;

   4: fn 0xffffffff8895bd90 - pc 0xffffffff8895be12 / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f62a020;

   5: fn 0xffffffff88134bd0 - pc 0xffffffff88134c00 / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f62a0b0;

   6: fn 0xffffffff885c09f0 - pc 0xffffffff885c0d33 / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f62a100;

   7: fn 0xffffffff885c0fb0 - pc 0xffffffff885c1ccf / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f62a460;

   8: fn 0xffffffff88136b80 - pc 0xffffffff88164718 / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f62ab40;

   9: fn 0xffffffff88121f60 - pc 0xffffffff88121fee / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f6403d0;

  10: fn 0xffffffff88515020 - pc 0xffffffff88515b9f / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f640570;

  11: fn 0xffffffff884522a0 - pc 0xffffffff884522ae / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f640f90;

  12: fn 0xffffffff887a72f0 - pc 0xffffffff887a7306 / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f640fb0;

  13: fn 0xffffffff88843bc0 - pc 0xffffffff88843c29 / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f640fd0;

  14: fn                0x0 - pc                0x0 / registers:

     rax               0x12; rdx 0xffffffff889e5558; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f641000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:107
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) 86161ztg45fpd9b1tjbn4gsl1:?
(  4) ??:?
(  5) /root/asterinas/kernel/aster-nix/src/fs/file_handle.rs:61
(  6) /root/asterinas/kernel/aster-nix/src/syscall/stat.rs:20
(  7) /root/asterinas/kernel/aster-nix/src/syscall/stat.rs:60
(  8) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:173
(  9) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:323
( 10) /root/asterinas/kernel/aster-nix/src/thread/task.rs:69
( 11) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 12) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 13) /root/asterinas/ostd/src/task/task/mod.rs:310
make: *** [Makefile:153: run] Error 1
```

# syscall-recvfrom-ce2af1eb-recvmsg-panic
Reachable panic in `recvmsg()` of `recvfrom`/`sendmsg`/`sendto` syscalls

### Reference

### Describe the bug
There is a reachable panic in `recvmsg()` at kernel/aster-nix/src/net/socket/unix/stream/socket.rs:318 when make a `recvfrom` syscall with specific arguments.

https://github.com/asterinas/asterinas/blob/c6d381bc191a960b194c9da851e8f04b98b66979/kernel/aster-nix/src/net/socket/unix/stream/socket.rs#L318


### To Reproduce
1. Compile a program which calls `recvfrom`:
```C
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <arpa/inet.h>

int main() {
    int fd = socket(1, 0x5, 0);
    char ubuf[8];
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    recvfrom(fd, ubuf, 0, 0x4, (struct sockaddr *)&addr, &addr_len);

    return 0;
}
```
2. Run the compiled program in Asterinas.

### Expected behavior
Asterinas reports panic and is terminated.

### Environment
- Official docker asterinas/asterinas:0.6.2
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main ce2af1eb

### Logs
```
~ # /root/recvfrom.c
panicked at /root/asterinas/kernel/aster-nix/src/net/socket/unix/stream/socket.rs:318:9:
assertion failed: flags.is_all_supported()
Printing stack trace:
   1: fn 0xffffffff88869570 - pc 0xffffffff88869588 / registers:

     rax               0x12; rdx 0xffffffff88a385e0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f629650;

   2: fn 0xffffffff88869350 - pc 0xffffffff888694c8 / registers:

     rax               0x12; rdx 0xffffffff88a385e0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f629660;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax               0x12; rdx 0xffffffff88a385e0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f6297e0;

   4: fn 0xffffffff889a8650 - pc 0xffffffff889a86d2 / registers:

     rax               0x12; rdx 0xffffffff88a385e0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f6297f0;

   5: fn 0xffffffff889a87f0 - pc 0xffffffff889a8830 / registers:

     rax               0x12; rdx 0xffffffff88a385e0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f629880;

   6: fn 0xffffffff882d2520 - pc 0xffffffff882d258b / registers:

     rax               0x12; rdx 0xffffffff88a385e0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f6298f0;

   7: fn 0xffffffff8813ffe0 - pc 0xffffffff88140595 / registers:

     rax               0x12; rdx 0xffffffff88a385e0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f629a90;

   8: fn 0xffffffff88579fa0 - pc 0xffffffff885882df / registers:

     rax               0x12; rdx 0xffffffff88a385e0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f629f30;

   9: fn 0xffffffff8821e920 - pc 0xffffffff8821e9ae / registers:

     rax               0x12; rdx 0xffffffff88a385e0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f6403d0;

  10: fn 0xffffffff881d88f0 - pc 0xffffffff881d946f / registers:

     rax               0x12; rdx 0xffffffff88a385e0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f640570;

  11: fn 0xffffffff88095d80 - pc 0xffffffff88095d8e / registers:

     rax               0x12; rdx 0xffffffff88a385e0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f640f90;

  12: fn 0xffffffff887c0730 - pc 0xffffffff887c0746 / registers:

     rax               0x12; rdx 0xffffffff88a385e0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f640fb0;

  13: fn 0xffffffff887a61a0 - pc 0xffffffff887a6209 / registers:

     rax               0x12; rdx 0xffffffff88a385e0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f640fd0;

  14: fn                0x0 - pc                0x0 / registers:

     rax               0x12; rdx 0xffffffff88a385e0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f641000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:107
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) 0yfjuqijmdltwb3g9ceg2tyxs:?
(  4) ??:?
(  5) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/panicking.rs:220
(  6) ??:?
(  7) /root/asterinas/kernel/aster-nix/src/syscall/recvfrom.rs:29
(  8) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:220
(  9) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:328
( 10) /root/asterinas/kernel/aster-nix/src/thread/task.rs:69
( 11) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 12) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 13) /root/asterinas/ostd/src/task/task/mod.rs:310
make: *** [Makefile:153: run] Error 1
```

# syscall-munmap-ce2af1eb-do_exit-unwrap
Reachable unwrap panic in `do_exit()`

### Reference
https://github.com/asterinas/asterinas/issues/1194

### Describe the bug
There is a reachable unwrap panic in `do_exit()` at kernel/aster-nix/src/process/posix_thread/exit.rs:32 when make a `munmap` syscall with specific arguments.

https://github.com/asterinas/asterinas/blob/ce2af1eb057077753a7a757edc1833e677a83918/kernel/aster-nix/src/process/posix_thread/exit.rs#L27-L34

The address `0x10001690` is the `clear_child_tid` of a POSIX thread, and it together with the pointed memory will be set as 0 when exit.
However, when user calls syscall `munmap` for invalid address (i.e., 0x15000 - 0x100014000) in the example code, it will fail and fall into the exit process. Meanwhile the corresponding memory region is already unmapped, and the exit process will access this memory region and trigger this bug.


Before munmap, the memory info:
```
Mapped { va: 0x10001000, frame: Frame { page: Page { ptr: 0xfffffe00007e5700, _marker: PhantomData<ostd::mm::page::meta::FrameMeta> } }, prop: PageProperty { flags: R | W | RW | ACCESSED | DIRTY, cache: Writeback, priv_flags: USER } }
```

After munmap, the memory info:
```
NotMapped { va: 0x10001000, len: 0x1000 }
```

### To Reproduce
1. Compile a program which calls `munmap`:
```C
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
  void *addr = mmap(NULL, 4096, 0x3, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  munmap(addr, 0xfffff000);

  return EXIT_SUCCESS;
}
```
2. Run the compiled program in Asterinas.

### Expected behavior
Asterinas reports panic and is terminated.

### Environment
- Official docker asterinas/asterinas:0.6.2
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main ce2af1eb

### Logs
```
~ # /root/munmap.c
[    10.404] ERROR: page fault handler failed: addr: 0x447e1b, err: Error { errno: EACCES, msg: Some("page fault addr is not in current vmar") }
[    10.405] ERROR: page fault handler failed: addr: 0x10001690, err: Error { errno: EACCES, msg: Some("page fault addr is not in current vmar") }
panicked at /root/asterinas/kernel/aster-nix/src/process/posix_thread/exit.rs:32:14:
called `Result::unwrap()` on an `Err` value: Error { errno: EFAULT, msg: None }
Printing stack trace:
   1: fn 0xffffffff8876b810 - pc 0xffffffff8876b828 / registers:

     rax               0x12; rdx 0xffffffff889fd928; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000010bf160;

   2: fn 0xffffffff8876b5f0 - pc 0xffffffff8876b768 / registers:

     rax               0x12; rdx 0xffffffff889fd928; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000010bf170;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax               0x12; rdx 0xffffffff889fd928; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000010bf2f0;

   4: fn 0xffffffff8897edf0 - pc 0xffffffff8897ee72 / registers:

     rax               0x12; rdx 0xffffffff889fd928; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000010bf300;

   5: fn 0xffffffff8895c0d0 - pc 0xffffffff8895c1fb / registers:

     rax               0x12; rdx 0xffffffff889fd928; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000010bf390;

   6: fn 0xffffffff88117a90 - pc 0xffffffff88117ec6 / registers:

     rax               0x12; rdx 0xffffffff889fd928; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000010bf490;

   7: fn 0xffffffff882af3a0 - pc 0xffffffff882af840 / registers:

     rax               0x12; rdx 0xffffffff889fd928; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000010bf6e0;

   8: fn 0xffffffff883196c0 - pc 0xffffffff8831a266 / registers:

     rax               0x12; rdx 0xffffffff889fd928; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000010bfcf0;

   9: fn 0xffffffff88297270 - pc 0xffffffff88297e87 / registers:

     rax               0x12; rdx 0xffffffff889fd928; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000010c0570;

  10: fn 0xffffffff885a9820 - pc 0xffffffff885a982e / registers:

     rax               0x12; rdx 0xffffffff889fd928; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000010c0f90;

  11: fn 0xffffffff88781b60 - pc 0xffffffff88781b76 / registers:

     rax               0x12; rdx 0xffffffff889fd928; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000010c0fb0;

  12: fn 0xffffffff887b9710 - pc 0xffffffff887b9779 / registers:

     rax               0x12; rdx 0xffffffff889fd928; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000010c0fd0;

  13: fn                0x0 - pc                0x0 / registers:

     rax               0x12; rdx 0xffffffff889fd928; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000010c1000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:107
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) 2aghao2n2kcoquuybdyjuveav:?
(  4) ??:?
(  5) ??:?
(  6) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/result.rs:1102
(  7) /root/asterinas/kernel/aster-nix/src/process/exit.rs:23
(  8) /root/asterinas/kernel/aster-nix/src/process/signal/mod.rs:108
(  9) /root/asterinas/kernel/aster-nix/src/thread/task.rs:76
( 10) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 11) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 12) /root/asterinas/ostd/src/task/task/mod.rs:310
make: *** [Makefile:153: run] Error 1
```

# syscall-waitid-ce2af1eb-ProcessFilter::from_which_and_id-unwrap
Reachable unwrap panic in `ProcessFilter::from_which_and_id()` caused by invalid `which`

### Reference
https://github.com/asterinas/asterinas/issues/1197

### Describe the bug
There is a reachable unwrap panic in `ProcessFilter::from_which_and_id()` at kernel/aster-nix/src/process/process_filter.rs:24 when make a `waitid` syscall with specific arguments.

https://github.com/asterinas/asterinas/blob/ce2af1eb057077753a7a757edc1833e677a83918/kernel/aster-nix/src/process/process_filter.rs#L17-L26


### To Reproduce
1. Compile a program which calls `waitid`:
```C
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
  waitid(0x31142aad0bc93209, 15, 0x1, -10);

  return 0;
}

```
2. Run the compiled program in Asterinas.

### Expected behavior
Asterinas reports panic and is terminated.

### Environment
- Official docker asterinas/asterinas:0.6.2
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main ce2af1eb

### Logs
```
~ # /root/waitpid.c
panicked at /root/asterinas/kernel/aster-nix/src/process/process_filter.rs:24:18:
Unknown id type
Printing stack trace:
   1: fn 0xffffffff8876b840 - pc 0xffffffff8876b858 / registers:

     rax               0x12; rdx 0xffffffff889fd930; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80000122a370;

   2: fn 0xffffffff8876b620 - pc 0xffffffff8876b798 / registers:

     rax               0x12; rdx 0xffffffff889fd930; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80000122a380;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax               0x12; rdx 0xffffffff889fd930; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80000122a500;

   4: fn 0xffffffff8897ee20 - pc 0xffffffff8897eea2 / registers:

     rax               0x12; rdx 0xffffffff889fd930; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80000122a510;

   5: fn 0xffffffff88163050 - pc 0xffffffff881630b0 / registers:

     rax               0x12; rdx 0xffffffff889fd930; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80000122a5a0;

   6: fn 0xffffffff8822cc20 - pc 0xffffffff8822cc8d / registers:

     rax               0x12; rdx 0xffffffff889fd930; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80000122a610;

   7: fn 0xffffffff88292390 - pc 0xffffffff882bebda / registers:

     rax               0x12; rdx 0xffffffff889fd930; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80000122a730;

   8: fn 0xffffffff88291f70 - pc 0xffffffff88291ffe / registers:

     rax               0x12; rdx 0xffffffff889fd930; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000012403d0;

   9: fn 0xffffffff8836de80 - pc 0xffffffff8836e9ff / registers:

     rax               0x12; rdx 0xffffffff889fd930; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff800001240570;

  10: fn 0xffffffff885a49f0 - pc 0xffffffff885a49fe / registers:

     rax               0x12; rdx 0xffffffff889fd930; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff800001240f90;

  11: fn 0xffffffff88781b90 - pc 0xffffffff88781ba6 / registers:

     rax               0x12; rdx 0xffffffff889fd930; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff800001240fb0;

  12: fn 0xffffffff887b9740 - pc 0xffffffff887b97a9 / registers:

     rax               0x12; rdx 0xffffffff889fd930; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff800001240fd0;

  13: fn                0x0 - pc                0x0 / registers:

     rax               0x12; rdx 0xffffffff889fd930; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff800001241000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:107
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) 2aghao2n2kcoquuybdyjuveav:?
(  4) ??:?
(  5) /root/asterinas/kernel/aster-nix/src/process/process_filter.rs:21
(  6) /root/asterinas/kernel/aster-nix/src/syscall/waitid.rs:18
(  7) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:195
(  8) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:325
(  9) /root/asterinas/kernel/aster-nix/src/thread/task.rs:69
( 10) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 11) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 12) /root/asterinas/ostd/src/task/task/mod.rs:310
make: *** [Makefile:153: run] Error 1
```

# syscall-timer_create-ce2af1e-sys_timer_create-unwrap
Reachable unwrap panic in `sys_timer_create()`

### Reference
https://github.com/asterinas/asterinas/issues/1199

### Describe the bug
There is a reachable unwrap panic in `sys_timer_create()` at kernel/aster-nix/src/process/process_filter.rs:24 when make a `timer_create` syscall with specific arguments.

https://github.com/asterinas/asterinas/blob/0160a85ccd130c222ae9b8712b4102c726cba35c/kernel/aster-nix/src/syscall/timer_create.rs#L137


### To Reproduce
1. Compile a program which calls `timer_create`:
```C
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>

int main() {
  void *addr = mmap(NULL, 4096, 0x3, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  timer_create(-3, 0x0, addr);

  return 0;
}
```
2. Run the compiled program in Asterinas.

### Expected behavior
Asterinas reports panic and is terminated.

### Environment
- Official docker asterinas/asterinas:0.6.2
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main ce2af1e

### Logs
```
~ # /root/timer_create.c
panicked at /root/asterinas/kernel/aster-nix/src/syscall/timer_create.rs:137:61:
called `Option::unwrap()` on a `None` value
Printing stack trace:
   1: fn 0xffffffff8878fa60 - pc 0xffffffff8878fa78 / registers:

     rax               0x12; rdx 0xffffffff88a22b28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000013293f0;

   2: fn 0xffffffff8878f840 - pc 0xffffffff8878f9b8 / registers:

     rax               0x12; rdx 0xffffffff88a22b28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff800001329400;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax               0x12; rdx 0xffffffff88a22b28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff800001329580;

   4: fn 0xffffffff889a3060 - pc 0xffffffff889a30e2 / registers:

     rax               0x12; rdx 0xffffffff88a22b28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff800001329590;

   5: fn 0xffffffff889a3200 - pc 0xffffffff889a3240 / registers:

     rax               0x12; rdx 0xffffffff88a22b28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff800001329620;

   6: fn 0xffffffff8898c7e0 - pc 0xffffffff8898c7f5 / registers:

     rax               0x12; rdx 0xffffffff88a22b28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff800001329690;

   7: fn 0xffffffff8829e4b0 - pc 0xffffffff8829f859 / registers:

     rax               0x12; rdx 0xffffffff88a22b28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000013296a0;

   8: fn 0xffffffff883a1240 - pc 0xffffffff883cb92f / registers:

     rax               0x12; rdx 0xffffffff88a22b28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff800001329f30;

   9: fn 0xffffffff883a0f60 - pc 0xffffffff883a0fee / registers:

     rax               0x12; rdx 0xffffffff88a22b28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000013403d0;

  10: fn 0xffffffff8835ac20 - pc 0xffffffff8835b79f / registers:

     rax               0x12; rdx 0xffffffff88a22b28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff800001340570;

  11: fn 0xffffffff885caef0 - pc 0xffffffff885caefe / registers:

     rax               0x12; rdx 0xffffffff88a22b28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff800001340f90;

  12: fn 0xffffffff887a5db0 - pc 0xffffffff887a5dc6 / registers:

     rax               0x12; rdx 0xffffffff88a22b28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff800001340fb0;

  13: fn 0xffffffff887dd960 - pc 0xffffffff887dd9c9 / registers:

     rax               0x12; rdx 0xffffffff88a22b28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff800001340fd0;

  14: fn                0x0 - pc                0x0 / registers:

     rax               0x12; rdx 0xffffffff88a22b28; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff800001341000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:107
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) 2aghao2n2kcoquuybdyjuveav:?
(  4) ??:?
(  5) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/panicking.rs:220
(  6) ??:?
(  7) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/option.rs:961
(  8) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:171
(  9) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:328
( 10) /root/asterinas/kernel/aster-nix/src/thread/task.rs:69
( 11) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 12) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 13) /root/asterinas/ostd/src/task/task/mod.rs:310
make: *** [Makefile:153: run] Error 1
```

# syscall-semget-0160a85c-check_sem-unwrap
Reachable unwrap panic in `check_sem()` by invalid `key` value

### Reference
https://github.com/asterinas/asterinas/issues/1200

### Describe the bug
There is a reachable unwrap panic in `check_sem()` at kernel/aster-nix/src/ipc/semaphore/system_v/sem_set.rs:131 when make a `semget` syscall with specific arguments.

https://github.com/asterinas/asterinas/blob/0160a85ccd130c222ae9b8712b4102c726cba35c/kernel/aster-nix/src/ipc/semaphore/system_v/sem_set.rs#L131


### To Reproduce
1. Compile a program which calls `semget`:
```C
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

int main() {
    semget(-7, 32, 0x69a5d000);

    return 0;
}

```
2. Run the compiled program in Asterinas.

### Expected behavior
Asterinas reports panic and is terminated.

### Environment
- Official docker asterinas/asterinas:0.6.2
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 0160a85c

### Logs
```
~ # /root/semget.c
panicked at /root/asterinas/kernel/aster-nix/src/ipc/semaphore/system_v/sem_set.rs:131:5:
assertion failed: id > 0
Printing stack trace:
   1: fn 0xffffffff8878f9f0 - pc 0xffffffff8878fa08 / registers:

     rax               0x12; rdx 0xffffffff88a22a48; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014a9730;

   2: fn 0xffffffff8878f7d0 - pc 0xffffffff8878f948 / registers:

     rax               0x12; rdx 0xffffffff88a22a48; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014a9740;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax               0x12; rdx 0xffffffff88a22a48; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014a98c0;

   4: fn 0xffffffff889a2ff0 - pc 0xffffffff889a3072 / registers:

     rax               0x12; rdx 0xffffffff88a22a48; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014a98d0;

   5: fn 0xffffffff889a3190 - pc 0xffffffff889a31d0 / registers:

     rax               0x12; rdx 0xffffffff88a22a48; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014a9960;

   6: fn 0xffffffff880ac870 - pc 0xffffffff880ac8b7 / registers:

     rax               0x12; rdx 0xffffffff88a22a48; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014a99d0;

   7: fn 0xffffffff88369810 - pc 0xffffffff88369c1b / registers:

     rax               0x12; rdx 0xffffffff88a22a48; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014a9b90;

   8: fn 0xffffffff8856efd0 - pc 0xffffffff88582ae7 / registers:

     rax               0x12; rdx 0xffffffff88a22a48; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014a9f30;

   9: fn 0xffffffff8856ecf0 - pc 0xffffffff8856ed7e / registers:

     rax               0x12; rdx 0xffffffff88a22a48; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014c03d0;

  10: fn 0xffffffff8810ab70 - pc 0xffffffff8810b6ef / registers:

     rax               0x12; rdx 0xffffffff88a22a48; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014c0570;

  11: fn 0xffffffff885cc480 - pc 0xffffffff885cc48e / registers:

     rax               0x12; rdx 0xffffffff88a22a48; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014c0f90;

  12: fn 0xffffffff887a5d40 - pc 0xffffffff887a5d56 / registers:

     rax               0x12; rdx 0xffffffff88a22a48; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014c0fb0;

  13: fn 0xffffffff887dd8f0 - pc 0xffffffff887dd959 / registers:

     rax               0x12; rdx 0xffffffff88a22a48; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014c0fd0;

  14: fn                0x0 - pc                0x0 / registers:

     rax               0x12; rdx 0xffffffff88a22a48; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014c1000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:107
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) 2aghao2n2kcoquuybdyjuveav:?
(  4) ??:?
(  5) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/panicking.rs:220
(  6) /root/asterinas/kernel/aster-nix/src/ipc/semaphore/system_v/sem_set.rs:133
(  7) /root/asterinas/kernel/aster-nix/src/syscall/semget.rs:42
(  8) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:171
(  9) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:328
( 10) /root/asterinas/kernel/aster-nix/src/thread/task.rs:69
( 11) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 12) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 13) /root/asterinas/ostd/src/task/task/mod.rs:310
make: *** [Makefile:153: run] Error 1
```


# syscall-semget-0160a85c-create_sem_set_with_id-outofbounds
Out of bounds in `create_sem_set_with_id()` caused by invalid `key`

### Reference
https://github.com/asterinas/asterinas/issues/1201

### Describe the bug
There is a out of bounds error in `create_sem_set_with_id()` at kernel/aster-nix/src/ipc/semaphore/system_v/sem_set.rs:116 when make a `semget` syscall with specific arguments.

https://github.com/asterinas/asterinas/blob/0160a85ccd130c222ae9b8712b4102c726cba35c/kernel/aster-nix/src/ipc/semaphore/system_v/sem_set.rs#L116-L121

The out of bounds finally happens in `IdAlloc::alloc_specific()` at ostd/libs/id-alloc/src/lib.rs:160

https://github.com/asterinas/asterinas/blob/2102107be18729fba8f3546d5a3fe39c819d814a/ostd/libs/id-alloc/src/lib.rs#L156-L162

### To Reproduce
1. Compile a program which calls `semget`:
```C
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

int main() {
    semget(0x2209e1130c, 179, 0xaed34fff);

    return 0;
}

```
2. Run the compiled program in Asterinas.

### Expected behavior
Asterinas reports panic and is terminated.

### Environment
- Official docker asterinas/asterinas:0.6.2
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 0160a85c

### Logs
```
~ # /root/semget.c
panicked at /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/bitvec-1.0.1/src/slice/api.rs:2594:13:
index 165745420 out of bounds: 32001
Printing stack trace:
   1: fn 0xffffffff8878fae0 - pc 0xffffffff8878faf8 / registers:

     rax               0x12; rdx 0xffffffff88a22b80; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014a9410;

   2: fn 0xffffffff8878f8c0 - pc 0xffffffff8878fa38 / registers:

     rax               0x12; rdx 0xffffffff88a22b80; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014a9420;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax               0x12; rdx 0xffffffff88a22b80; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014a95a0;

   4: fn 0xffffffff889a30e0 - pc 0xffffffff889a3162 / registers:

     rax               0x12; rdx 0xffffffff88a22b80; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014a95b0;

   5: fn 0xffffffff8893ea70 - pc 0xffffffff8893eb7f / registers:

     rax               0x12; rdx 0xffffffff88a22b80; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014a9640;

   6: fn 0xffffffff8893f6a0 - pc 0xffffffff8893f70a / registers:

     rax               0x12; rdx 0xffffffff88a22b80; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014a9720;

   7: fn 0xffffffff8893e9f0 - pc 0xffffffff8893ea5f / registers:

     rax               0x12; rdx 0xffffffff88a22b80; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014a9780;

   8: fn 0xffffffff8893f410 - pc 0xffffffff8893f442 / registers:

     rax               0x12; rdx 0xffffffff88a22b80; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014a97f0;

   9: fn 0xffffffff88939d40 - pc 0xffffffff88939daa / registers:

     rax               0x12; rdx 0xffffffff88a22b80; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014a9860;

  10: fn 0xffffffff889422f0 - pc 0xffffffff88942314 / registers:

     rax               0x12; rdx 0xffffffff88a22b80; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014a98c0;

  11: fn 0xffffffff883688f0 - pc 0xffffffff88368a79 / registers:

     rax               0x12; rdx 0xffffffff88a22b80; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014a9920;

  12: fn 0xffffffff88444f80 - pc 0xffffffff884455c3 / registers:

     rax               0x12; rdx 0xffffffff88a22b80; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014a9b90;

  13: fn 0xffffffff883a4760 - pc 0xffffffff883b8277 / registers:

     rax               0x12; rdx 0xffffffff88a22b80; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014a9f30;

  14: fn 0xffffffff883a4480 - pc 0xffffffff883a450e / registers:

     rax               0x12; rdx 0xffffffff88a22b80; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014c03d0;

  15: fn 0xffffffff8835e140 - pc 0xffffffff8835ecbf / registers:

     rax               0x12; rdx 0xffffffff88a22b80; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014c0570;

  16: fn 0xffffffff885caf70 - pc 0xffffffff885caf7e / registers:

     rax               0x12; rdx 0xffffffff88a22b80; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014c0f90;

  17: fn 0xffffffff887a5e30 - pc 0xffffffff887a5e46 / registers:

     rax               0x12; rdx 0xffffffff88a22b80; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014c0fb0;

  18: fn 0xffffffff887dd9e0 - pc 0xffffffff887dda49 / registers:

     rax               0x12; rdx 0xffffffff88a22b80; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014c0fd0;

  19: fn                0x0 - pc                0x0 / registers:

     rax               0x12; rdx 0xffffffff88a22b80; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000014c1000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:107
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) 2aghao2n2kcoquuybdyjuveav:?
(  4) ??:?
(  5) ??:?
(  6) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/option.rs:1006
(  7) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/bitvec-1.0.1/src/slice/api.rs:2593
(  8) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/bitvec-1.0.1/src/slice/ops.rs:159
(  9) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/bitvec-1.0.1/src/vec/ops.rs:237
( 10) /root/asterinas/ostd/libs/id-alloc/src/lib.rs:160
( 11) /root/asterinas/kernel/aster-nix/src/ipc/semaphore/system_v/sem_set.rs:116
( 12) /root/asterinas/kernel/aster-nix/src/syscall/semget.rs:61
( 13) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:171
( 14) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:328
( 15) /root/asterinas/kernel/aster-nix/src/thread/task.rs:69
( 16) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 17) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 18) /root/asterinas/ostd/src/task/task/mod.rs:310
make: *** [Makefile:153: run] Error 1
```

# syscall-mmap-0160a85c-sys_mmap-unwrap
Reachable unwrap panic in `sys_mmap()`

### Reference
https://github.com/asterinas/asterinas/issues/1203

### Describe the bug
There is a out of bounds error in `sys_mmap()` at kernel/aster-nix/src/syscall/mmap.rs:27 when make a `mmap` syscall with specific arguments.

https://github.com/asterinas/asterinas/blob/0160a85ccd130c222ae9b8712b4102c726cba35c/kernel/aster-nix/src/syscall/mmap.rs#L27


### To Reproduce
1. Compile a program which calls `mmap`:
```C
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

int main(void)
{
  mmap(NULL, 0, 0xb, 0x4f142, -1, 0);
  return 0;
}
```
2. Run the compiled program in Asterinas.

### Expected behavior
Asterinas reports panic and is terminated.

### Environment
- Official docker asterinas/asterinas:0.6.2
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 0160a85c

### Logs
```
~ # /root/mmap_test.c
panicked at /root/asterinas/kernel/aster-nix/src/syscall/mmap.rs:27:61:
called `Option::unwrap()` on a `None` value
Printing stack trace:
   1: fn 0xffffffff8878fa40 - pc 0xffffffff8878fa58 / registers:

     rax               0x12; rdx 0xffffffff88a22ad8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000015a9af0;

   2: fn 0xffffffff8878f820 - pc 0xffffffff8878f998 / registers:

     rax               0x12; rdx 0xffffffff88a22ad8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000015a9b00;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax               0x12; rdx 0xffffffff88a22ad8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000015a9c80;

   4: fn 0xffffffff889a3040 - pc 0xffffffff889a30c2 / registers:

     rax               0x12; rdx 0xffffffff88a22ad8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000015a9c90;

   5: fn 0xffffffff889a31e0 - pc 0xffffffff889a3220 / registers:

     rax               0x12; rdx 0xffffffff88a22ad8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000015a9d20;

   6: fn 0xffffffff8898c7c0 - pc 0xffffffff8898c7d5 / registers:

     rax               0x12; rdx 0xffffffff88a22ad8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000015a9d90;

   7: fn 0xffffffff88136280 - pc 0xffffffff88136315 / registers:

     rax               0x12; rdx 0xffffffff88a22ad8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000015a9da0;

   8: fn 0xffffffff8839d1f0 - pc 0xffffffff883a1e5c / registers:

     rax               0x12; rdx 0xffffffff88a22ad8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000015a9f30;

   9: fn 0xffffffff882b42e0 - pc 0xffffffff882b436e / registers:

     rax               0x12; rdx 0xffffffff88a22ad8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000015c03d0;

  10: fn 0xffffffff88190fd0 - pc 0xffffffff88191b4f / registers:

     rax               0x12; rdx 0xffffffff88a22ad8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000015c0570;

  11: fn 0xffffffff885cbd10 - pc 0xffffffff885cbd1e / registers:

     rax               0x12; rdx 0xffffffff88a22ad8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000015c0f90;

  12: fn 0xffffffff887a5d90 - pc 0xffffffff887a5da6 / registers:

     rax               0x12; rdx 0xffffffff88a22ad8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000015c0fb0;

  13: fn 0xffffffff887dd940 - pc 0xffffffff887dd9a9 / registers:

     rax               0x12; rdx 0xffffffff88a22ad8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000015c0fd0;

  14: fn                0x0 - pc                0x0 / registers:

     rax               0x12; rdx 0xffffffff88a22ad8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff8000015c1000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:107
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) 2aghao2n2kcoquuybdyjuveav:?
(  4) ??:?
(  5) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/panicking.rs:220
(  6) ??:?
(  7) 9yd76ggpeli67ls0owdwumxu5:?
(  8) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:220
(  9) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:328
( 10) /root/asterinas/kernel/aster-nix/src/thread/task.rs:69
( 11) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 12) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 13) /root/asterinas/ostd/src/task/task/mod.rs:310
make: *** [Makefile:153: run] Error 1
```

# syscall-timer_delete-0291b5dc-remove_posix_timer-outofbounds
index out of bounds in `remove_posix_timer()` caused by invalid `timer_id` of `timer_delete` syscall

### Reference
https://github.com/asterinas/asterinas/pull/1211

### Describe the bug
There is a index out of bounds bug in `remove_posix_timer()` at kernel/aster-nix/src/process/process/timer_manager.rs:201 when make a `timer_delete` syscall with specific arguments.

https://github.com/asterinas/asterinas/blob/0291b5dc6bb142b9c6165c1cb29b7658eefdaa63/kernel/aster-nix/src/process/process/timer_manager.rs#L196

1. Compile a program which calls `timer_delete` with specific arguments:
```C
#include <errno.h>
#include <linux/rseq.h>
#include <stdio.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/timerfd.h>
#include <unistd.h>

int main() {
  timer_delete(0xd5d5d5d5);

  return 0;
}
```
2. Run the compiled program in Asterinas.

### Expected behavior
Asterinas reports panic and is terminated.

### Environment

- Official docker asterinas/asterinas:0.6.2
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 0291b5dc

### Logs
```
~ # /root/timer_delete.c 
panicked at /root/asterinas/kernel/aster-nix/src/process/process/timer_manager.rs:201:27:
index out of bounds: the len is 0 but the index is 3587560917
Printing stack trace:
   1: fn 0xffffffff888695b0 - pc 0xffffffff888695c8 / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4a9a10;

   2: fn 0xffffffff88869390 - pc 0xffffffff88869508 / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4a9a20;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4a9ba0;

   4: fn 0xffffffff889a8690 - pc 0xffffffff889a8712 / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4a9bb0;

   5: fn 0xffffffff889a89b0 - pc 0xffffffff889a8ac8 / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4a9c40;

   6: fn 0xffffffff883e6df0 - pc 0xffffffff883e6e44 / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4a9d30;

   7: fn 0xffffffff885d10d0 - pc 0xffffffff885d1149 / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4a9d70;

   8: fn 0xffffffff885bf190 - pc 0xffffffff885bf202 / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4a9de0;

   9: fn 0xffffffff88553e00 - pc 0xffffffff88553ec7 / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4a9e70;

  10: fn 0xffffffff8856ded0 - pc 0xffffffff88599450 / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4a9f30;

  11: fn 0xffffffff880df4c0 - pc 0xffffffff880df54e / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4c03d0;

  12: fn 0xffffffff882db7a0 - pc 0xffffffff882dc31f / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4c0570;

  13: fn 0xffffffff883ffc40 - pc 0xffffffff883ffc4e / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4c0f90;

  14: fn 0xffffffff887c0770 - pc 0xffffffff887c0786 / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4c0fb0;

  15: fn 0xffffffff887a61e0 - pc 0xffffffff887a6249 / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4c0fd0;

  16: fn                0x0 - pc                0x0 / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4c1000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:107
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) 0yfjuqijmdltwb3g9ceg2tyxs:?
(  4) ??:?
(  5) ??:?
(  6) ??:?
(  7) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/slice/index.rs:27
(  8) /root/asterinas/kernel/aster-nix/src/process/process/timer_manager.rs:201
(  9) /root/asterinas/kernel/aster-nix/src/syscall/timer_create.rs:155
( 10) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:157
( 11) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:328
( 12) /root/asterinas/kernel/aster-nix/src/thread/task.rs:69
( 13) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 14) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 15) /root/asterinas/ostd/src/task/task/mod.rs:310
make: *** [Makefile:153: run] Error 1
```

# syscall-mmap-562e6443-find_free_region-integeroverflow
Integer overflow in `find_free_region()` caused by large `size`

### Reference


### Describe the bug
There is a add with overflow error in `find_free_region()` at kernel/aster-nix/src/vm/vmar/mod.rs:167 when make a `mmap` syscall with large `size`.

https://github.com/asterinas/asterinas/blob/562e64437584279783f244edba10b512beddc81d/kernel/aster-nix/src/vm/vmar/mod.rs#L167


### To Reproduce
1. Compile a program which calls `mmap`:
```C
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
  void *addr = mmap(NULL, 0xfffffffffffff000, 0x3, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

  return 0;
}

```
2. Run the compiled program in Asterinas.

### Expected behavior
Asterinas reports `attempt to add with overflow` panic and is terminated.

### Environment
- Official docker asterinas/asterinas:0.6.2
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 562e6443

### Logs
```
~ # /root/munmap.c 
panicked at /root/asterinas/kernel/aster-nix/src/vm/vmar/mod.rs:167:43:
attempt to add with overflow
Printing stack trace:
   1: fn 0xffffffff888695b0 - pc 0xffffffff888695c8 / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4a7a70;

   2: fn 0xffffffff88869390 - pc 0xffffffff88869508 / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4a7a80;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4a7c00;

   4: fn 0xffffffff889a8690 - pc 0xffffffff889a8712 / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4a7c10;

   5: fn 0xffffffff88983e50 - pc 0xffffffff88983e7e / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4a7ca0;

   6: fn 0xffffffff88470450 - pc 0xffffffff884707f7 / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4a7cf0;

   7: fn 0xffffffff884742b0 - pc 0xffffffff88474c1a / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4a7f30;

   8: fn 0xffffffff8855aa00 - pc 0xffffffff8855ab86 / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4a8900;

   9: fn 0xffffffff8855b6d0 - pc 0xffffffff8855b824 / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4a8dd0;

  10: fn 0xffffffff8844e2f0 - pc 0xffffffff8844f4da / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4a90f0;

  11: fn 0xffffffff8844e000 - pc 0xffffffff8844e168 / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4a9da0;

  12: fn 0xffffffff8856ded0 - pc 0xffffffff88572b3c / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4a9f30;

  13: fn 0xffffffff880df4c0 - pc 0xffffffff880df54e / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4c03d0;

  14: fn 0xffffffff882db7a0 - pc 0xffffffff882dc31f / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4c0570;

  15: fn 0xffffffff883ffc40 - pc 0xffffffff883ffc4e / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4c0f90;

  16: fn 0xffffffff887c0770 - pc 0xffffffff887c0786 / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4c0fb0;

  17: fn 0xffffffff887a61e0 - pc 0xffffffff887a6249 / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4c0fd0;

  18: fn                0x0 - pc                0x0 / registers:

     rax               0x12; rdx 0xffffffff88a38470; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e4c1000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:107
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) 0yfjuqijmdltwb3g9ceg2tyxs:?
(  4) ??:?
(  5) ??:?
(  6) axbvs75vp1k66nwsaobv39t4g:?
(  7) /root/asterinas/kernel/aster-nix/src/vm/vmar/mod.rs:607
(  8) /root/asterinas/kernel/aster-nix/src/vm/vmar/vm_mapping.rs:102
(  9) /root/asterinas/kernel/aster-nix/src/vm/vmar/vm_mapping.rs:695
( 10) /root/asterinas/kernel/aster-nix/src/syscall/mmap.rs:137
( 11) /root/asterinas/kernel/aster-nix/src/syscall/mmap.rs:33
( 12) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:220
( 13) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:328
( 14) /root/asterinas/kernel/aster-nix/src/thread/task.rs:69
( 15) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 16) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 17) /root/asterinas/ostd/src/task/task/mod.rs:310
```

# syscall-munmap-562e6443-sys_munmap-integeroverflow
Integer overflow in `sys_munmap()` caused by large `size`

# syscall-mprotect-562e6443-sys_munmap-integeroverflow
Integer overflow in `sys_munmap()` caused by large `size`

# syscall-madvise-562e6443-sys_munmap-integeroverflow
Integer overflow in `sys_munmap()` caused by large `size`

# syscall-munmap-562e6443-align_up-unwrap
Reachable unwrap panic in `align_up` caused by large `size`

### Reference
https://github.com/asterinas/asterinas/issues/1214

### Describe the bug
There is a reachable unwrap panic in `align_up` at ostd/libs/align_ext/src/lib.rs:74 when make a `munmap` syscall with large `size` (i.e., `0xffffffffffffffff`).

https://github.com/asterinas/asterinas/blob/562e64437584279783f244edba10b512beddc81d/ostd/libs/align_ext/src/lib.rs#L61


### To Reproduce
1. Compile a program which calls `munmap`:
```C
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
  void *addr = mmap(NULL, 4096, 0x3, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  munmap(addr, 0xffffffffffffffff);

  return EXIT_SUCCESS;
}
```
2. Run the compiled program in Asterinas.

### Expected behavior
Asterinas reports panic and is terminated.

### Environment
- Official docker asterinas/asterinas:0.6.2
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 562e6443

### Logs

```
~ # /root/munmap.c 
panicked at /root/asterinas/ostd/libs/align_ext/src/lib.rs:74:1:
called `Option::unwrap()` on a `None` value
Printing stack trace:
   1: fn 0xffffffff88869620 - pc 0xffffffff88869638 / registers:

     rax               0x12; rdx 0xffffffff88a38508; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6a98f0;

   2: fn 0xffffffff88869400 - pc 0xffffffff88869578 / registers:

     rax               0x12; rdx 0xffffffff88a38508; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6a9900;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax               0x12; rdx 0xffffffff88a38508; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6a9a80;

   4: fn 0xffffffff889a8700 - pc 0xffffffff889a8782 / registers:

     rax               0x12; rdx 0xffffffff88a38508; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6a9a90;

   5: fn 0xffffffff889a88a0 - pc 0xffffffff889a88e0 / registers:

     rax               0x12; rdx 0xffffffff88a38508; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6a9b20;

   6: fn 0xffffffff88991e80 - pc 0xffffffff88991e95 / registers:

     rax               0x12; rdx 0xffffffff88a38508; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6a9b90;

   7: fn 0xffffffff885c02d0 - pc 0xffffffff885c03c0 / registers:

     rax               0x12; rdx 0xffffffff88a38508; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6a9ba0;

   8: fn 0xffffffff88515de0 - pc 0xffffffff88516038 / registers:

     rax               0x12; rdx 0xffffffff88a38508; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6a9c10;

   9: fn 0xffffffff88573420 - pc 0xffffffff88578a42 / registers:

     rax               0x12; rdx 0xffffffff88a38508; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6a9f30;

  10: fn 0xffffffff88563460 - pc 0xffffffff885634ee / registers:

     rax               0x12; rdx 0xffffffff88a38508; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6c03d0;

  11: fn 0xffffffff88445c30 - pc 0xffffffff884467af / registers:

     rax               0x12; rdx 0xffffffff88a38508; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6c0570;

  12: fn 0xffffffff88401f60 - pc 0xffffffff88401f6e / registers:

     rax               0x12; rdx 0xffffffff88a38508; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6c0f90;

  13: fn 0xffffffff887c07e0 - pc 0xffffffff887c07f6 / registers:

     rax               0x12; rdx 0xffffffff88a38508; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6c0fb0;

  14: fn 0xffffffff887a6250 - pc 0xffffffff887a62b9 / registers:

     rax               0x12; rdx 0xffffffff88a38508; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6c0fd0;

  15: fn                0x0 - pc                0x0 / registers:

     rax               0x12; rdx 0xffffffff88a38508; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007e6c1000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:107
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) 0yfjuqijmdltwb3g9ceg2tyxs:?
(  4) ??:?
(  5) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/panicking.rs:220
(  6) ??:?
(  7) dxo5803zce17no7m33fsuqpvr:?
(  8) /root/asterinas/kernel/aster-nix/src/syscall/munmap.rs:14
(  9) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:164
( 10) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:328
( 11) /root/asterinas/kernel/aster-nix/src/thread/task.rs:69
( 12) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 13) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 14) /root/asterinas/ostd/src/task/task/mod.rs:310
make: *** [Makefile:153: run] Error 1
```


# syscall-waitid-562e6443-sys_waitid-panic
Reachable panic in `sys_waitid()` caused by invalid `options`

### Reference
https://github.com/asterinas/asterinas/issues/1216

### Describe the bug
There is a reachable unwrap panic in `sys_waitid()` at kernel/aster-nix/src/syscall/waitid.rs:19 when make a `waitid` syscall with invalid `options` argument.

https://github.com/asterinas/asterinas/blob/562e64437584279783f244edba10b512beddc81d/kernel/aster-nix/src/syscall/waitid.rs#L19


### To Reproduce
1. Compile a program which calls `waitid`:
```C
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
  waitid(0, 15, 1024, 10474);

  return 0;
}
```
2. Run the compiled program in Asterinas.

### Expected behavior
Asterinas reports panic and is terminated.

### Environment
- Official docker asterinas/asterinas:0.6.2
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 562e6443

### Logs

```
~ # /root/waitpid.c 
waitid: Invalid argument
panicked at /root/asterinas/kernel/aster-nix/src/syscall/waitid.rs:19:63:
Unknown wait options
Printing stack trace:
   1: fn 0xffffffff888698a0 - pc 0xffffffff888698b8 / registers:

     rax               0x12; rdx 0xffffffff88a387f8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f029a90;

   2: fn 0xffffffff88869680 - pc 0xffffffff888697f8 / registers:

     rax               0x12; rdx 0xffffffff88a387f8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f029aa0;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax               0x12; rdx 0xffffffff88a387f8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f029c20;

   4: fn 0xffffffff889a8980 - pc 0xffffffff889a8a02 / registers:

     rax               0x12; rdx 0xffffffff88a387f8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f029c30;

   5: fn 0xffffffff889a8c10 - pc 0xffffffff889a8ca0 / registers:

     rax               0x12; rdx 0xffffffff88a387f8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f029cc0;

   6: fn 0xffffffff88992120 - pc 0xffffffff88992143 / registers:

     rax               0x12; rdx 0xffffffff88a387f8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f029d60;

   7: fn 0xffffffff880e7ae0 - pc 0xffffffff880e7b21 / registers:

     rax               0x12; rdx 0xffffffff88a387f8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f029d80;

   8: fn 0xffffffff882191a0 - pc 0xffffffff88219267 / registers:

     rax               0x12; rdx 0xffffffff88a387f8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f029dc0;

   9: fn 0xffffffff8856f780 - pc 0xffffffff8859d408 / registers:

     rax               0x12; rdx 0xffffffff88a387f8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f029f30;

  10: fn 0xffffffff88175a60 - pc 0xffffffff88175aee / registers:

     rax               0x12; rdx 0xffffffff88a387f8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f0403d0;

  11: fn 0xffffffff8848f2f0 - pc 0xffffffff8848fe6f / registers:

     rax               0x12; rdx 0xffffffff88a387f8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f040570;

  12: fn 0xffffffff88057950 - pc 0xffffffff8805795e / registers:

     rax               0x12; rdx 0xffffffff88a387f8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f040f90;

  13: fn 0xffffffff887c0a60 - pc 0xffffffff887c0a76 / registers:

     rax               0x12; rdx 0xffffffff88a387f8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f040fb0;

  14: fn 0xffffffff887a64d0 - pc 0xffffffff887a6539 / registers:

     rax               0x12; rdx 0xffffffff88a387f8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f040fd0;

  15: fn                0x0 - pc                0x0 / registers:

     rax               0x12; rdx 0xffffffff88a387f8; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f041000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:107
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) 0yfjuqijmdltwb3g9ceg2tyxs:?
(  4) ??:?
(  5) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/panicking.rs:271
(  6) ??:?
(  7) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/option.rs:923
(  8) /root/asterinas/kernel/aster-nix/src/syscall/waitid.rs:19
(  9) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:198
( 10) /root/asterinas/kernel/aster-nix/src/syscall/mod.rs:328
( 11) /root/asterinas/kernel/aster-nix/src/thread/task.rs:69
( 12) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 13) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 14) /root/asterinas/ostd/src/task/task/mod.rs:310
make: *** [Makefile:153: run] Error 1
```

# syscall-clock_gettime-aa77747f-read_clock-unwrap
Reachable unwrap panic in `read_clock()`

### Reference
https://github.com/asterinas/asterinas/issues/1244

### Describe the bug
There is a reachable unwrap panic in `read_clock()` at kernel/src/syscall/clock_gettime.rs:141 when make a `clock_gettime` syscall with specific argument.

https://github.com/asterinas/asterinas/blob/aa77747f94c4b1cb1237ba52414642827a6efc25/kernel/src/syscall/clock_gettime.rs#L141


### To Reproduce
1. Compile a program which calls `clock_gettime`:
```C
#include <errno.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>

int main() {
  clock_gettime(-10, 0x1);
  perror("clock_gettime");

  return 0;
}
```
2. Run the compiled program in Asterinas.

### Expected behavior
Asterinas reports panic and is terminated.

### Environment
- Official docker asterinas/asterinas:0.8.0
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main aa77747f

### Logs

```
~ # /root/clock_gettime.c 
panicked at /root/asterinas/kernel/src/syscall/clock_gettime.rs:141:61:
called `Option::unwrap()` on a `None` value
Printing stack trace:
   1: fn 0xffffffff8880e1c0 - pc 0xffffffff8880e1d8 / registers:

     rax               0x12; rdx 0xffffffff88a3a1a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f6297c0;

   2: fn 0xffffffff8880dfa0 - pc 0xffffffff8880e118 / registers:

     rax               0x12; rdx 0xffffffff88a3a1a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f6297d0;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax               0x12; rdx 0xffffffff88a3a1a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f629950;

   4: fn 0xffffffff889b0fb0 - pc 0xffffffff889b1032 / registers:

     rax               0x12; rdx 0xffffffff88a3a1a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f629960;

   5: fn 0xffffffff889b1150 - pc 0xffffffff889b1190 / registers:

     rax               0x12; rdx 0xffffffff88a3a1a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f6299f0;

   6: fn 0xffffffff8899a710 - pc 0xffffffff8899a725 / registers:

     rax               0x12; rdx 0xffffffff88a3a1a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f629a60;

   7: fn 0xffffffff884f2290 - pc 0xffffffff884f289f / registers:

     rax               0x12; rdx 0xffffffff88a3a1a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f629a70;

   8: fn 0xffffffff884f1d20 - pc 0xffffffff884f1d81 / registers:

     rax               0x12; rdx 0xffffffff88a3a1a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f629d30;

   9: fn 0xffffffff88161a50 - pc 0xffffffff8818d4ab / registers:

     rax               0x12; rdx 0xffffffff88a3a1a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f629f30;

  10: fn 0xffffffff88152f60 - pc 0xffffffff88152fee / registers:

     rax               0x12; rdx 0xffffffff88a3a1a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f6403d0;

  11: fn 0xffffffff88110380 - pc 0xffffffff88110eff / registers:

     rax               0x12; rdx 0xffffffff88a3a1a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f640570;

  12: fn 0xffffffff8845cb70 - pc 0xffffffff8845cb7e / registers:

     rax               0x12; rdx 0xffffffff88a3a1a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f640f90;

  13: fn 0xffffffff887cdc50 - pc 0xffffffff887cdc66 / registers:

     rax               0x12; rdx 0xffffffff88a3a1a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f640fb0;

  14: fn 0xffffffff887b0280 - pc 0xffffffff887b02e9 / registers:

     rax               0x12; rdx 0xffffffff88a3a1a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f640fd0;


     rax               0x12; rdx 0xffffffff88a3a1a0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f641000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:106
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) 89yvfinwjerz0clyodmhm6lzz:?
(  4) ??:?
(  5) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/panicking.rs:220
(  6) ??:?
(  7) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/option.rs:961
(  8) /root/asterinas/kernel/src/syscall/clock_gettime.rs:29
(  9) /root/asterinas/kernel/src/syscall/mod.rs:164
( 10) /root/asterinas/kernel/src/syscall/mod.rs:328
( 11) /root/asterinas/kernel/src/thread/task.rs:69
( 12) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 13) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 14) /root/asterinas/ostd/src/task/task/mod.rs:341
make: *** [Makefile:167: run] Error 1
```

# syscall-preadv-aa77747f-handle_alloc_error-unwrap
Reachable unwrap panic caused by Heap allocation error through `preadv`

### Reference
https://github.com/asterinas/asterinas/issues/1245

### Describe the bug
There is a reachable unwrap panic in `handle_alloc_error()` at ostd/src/mm/heap_allocator.rs:26 when make a `preadv` syscall with specific argument.

https://github.com/asterinas/asterinas/blob/aa77747f94c4b1cb1237ba52414642827a6efc25/ostd/src/mm/heap_allocator.rs#L26


### To Reproduce
1. Compile a program which calls syscall `preadv`:
```C
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/uio.h>
#include <unistd.h>

int main() {
  const char *filename = "/proc/8/cmdline";
  int flags = O_RDONLY;
  int fd = open(filename, flags);

  struct iovec vec;
  char buffer[1024];
  vec.iov_base = buffer;
  vec.iov_len = sizeof(buffer);

  off_t pos_l = 0xa73771c48a;
  off_t pos_h = 16384;
  off_t pos = (pos_h << 32) |
              pos_l; // Combine high and low parts to form the full offset

  ssize_t result = syscall(SYS_preadv, fd, &vec, 1, pos, 0);
  return 0;
}

```
2. Run the compiled program in Asterinas.

### Expected behavior
Asterinas reports panic and is terminated.

### Environment
- Official docker asterinas/asterinas:0.8.0
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main aa77747f

### Logs

```
~ # /root/preadv.c 
panicked at /root/asterinas/ostd/src/mm/heap_allocator.rs:26:5:
Heap allocation error, layout = Layout { size: 2251799810014944, align: 8 (1 << 3) }
Printing stack trace:
   1: fn 0xffffffff8880e540 - pc 0xffffffff8880e558 / registers:

     rax               0x12; rdx 0xffffffff88a3a4d0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f728ce0;

   2: fn 0xffffffff8880e320 - pc 0xffffffff8880e498 / registers:

     rax               0x12; rdx 0xffffffff88a3a4d0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f728cf0;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax               0x12; rdx 0xffffffff88a3a4d0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f728e70;

   4: fn 0xffffffff889b1330 - pc 0xffffffff889b13b2 / registers:

     rax               0x12; rdx 0xffffffff88a3a4d0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f728e80;

   5: fn 0xffffffff887a71f0 - pc 0xffffffff887a7287 / registers:

     rax               0x12; rdx 0xffffffff88a3a4d0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f728f10;

   6: fn 0xffffffff887a7290 - pc 0xffffffff887a72ae / registers:

     rax               0x12; rdx 0xffffffff88a3a4d0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f728fb0;

   7: fn 0xffffffff8897b6c0 - pc 0xffffffff8897b6f9 / registers:

     rax               0x12; rdx 0xffffffff88a3a4d0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f728fd0;

   8: fn 0xffffffff8897b6a0 - pc 0xffffffff8897b6b3 / registers:

     rax               0x12; rdx 0xffffffff88a3a4d0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f729000;

   9: fn 0xffffffff88976110 - pc 0xffffffff88976167 / registers:

     rax               0x12; rdx 0xffffffff88a3a4d0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f729020;

  10: fn 0xffffffff88114610 - pc 0xffffffff8811466f / registers:

     rax               0x12; rdx 0xffffffff88a3a4d0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f729060;

  11: fn 0xffffffff880928f0 - pc 0xffffffff88092913 / registers:

     rax               0x12; rdx 0xffffffff88a3a4d0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f7290c0;

  12: fn 0xffffffff8808c260 - pc 0xffffffff8808c276 / registers:

     rax               0x12; rdx 0xffffffff88a3a4d0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f7290f0;

  13: fn 0xffffffff88521920 - pc 0xffffffff88521a51 / registers:

     rax               0x12; rdx 0xffffffff88a3a4d0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f729110;

  14: fn 0xffffffff885aabd0 - pc 0xffffffff885aac4b / registers:

     rax               0x12; rdx 0xffffffff88a3a4d0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f729390;

  15: fn 0xffffffff883d0200 - pc 0xffffffff883d0240 / registers:

     rax               0x12; rdx 0xffffffff88a3a4d0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f729500;

  16: fn 0xffffffff8818e400 - pc 0xffffffff8818e4d9 / registers:

     rax               0x12; rdx 0xffffffff88a3a4d0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f7296b0;

  17: fn 0xffffffff88190630 - pc 0xffffffff881906da / registers:

     rax               0x12; rdx 0xffffffff88a3a4d0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f729740;

  18: fn 0xffffffff883eb670 - pc 0xffffffff883eb6fc / registers:

     rax               0x12; rdx 0xffffffff88a3a4d0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f7297b0;

  19: fn 0xffffffff885ed220 - pc 0xffffffff885eddb7 / registers:

     rax               0x12; rdx 0xffffffff88a3a4d0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f729830;

  20: fn 0xffffffff885ecda0 - pc 0xffffffff885ece20 / registers:

     rax               0x12; rdx 0xffffffff88a3a4d0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f729e40;

  21: fn 0xffffffff885b4df0 - pc 0xffffffff885ea4b0 / registers:

     rax               0x12; rdx 0xffffffff88a3a4d0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f729f30;

  22: fn 0xffffffff885b4900 - pc 0xffffffff885b498e / registers:

     rax               0x12; rdx 0xffffffff88a3a4d0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f7403d0;

  23: fn 0xffffffff88595310 - pc 0xffffffff88595e8f / registers:

     rax               0x12; rdx 0xffffffff88a3a4d0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f740570;

  24: fn 0xffffffff883fc690 - pc 0xffffffff883fc69e / registers:

     rax               0x12; rdx 0xffffffff88a3a4d0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f740f90;

  25: fn 0xffffffff887cdfd0 - pc 0xffffffff887cdfe6 / registers:

     rax               0x12; rdx 0xffffffff88a3a4d0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f740fb0;

  26: fn 0xffffffff887b0600 - pc 0xffffffff887b0669 / registers:

     rax               0x12; rdx 0xffffffff88a3a4d0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f740fd0;


     rax               0x12; rdx 0xffffffff88a3a4d0; rcx                0x1; rbx                0x0;
     rsi                0x0; rdi                0x0; rbp                0x0; rsp 0xffff80007f741000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:106
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) 89yvfinwjerz0clyodmhm6lzz:?
(  4) ??:?
(  5) 2ko6a4rwavth91j7km1opv8mw:?
(  6) 2ko6a4rwavth91j7km1opv8mw:?
(  7) alloc.4436a8b0cd505e33-cgu.1:?
(  8) ??:?
(  9) ??:?
( 10) ??:?
( 11) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/vec/mod.rs:699
( 12) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/vec/mod.rs:481
( 13) /root/asterinas/kernel/src/process/process_vm/init_stack/mod.rs:386
( 14) /root/asterinas/kernel/src/fs/procfs/pid/cmdline.rs:30
( 15) /root/asterinas/kernel/src/fs/procfs/template/file.rs:66
( 16) /root/asterinas/kernel/src/fs/inode_handle/mod.rs:84
( 17) ??:?
( 18) /root/asterinas/kernel/src/fs/file_handle.rs:164
( 19) /root/asterinas/kernel/src/syscall/preadv.rs:115
( 20) /root/asterinas/kernel/src/syscall/preadv.rs:27
( 21) /root/asterinas/kernel/src/syscall/mod.rs:178
( 22) /root/asterinas/kernel/src/syscall/mod.rs:328
( 23) /root/asterinas/kernel/src/thread/task.rs:69
( 24) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 25) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 26) /root/asterinas/ostd/src/task/task/mod.rs:341
make: *** [Makefile:167: run] Error 1
```

# cmd-asterinas-af210df-print_stack_trace-deadlock
Deadlock in `print_stack_trace()`

### Reference
https://github.com/asterinas/asterinas/issues/1248

### Describe the bug
There is a deadlock in `print_stack_trace()` when panic, due to the double panic described in #1167 and the lock introduced in cf4aee0.

### To Reproduce

1. To trigger a panic, for example run `$ ping www`.

### Expected behavior

During the stack trace print process, Asterinas panic again and get stuck.

### Environment
- Official docker asterinas/asterinas:0.8.0
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main af210df

# syscall-clone-745ac6d-check_unsupported_flags-panic
Reachable panic in `check_unsupported_flags()` caused by invalid `clone_flags` in syscall `clone`

### Reference


### Describe the bug
There is a reachable panic in `CloneFlags::check_unsupported_flags()` at kernel/src/process/clone.rs:118 when make a `clone` syscall with invalid `clone_flags` argument.

https://github.com/asterinas/asterinas/blob/745ac6d982638d3548f09ea2aa2103e37c135153/kernel/src/process/clone.rs#L118

### To Reproduce

1. Compile a program which calls `clone`:
```C
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int child_func(void *arg) {
    return 0;
}

int main() {
    char *addr = malloc(1024);
    clone(child_func, addr, 0x80000000, "fuzz");

    return 0;
}
```
2. Run the compiled program in Asterinas.

### Expected behavior

Asterinas reports panic and is terminated.

### Environment
- Official docker asterinas/asterinas:0.8.0
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 745ac6d

### Logs
```
panicked at /root/asterinas/kernel/src/process/clone.rs:118:13:
contains unsupported clone flags: CLONE_IO
Printing stack trace:
   1: fn 0xffffffff88828950 - pc 0xffffffff888289ba / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88851657; rbp                0x0; rsp 0xffff80000a3296e0;

   2: fn 0xffffffff88828760 - pc 0xffffffff888288b0 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88851657; rbp                0x0; rsp 0xffff80000a329740;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88851657; rbp                0x0; rsp 0xffff80000a329890;

   4: fn 0xffffffff889c2ce0 - pc 0xffffffff889c2d62 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88851657; rbp                0x0; rsp 0xffff80000a3298a0;

   5: fn 0xffffffff8853b040 - pc 0xffffffff8853b187 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88851657; rbp                0x0; rsp 0xffff80000a329930;

   6: fn 0xffffffff8853b1a0 - pc 0xffffffff8853b1e9 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88851657; rbp                0x0; rsp 0xffff80000a3299f0;

   7: fn 0xffffffff8821ce60 - pc 0xffffffff8821cf8e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88851657; rbp                0x0; rsp 0xffff80000a329c30;

   8: fn 0xffffffff88453870 - pc 0xffffffff88465187 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88851657; rbp                0x0; rsp 0xffff80000a329f30;

   9: fn 0xffffffff88257770 - pc 0xffffffff882577fe / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88851657; rbp                0x0; rsp 0xffff80000a3403d0;

  10: fn 0xffffffff882b0c10 - pc 0xffffffff882b178f / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88851657; rbp                0x0; rsp 0xffff80000a340570;

  11: fn 0xffffffff883680a0 - pc 0xffffffff883680ae / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88851657; rbp                0x0; rsp 0xffff80000a340f90;

  12: fn 0xffffffff887ed800 - pc 0xffffffff887ed816 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88851657; rbp                0x0; rsp 0xffff80000a340fb0;

  13: fn 0xffffffff887b41a0 - pc 0xffffffff887b4209 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88851657; rbp                0x0; rsp 0xffff80000a340fd0;


     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88851657; rbp                0x0; rsp 0xffff80000a341000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:113
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) 612kfi8k15yg732ys5rlrja7q:?
(  4) ??:?
(  5) cb0hexq8x5nes814zm7udstlx:?
(  6) /root/asterinas/kernel/src/process/clone.rs:133
(  7) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/result.rs:1100
(  8) /root/asterinas/kernel/src/syscall/mod.rs:208
(  9) /root/asterinas/kernel/src/syscall/mod.rs:328
( 10) /root/asterinas/kernel/src/thread/task.rs:69
( 11) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 12) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 13) /root/asterinas/ostd/src/task/mod.rs:175
make: *** [Makefile:173: run] Error 1
```

# syscall-mmap-9638744-Cursor::new-integeroverflow
Integer overflow in `Cursor::new()`

### Reference
https://github.com/asterinas/asterinas/issues/1285

### Describe the bug
There is a integer overflow in `Cursor::new()` at ostd/src/mm/page_table/cursor.rs:175 when Asterinas trying to access a low memory address which is set by syscall `mmap` with empty file.

https://github.com/asterinas/asterinas/blob/963874471284ed014b76d268d933b6d13073c2cc/ostd/src/mm/page_table/cursor.rs#L175

### To Reproduce
1. Compile a program which calls `mmap` with empty file and access low memory address: 
```C
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

struct MyStructure {
  int myNum;
  char myLetter;
};

int main() {
  int fd = -1;
  struct MyStructure *p = NULL;

  FILE *f = fopen("./testfile-2", "w+");
  fd = fileno(f);
  void* addr = mmap(NULL, 0x1000, 0x3, 0x4b112, fd, 0x0);
  perror("mmap");
  printf("address: %p\n", addr);
  printf("%d\n", p->myNum);

  return 0;
}
```
2. Run the compiled program in Asterinas.

### Expected behavior

Asterinas reports subtract with overflow and is terminated.

### Environment
- Official docker asterinas/asterinas:0.8.0
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 9638744

### Logs
```
~ # /root/mmap.c
mmap: Success
address: (nil)
panicked at /root/asterinas/ostd/src/mm/page_table/cursor.rs:175:46:
attempt to subtract with overflow
Printing stack trace:
   1: fn 0xffffffff88826290 - pc 0xffffffff888262fa / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff80000b8bee20;

   2: fn 0xffffffff888260a0 - pc 0xffffffff888261f0 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff80000b8bee80;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff80000b8befd0;

   4: fn 0xffffffff889c2c50 - pc 0xffffffff889c2cd2 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff80000b8befe0;

   5: fn 0xffffffff889a0260 - pc 0xffffffff889a028e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff80000b8bf070;

   6: fn 0xffffffff8882e8c0 - pc 0xffffffff8882ebec / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff80000b8bf0c0;

   7: fn 0xffffffff88830df0 - pc 0xffffffff88830e18 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff80000b8bf310;

   8: fn 0xffffffff888aee40 - pc 0xffffffff888aee5a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff80000b8bf3a0;

   9: fn 0xffffffff888964a0 - pc 0xffffffff888964d2 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff80000b8bf3c0;

  10: fn 0xffffffff8858ef60 - pc 0xffffffff8858f44c / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff80000b8bf5a0;

  11: fn 0xffffffff8858e090 - pc 0xffffffff8858e24b / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff80000b8bfa80;

  12: fn 0xffffffff88555c60 - pc 0xffffffff885560b4 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff80000b8bff30;

  13: fn 0xffffffff882efdb0 - pc 0xffffffff882efeb1 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff80000b8c00b0;

  14: fn 0xffffffff8855ac20 - pc 0xffffffff8855aff6 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff80000b8c01c0;

  15: fn 0xffffffff8855ab10 - pc 0xffffffff8855abec / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff80000b8c0500;

  16: fn 0xffffffff885d7790 - pc 0xffffffff885d8326 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff80000b8c0570;

  17: fn 0xffffffff8837c890 - pc 0xffffffff8837c89e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff80000b8c0f90;

  18: fn 0xffffffff887ed560 - pc 0xffffffff887ed576 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff80000b8c0fb0;

  19: fn 0xffffffff887b4040 - pc 0xffffffff887b40a9 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff80000b8c0fd0;


     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff80000b8c1000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:113
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) 612kfi8k15yg732ys5rlrja7q:?
(  4) ??:?
(  5) ??:?
(  6) /root/asterinas/ostd/src/mm/page_table/cursor.rs:175
(  7) /root/asterinas/ostd/src/mm/page_table/cursor.rs:399
(  8) /root/asterinas/ostd/src/mm/page_table/mod.rs:244
(  9) /root/asterinas/ostd/src/mm/vm_space.rs:106
( 10) /root/asterinas/kernel/src/vm/vmar/vm_mapping.rs:320
( 11) /root/asterinas/kernel/src/vm/vmar/vm_mapping.rs:231
( 12) /root/asterinas/kernel/src/vm/vmar/mod.rs:327
( 13) /root/asterinas/kernel/src/vm/vmar/static_cap.rs:183
( 14) /root/asterinas/kernel/src/thread/exception.rs:58
( 15) /root/asterinas/kernel/src/thread/exception.rs:21
( 16) /root/asterinas/kernel/src/thread/task.rs:68
( 17) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 18) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 19) /root/asterinas/ostd/src/task/mod.rs:175
make: *** [Makefile:173: run] Error 1
```

# syscall-?-9638744-VsockStreamSocket::drop-unwrap
Reachable unwrap panic in `VsockStreamSocket::drop`

### Reference
https://github.com/asterinas/asterinas/issues/1288

### Describe the bug
There is a reachable unwrap panic in `VsockStreamSocket::drop()` at kernel/src/net/socket/vsock/stream/socket.rs:354 when closing file descriptor in multiple processes.

https://github.com/asterinas/asterinas/blob/963874471284ed014b76d268d933b6d13073c2cc/kernel/src/net/socket/vsock/stream/socket.rs#L354

### To Reproduce
Closing file descriptor in multiple processes.


### Expected behavior

Asterinas reports subtract with overflow and is terminated.

### Environment
- Official docker asterinas/asterinas:0.8.0
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 9638744

### Logs
```
panicked at /root/asterinas/kernel/src/net/socket/vsock/stream/socket.rs:354:45:
called `Option::unwrap()` on a `None` value
Printing stack trace:
   1: fn 0xffffffff88826290 - pc 0xffffffff888262fa / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff800003ea9740;

   2: fn 0xffffffff888260a0 - pc 0xffffffff888261f0 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff800003ea97a0;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff800003ea98f0;

   4: fn 0xffffffff889c2c50 - pc 0xffffffff889c2cd2 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff800003ea9900;

   5: fn 0xffffffff889c2df0 - pc 0xffffffff889c2e30 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff800003ea9990;

   6: fn 0xffffffff889ac650 - pc 0xffffffff889ac665 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff800003ea9a00;

   7: fn 0xffffffff883b44e0 - pc 0xffffffff883b452c / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff800003ea9a10;

   8: fn 0xffffffff88392de0 - pc 0xffffffff88392df3 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff800003ea9c10;

   9: fn 0xffffffff8838f820 - pc 0xffffffff8838f84d / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff800003ea9c40;

  10: fn 0xffffffff88075f60 - pc 0xffffffff88075f7d / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff800003ea9c70;

  11: fn 0xffffffff88396d90 - pc 0xffffffff88396e13 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff800003ea9ca0;

  12: fn 0xffffffff883934e0 - pc 0xffffffff883934ea / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff800003ea9d20;

  13: fn 0xffffffff885626e0 - pc 0xffffffff885626f8 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff800003ea9d30;

  14: fn 0xffffffff8835f440 - pc 0xffffffff8835f7ad / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff800003ea9d50;

  15: fn 0xffffffff8845f3a0 - pc 0xffffffff884622cc / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff800003ea9f30;

  16: fn 0xffffffff88230420 - pc 0xffffffff882304ae / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff800003ec03d0;

  17: fn 0xffffffff885d7790 - pc 0xffffffff885d830f / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff800003ec0570;

  18: fn 0xffffffff8837c890 - pc 0xffffffff8837c89e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff800003ec0f90;

  19: fn 0xffffffff887ed560 - pc 0xffffffff887ed576 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff800003ec0fb0;

  20: fn 0xffffffff887b4040 - pc 0xffffffff887b40a9 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff800003ec0fd0;


     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887ee247; rbp                0x0; rsp 0xffff800003ec1000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:113
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) 612kfi8k15yg732ys5rlrja7q:?
(  4) ??:?
(  5) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/panicking.rs:220
(  6) ??:?
(  7) ??:?
(  8) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ptr/mod.rs:542
(  9) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ptr/mod.rs:542
( 10) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/sync.rs:1841
( 11) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/sync.rs:2509
( 12) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ptr/mod.rs:542
( 13) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/mem/mod.rs:938
( 14) /root/asterinas/kernel/src/syscall/close.rs:20
( 15) /root/asterinas/kernel/src/syscall/mod.rs:157
( 16) /root/asterinas/kernel/src/syscall/mod.rs:328
( 17) /root/asterinas/kernel/src/thread/task.rs:69
( 18) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 19) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 20) /root/asterinas/ostd/src/task/mod.rs:175
make: *** [Makefile:173: run] Error 1
```

# syscall-mmap-c68302f-handle_page_faults_around-integeroverflow
Integer overflow in `handle_page_faults_around()`

### Reference
https://github.com/asterinas/asterinas/issues/1316

### Describe the bug
There is a integer overflow in `handle_page_faults_around()` at kernel/src/vm/vmar/vm_mapping.rs:308 when Asterinas trying to access a low memory address which is set by syscall `mmap` with empty file and offset.

https://github.com/asterinas/asterinas/blob/c68302f7007225fa47f22a1085a8c59dcdae2ad4/kernel/src/vm/vmar/vm_mapping.rs#L308

Similar to #1285, but `vmo_offset` is related with `offset` of `mmap`.
As a result, if we set a large `offset`, there will be a subtract with overflow error.

### To Reproduce
1. Compile a program which calls `mmap` with empty file and access low memory address: 
```C
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

struct MyStructure {
  int myNum;
  char myLetter;
};

int main() {
  int fd = -1;
  struct MyStructure *p = NULL;

  FILE *f = fopen("./testfile-2", "w+");
  fd = fileno(f);
  void* addr = mmap(NULL, 0x1000, 0x3, 0x4b112, fd, 16384);
  printf("%d\n", p->myNum);

  return 0;
}
```
2. Run the compiled program in Asterinas.

### Expected behavior

Asterinas reports subtract with overflow and is terminated.

### Environment
- Official docker asterinas/asterinas:0.8.1
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main c68302f

### Logs
```
~ # /root/mmap.c 
panicked at /root/asterinas/kernel/src/vm/vmar/vm_mapping.rs:308:30:
attempt to subtract with overflow
Printing stack trace:
   1: fn 0xffffffff8880e810 - pc 0xffffffff8880e87a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887d9247; rbp                0x0; rsp 0xffff80000a33f300;

   2: fn 0xffffffff8880e620 - pc 0xffffffff8880e770 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887d9247; rbp                0x0; rsp 0xffff80000a33f360;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887d9247; rbp                0x0; rsp 0xffff80000a33f4b0;

   4: fn 0xffffffff889c2020 - pc 0xffffffff889c20a2 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887d9247; rbp                0x0; rsp 0xffff80000a33f4c0;

   5: fn 0xffffffff8899f630 - pc 0xffffffff8899f65e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887d9247; rbp                0x0; rsp 0xffff80000a33f550;

   6: fn 0xffffffff88531330 - pc 0xffffffff88531517 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887d9247; rbp                0x0; rsp 0xffff80000a33f5a0;

   7: fn 0xffffffff88530460 - pc 0xffffffff8853061b / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887d9247; rbp                0x0; rsp 0xffff80000a33fa80;

   8: fn 0xffffffff88527a50 - pc 0xffffffff88527ea4 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887d9247; rbp                0x0; rsp 0xffff80000a33ff30;

   9: fn 0xffffffff885237a0 - pc 0xffffffff885238a1 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887d9247; rbp                0x0; rsp 0xffff80000a3400b0;

  10: fn 0xffffffff8852ca10 - pc 0xffffffff8852cde6 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887d9247; rbp                0x0; rsp 0xffff80000a3401c0;

  11: fn 0xffffffff8852c900 - pc 0xffffffff8852c9dc / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887d9247; rbp                0x0; rsp 0xffff80000a340500;

  12: fn 0xffffffff88383a70 - pc 0xffffffff88384606 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887d9247; rbp                0x0; rsp 0xffff80000a340570;

  13: fn 0xffffffff880ef350 - pc 0xffffffff880ef35e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887d9247; rbp                0x0; rsp 0xffff80000a340f90;

  14: fn 0xffffffff887a6510 - pc 0xffffffff887a6526 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887d9247; rbp                0x0; rsp 0xffff80000a340fb0;

  15: fn 0xffffffff887d90e0 - pc 0xffffffff887d9149 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887d9247; rbp                0x0; rsp 0xffff80000a340fd0;


     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887d9247; rbp                0x0; rsp 0xffff80000a341000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:113
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) e79ansgam06sthnp1tvixwo3s:?
(  4) ??:?
(  5) ??:?
(  6) /root/asterinas/kernel/src/vm/vmar/vm_mapping.rs:308
(  7) /root/asterinas/kernel/src/vm/vmar/vm_mapping.rs:231
(  8) /root/asterinas/kernel/src/vm/vmar/mod.rs:327
(  9) /root/asterinas/kernel/src/vm/vmar/static_cap.rs:183
( 10) /root/asterinas/kernel/src/thread/exception.rs:58
( 11) /root/asterinas/kernel/src/thread/exception.rs:21
( 12) /root/asterinas/kernel/src/thread/task.rs:68
( 13) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 14) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 15) /root/asterinas/ostd/src/task/mod.rs:175
make: *** [Makefile:174: run] Error 1
```

# syscall-recvfrom-8bfbdf6-create_message_buffer-capacity_overflow
Capacity overflow in `create_message_buffer()` by invalid `size`

### Reference
https://github.com/asterinas/asterinas/issues/1324

### Describe the bug
There is a capacity overflow in `handle_page_faults_around()` at kernel/src/net/socket/util/message_header.rs:65 when call `recvfrom` with large `size`.

https://github.com/asterinas/asterinas/blob/8bfbdf6642a9d04db3490792926d6a299d1e30be/kernel/src/net/socket/util/message_header.rs#L65

### To Reproduce
1. Compile a program which calls `recvfrom`: 
```C
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <arpa/inet.h>

int main() {
    int fd = socket(1, 0x5, 0);
    char ubuf[8];
    ssize_t size = -7;
    int flags = 0x0;
    struct sockaddr_in addr;
    socklen_t addr_len = 18;

    recvfrom(fd, ubuf, size, flags, (struct sockaddr *)&addr, &addr_len);

    return 0;
}
```
2. Run the compiled program in Asterinas.

### Expected behavior

Asterinas reports capacity overflow and is terminated.

### Environment
- Official docker asterinas/asterinas:0.8.1
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 8bfbdf6

### Logs
```
~ # /root/recvfrom.c
DEBUG: sockfd = 79, buf = 0x291d000, len = 18446744073709551609, flags = (empty), src_addr = 0x100215c0, addrlen_ptr = 0x12
panicked at /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/raw_vec.rs:25:5:
capacity overflow
Printing stack trace:
   1: fn 0xffffffff887e4960 - pc 0xffffffff887e49ca / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883ab57; rbp                0x0; rsp 0xffff80000a6a94a0;

   2: fn 0xffffffff887e4770 - pc 0xffffffff887e48c0 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883ab57; rbp                0x0; rsp 0xffff80000a6a9500;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883ab57; rbp                0x0; rsp 0xffff80000a6a9650;

   4: fn 0xffffffff889c1f90 - pc 0xffffffff889c2012 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883ab57; rbp                0x0; rsp 0xffff80000a6a9660;

   5: fn 0xffffffff88987480 - pc 0xffffffff889874a9 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883ab57; rbp                0x0; rsp 0xffff80000a6a96f0;

   6: fn 0xffffffff88988bb0 - pc 0xffffffff88988bee / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883ab57; rbp                0x0; rsp 0xffff80000a6a9730;

   7: fn 0xffffffff8874c680 - pc 0xffffffff8874c6df / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883ab57; rbp                0x0; rsp 0xffff80000a6a9770;

   8: fn 0xffffffff88741850 - pc 0xffffffff88741884 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883ab57; rbp                0x0; rsp 0xffff80000a6a97d0;

   9: fn 0xffffffff887492e0 - pc 0xffffffff887492ff / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883ab57; rbp                0x0; rsp 0xffff80000a6a9870;

  10: fn 0xffffffff882d2e60 - pc 0xffffffff882d2ec1 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883ab57; rbp                0x0; rsp 0xffff80000a6a9890;

  11: fn 0xffffffff8835d530 - pc 0xffffffff8835d5f1 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883ab57; rbp                0x0; rsp 0xffff80000a6a98d0;

  12: fn 0xffffffff88318df0 - pc 0xffffffff883193a5 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883ab57; rbp                0x0; rsp 0xffff80000a6a9a90;

  13: fn 0xffffffff884376d0 - pc 0xffffffff88445a0f / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883ab57; rbp                0x0; rsp 0xffff80000a6a9f30;

  14: fn 0xffffffff880f6fe0 - pc 0xffffffff880f706e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883ab57; rbp                0x0; rsp 0xffff80000a6c03d0;

  15: fn 0xffffffff883eb490 - pc 0xffffffff883ec00f / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883ab57; rbp                0x0; rsp 0xffff80000a6c0570;

  16: fn 0xffffffff883a3ac0 - pc 0xffffffff883a3ace / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883ab57; rbp                0x0; rsp 0xffff80000a6c0f90;

  17: fn 0xffffffff888573a0 - pc 0xffffffff888573b6 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883ab57; rbp                0x0; rsp 0xffff80000a6c0fb0;

  18: fn 0xffffffff88832150 - pc 0xffffffff888321b9 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883ab57; rbp                0x0; rsp 0xffff80000a6c0fd0;


     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883ab57; rbp                0x0; rsp 0xffff80000a6c1000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:113
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) 73ctk84we8vz1kw7hv2sr00er:?
(  4) ??:?
(  5) alloc.4436a8b0cd505e33-cgu.0:?
(  6) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/raw_vec.rs:594
(  7) ??:?
(  8) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/vec/spec_from_elem.rs:52
(  9) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/vec/mod.rs:2737
( 10) /root/asterinas/kernel/src/net/socket/util/message_header.rs:65
( 11) /root/asterinas/kernel/src/net/socket/unix/stream/socket.rs:331
( 12) /root/asterinas/kernel/src/syscall/recvfrom.rs:29
( 13) /root/asterinas/kernel/src/syscall/mod.rs:220
( 14) /root/asterinas/kernel/src/syscall/mod.rs:328
( 15) /root/asterinas/kernel/src/thread/task.rs:69
( 16) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 17) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 18) /root/asterinas/ostd/src/task/mod.rs:175
make: *** [Makefile:174: run] Error 1
```

# syscall-futex-8bfbdf6-FutexKey::load_val-unwrap
Reachable unwrap in `FutexKey::load_val()`

### Reference
https://github.com/asterinas/asterinas/issues/1334

### Describe the bug
There is a reachable unwrap in `FutexKey::load_val()` at kernel/src/process/posix_thread/futex.rs:333 when call `futex` with memory region without read access.

https://github.com/asterinas/asterinas/blob/53ce7df53ccb497395e87f8ee5261d0c3a3db98e/kernel/src/process/posix_thread/futex.rs#L333

### To Reproduce
1. Compile a program which calls `futex` with memory region without read access.: 
```C
#include <errno.h>
#include <linux/futex.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
  void *addr1 = mmap(NULL, 4096, PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  void *addr2 = mmap(NULL, 1048576, PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  syscall(SYS_futex, addr1, 0x189, 95, NULL, addr2, 0xff49);
  perror("futex");

  return 0;
}
```
2. Run the compiled program in Asterinas.

### Expected behavior

Asterinas reports unwrap panic and is terminated.

### Environment
- Official docker asterinas/asterinas:0.8.1
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 8bfbdf6

### Logs
```
panicked at /root/asterinas/kernel/src/process/posix_thread/futex.rs:333:53:
called `Result::unwrap()` on an `Err` value: Error { errno: EFAULT, msg: None }
Printing stack trace:
   1: fn 0xffffffff887e71b0 - pc 0xffffffff887e721a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3a7; rbp                0x0; rsp 0xffff80007caa8e80;

   2: fn 0xffffffff887e6fc0 - pc 0xffffffff887e7110 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3a7; rbp                0x0; rsp 0xffff80007caa8ee0;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3a7; rbp                0x0; rsp 0xffff80007caa9030;

   4: fn 0xffffffff889c4810 - pc 0xffffffff889c4892 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3a7; rbp                0x0; rsp 0xffff80007caa9040;

   5: fn 0xffffffff889a3910 - pc 0xffffffff889a3a3b / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3a7; rbp                0x0; rsp 0xffff80007caa90d0;

   6: fn 0xffffffff8846dd30 - pc 0xffffffff8846ded4 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3a7; rbp                0x0; rsp 0xffff80007caa91d0;

   7: fn 0xffffffff8846ba80 - pc 0xffffffff8846c3ad / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3a7; rbp                0x0; rsp 0xffff80007caa92d0;

   8: fn 0xffffffff8846ba50 - pc 0xffffffff8846ba75 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3a7; rbp                0x0; rsp 0xffff80007caa9870;

   9: fn 0xffffffff884e2d00 - pc 0xffffffff884e32c8 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3a7; rbp                0x0; rsp 0xffff80007caa98a0;

  10: fn 0xffffffff885766d0 - pc 0xffffffff8859f093 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3a7; rbp                0x0; rsp 0xffff80007caa9f30;

  11: fn 0xffffffff88576310 - pc 0xffffffff8857639e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3a7; rbp                0x0; rsp 0xffff80007cac03d0;

  12: fn 0xffffffff884b2230 - pc 0xffffffff884b2daf / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3a7; rbp                0x0; rsp 0xffff80007cac0570;

  13: fn 0xffffffff883b93e0 - pc 0xffffffff883b93ee / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3a7; rbp                0x0; rsp 0xffff80007cac0f90;

  14: fn 0xffffffff88859bf0 - pc 0xffffffff88859c06 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3a7; rbp                0x0; rsp 0xffff80007cac0fb0;

  15: fn 0xffffffff888349a0 - pc 0xffffffff88834a09 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3a7; rbp                0x0; rsp 0xffff80007cac0fd0;


     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3a7; rbp                0x0; rsp 0xffff80007cac1000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:113
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) 73ctk84we8vz1kw7hv2sr00er:?
(  4) ??:?
(  5) ??:?
(  6) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/result.rs:1102
(  7) /root/asterinas/kernel/src/process/posix_thread/futex.rs:44
(  8) /root/asterinas/kernel/src/process/posix_thread/futex.rs:23
(  9) /root/asterinas/kernel/src/syscall/futex.rs:46
( 10) /root/asterinas/kernel/src/syscall/mod.rs:220
( 11) /root/asterinas/kernel/src/syscall/mod.rs:328
( 12) /root/asterinas/kernel/src/thread/task.rs:69
( 13) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 14) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 15) /root/asterinas/ostd/src/task/mod.rs:175
make: *** [Makefile:172: run] Error 1
```

# syscall-getdents-8bfbdf6-sys_getdents-capacity_overflow
Capacity overflow in `sys_getdents()`

### Reference
https://github.com/asterinas/asterinas/issues/1326

### Describe the bug
There is a capacity overflow in `sys_getdents()` at kernel/src/syscall/getdents64.rs:36 when call `sys_getdents` with large `buf_len`.

https://github.com/asterinas/asterinas/blob/f01772ca853e76d3076a561da3281034e3a46196/kernel/src/syscall/getdents64.rs#L36

### To Reproduce
1. Compile a program which calls `sys_getdents`: 
```C
#define _GNU_SOURCE
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int fd = open("/proc", O_RDONLY);
  char *buffer = (char *)0x4;
  size_t count = 0xffffffffffffffff;

  long result = syscall(SYS_getdents, fd, buffer, count);

  perror("getdents");

  return 0;
}
```
2. Run the compiled program in Asterinas.

### Expected behavior

Asterinas reports capacity overflow and is terminated.

### Environment
- Official docker asterinas/asterinas:0.8.1
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 8bfbdf6

### Logs
```
~ # /root/getdents.c
panicked at /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/raw_vec.rs:25:5:
capacity overflow
Printing stack trace:
   1: fn 0xffffffff887e5490 - pc 0xffffffff887e54fa / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b687; rbp                0x0; rsp 0xffff80000a9296c0;

   2: fn 0xffffffff887e52a0 - pc 0xffffffff887e53f0 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b687; rbp                0x0; rsp 0xffff80000a929720;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b687; rbp                0x0; rsp 0xffff80000a929870;

   4: fn 0xffffffff889c2af0 - pc 0xffffffff889c2b72 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b687; rbp                0x0; rsp 0xffff80000a929880;

   5: fn 0xffffffff88987fe0 - pc 0xffffffff88988009 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b687; rbp                0x0; rsp 0xffff80000a929910;

   6: fn 0xffffffff88989710 - pc 0xffffffff8898974e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b687; rbp                0x0; rsp 0xffff80000a929950;

   7: fn 0xffffffff8874d1b0 - pc 0xffffffff8874d20f / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b687; rbp                0x0; rsp 0xffff80000a929990;

   8: fn 0xffffffff88742380 - pc 0xffffffff887423b4 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b687; rbp                0x0; rsp 0xffff80000a9299f0;

   9: fn 0xffffffff88749e10 - pc 0xffffffff88749e2f / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b687; rbp                0x0; rsp 0xffff80000a929a90;

  10: fn 0xffffffff885af2a0 - pc 0xffffffff885af899 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b687; rbp                0x0; rsp 0xffff80000a929ab0;

  11: fn 0xffffffff884ac9d0 - pc 0xffffffff884c30aa / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b687; rbp                0x0; rsp 0xffff80000a929f30;

  12: fn 0xffffffff884ac1b0 - pc 0xffffffff884ac23e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b687; rbp                0x0; rsp 0xffff80000a9403d0;

  13: fn 0xffffffff884a15f0 - pc 0xffffffff884a216f / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b687; rbp                0x0; rsp 0xffff80000a940570;

  14: fn 0xffffffff883a06f0 - pc 0xffffffff883a06fe / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b687; rbp                0x0; rsp 0xffff80000a940f90;

  15: fn 0xffffffff88857ed0 - pc 0xffffffff88857ee6 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b687; rbp                0x0; rsp 0xffff80000a940fb0;

  16: fn 0xffffffff88832c80 - pc 0xffffffff88832ce9 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b687; rbp                0x0; rsp 0xffff80000a940fd0;


     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b687; rbp                0x0; rsp 0xffff80000a941000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:113
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) 73ctk84we8vz1kw7hv2sr00er:?
(  4) ??:?
(  5) alloc.4436a8b0cd505e33-cgu.0:?
(  6) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/raw_vec.rs:594
(  7) ??:?
(  8) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/vec/spec_from_elem.rs:52
(  9) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/vec/mod.rs:2737
( 10) /root/asterinas/kernel/src/syscall/getdents64.rs:36
( 11) /root/asterinas/kernel/src/syscall/mod.rs:171
( 12) /root/asterinas/kernel/src/syscall/mod.rs:328
( 13) /root/asterinas/kernel/src/thread/task.rs:69
( 14) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 15) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 16) /root/asterinas/ostd/src/task/mod.rs:175
```

# syscall-getdents64-8bfbdf6-sys_getdents-capacity_overflow
Capacity overflow in `sys_getdents64()`

### Reference
https://github.com/asterinas/asterinas/issues/1326

# syscall-recvmsg-8bfbdf6-copy_iovs_from_user-panic
Reachable panic caused by Heap allocation error through `recvmsg`

### Reference
https://github.com/asterinas/asterinas/issues/1327

### Describe the bug
There is a reachable panic in `handle_alloc_error()` caused by large `msg_iovlen` of a `recvmsg` syscall.

https://github.com/asterinas/asterinas/blob/f01772ca853e76d3076a561da3281034e3a46196/kernel/src/util/iovec.rs#L117

Asterinas will fall into the `handle_alloc_error()` when allocing a large memory region.

### To Reproduce
1. Compile a program which calls `recvmsg`: 
```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <errno.h>

#define SOCKET_PATH "/tmp/test_socket"

int main() {
    int fd;
    struct sockaddr_un addr;
    char buffer[1024];
    struct iovec iov;
    struct msghdr msg;

    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket failed");
        return 1;
    }

    iov.iov_base = buffer;
    iov.iov_len = sizeof(buffer);

    memset(&msg, 0xff, sizeof(msg));

    msg.msg_iov = &iov;
    msg.msg_iovlen = 0xffffffffff; // <- This is the point
    int flags = 0x4;
    ssize_t result = recvmsg(fd, &msg, flags);

    return 0;
}
```
2. Run the compiled program in Asterinas.

### Expected behavior

Asterinas reports panic and is terminated.

### Environment
- Official docker asterinas/asterinas:0.8.1
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 8bfbdf6

### Logs
```
~ # /root/recvmsg.c
[    28.804] DEBUG: sockfd = 3, user_msghdr = CUserMsgHdr { msg_name: ffffffffffffffff, msg_namelen: ffffffff, msg_iov: 7ffffff2b800, msg_iovlen: ffffffff, msg_control: ffffffffffffffff, msg_controllen: ffffffff, msg_flags: ffffffff }, flags = MSG_DONTROUTE
panicked at /root/asterinas/ostd/src/mm/heap_allocator.rs:26:5:
Heap allocation error, layout = Layout { size: 68719476720, align: 8 (1 << 3) }
Printing stack trace:
   1: fn 0xffffffff887e5b60 - pc 0xffffffff887e5bca / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bd57; rbp                0x0; rsp 0xffff80000aa29090;

   2: fn 0xffffffff887e5970 - pc 0xffffffff887e5ac0 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bd57; rbp                0x0; rsp 0xffff80000aa290f0;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bd57; rbp                0x0; rsp 0xffff80000aa29240;

   4: fn 0xffffffff889c31c0 - pc 0xffffffff889c3242 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bd57; rbp                0x0; rsp 0xffff80000aa29250;

   5: fn 0xffffffff88896770 - pc 0xffffffff88896807 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bd57; rbp                0x0; rsp 0xffff80000aa292e0;

   6: fn 0xffffffff88896810 - pc 0xffffffff8889682e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bd57; rbp                0x0; rsp 0xffff80000aa29380;

   7: fn 0xffffffff8898f390 - pc 0xffffffff8898f3c9 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bd57; rbp                0x0; rsp 0xffff80000aa293a0;

   8: fn 0xffffffff8898f370 - pc 0xffffffff8898f383 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bd57; rbp                0x0; rsp 0xffff80000aa293d0;

   9: fn 0xffffffff88989de0 - pc 0xffffffff88989e37 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bd57; rbp                0x0; rsp 0xffff80000aa293f0;

  10: fn 0xffffffff8838d470 - pc 0xffffffff8838d4cf / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bd57; rbp                0x0; rsp 0xffff80000aa29430;

  11: fn 0xffffffff88091500 - pc 0xffffffff88091523 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bd57; rbp                0x0; rsp 0xffff80000aa29490;

  12: fn 0xffffffff8808b1c0 - pc 0xffffffff8808b1d6 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bd57; rbp                0x0; rsp 0xffff80000aa294c0;

  13: fn 0xffffffff88418440 - pc 0xffffffff8841848d / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bd57; rbp                0x0; rsp 0xffff80000aa294e0;

  14: fn 0xffffffff88537ca0 - pc 0xffffffff88537cc2 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bd57; rbp                0x0; rsp 0xffff80000aa296d0;

  15: fn 0xffffffff8834a730 - pc 0xffffffff8834b026 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bd57; rbp                0x0; rsp 0xffff80000aa296f0;

  16: fn 0xffffffff884ad0a0 - pc 0xffffffff884bbda1 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bd57; rbp                0x0; rsp 0xffff80000aa29f30;

  17: fn 0xffffffff884ac880 - pc 0xffffffff884ac90e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bd57; rbp                0x0; rsp 0xffff80000aa403d0;

  18: fn 0xffffffff884a1cc0 - pc 0xffffffff884a283f / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bd57; rbp                0x0; rsp 0xffff80000aa40570;

  19: fn 0xffffffff883a06f0 - pc 0xffffffff883a06fe / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bd57; rbp                0x0; rsp 0xffff80000aa40f90;

  20: fn 0xffffffff888585a0 - pc 0xffffffff888585b6 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bd57; rbp                0x0; rsp 0xffff80000aa40fb0;

  21: fn 0xffffffff88833350 - pc 0xffffffff888333b9 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bd57; rbp                0x0; rsp 0xffff80000aa40fd0;


     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bd57; rbp                0x0; rsp 0xffff80000aa41000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:113
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) 73ctk84we8vz1kw7hv2sr00er:?
(  4) ??:?
(  5) doma056hm2pcoofausuwfcxo7:?
(  6) doma056hm2pcoofausuwfcxo7:?
(  7) alloc.4436a8b0cd505e33-cgu.1:?
(  8) ??:?
(  9) ??:?
( 10) ??:?
( 11) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/vec/mod.rs:699
( 12) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/vec/mod.rs:481
( 13) /root/asterinas/kernel/src/util/iovec.rs:119
( 14) /root/asterinas/kernel/src/util/net/socket.rs:116
( 15) /root/asterinas/kernel/src/syscall/recvmsg.rs:27
( 16) /root/asterinas/kernel/src/syscall/mod.rs:171
( 17) /root/asterinas/kernel/src/syscall/mod.rs:328
( 18) /root/asterinas/kernel/src/thread/task.rs:69
( 19) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 20) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 21) /root/asterinas/ostd/src/task/mod.rs:175
make: *** [Makefile:172: run] Error 1
```


# syscall-set_robust_list-8bfbdf6-RobustListHead::futex_addr-integer_overflow
Integer overflow in `RobustListHead::futex_addr()`

### Reference
https://github.com/asterinas/asterinas/issues/

### Describe the bug
There is a integer overflow in `RobustListHead::futex_addr()` at kernel/src/process/posix_thread/robust_list.rs:53.

https://github.com/asterinas/asterinas/blob/f01772ca853e76d3076a561da3281034e3a46196/kernel/src/process/posix_thread/robust_list.rs#L53

### To Reproduce
I'm tring to construct a minimal PoC.

### Expected behavior

Asterinas reports add with overflow and is terminated.

### Environment
- Official docker asterinas/asterinas:0.8.1
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 8bfbdf6

### Logs
```
panicked at /root/asterinas/kernel/src/process/posix_thread/robust_list.rs:53:9:
attempt to add with overflow
Printing stack trace:
   1: fn 0xffffffff887e5be0 - pc 0xffffffff887e5c4a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bdd7; rbp                0x0; rsp 0xffff80007c829110;

   2: fn 0xffffffff887e59f0 - pc 0xffffffff887e5b40 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bdd7; rbp                0x0; rsp 0xffff80007c829170;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bdd7; rbp                0x0; rsp 0xffff80007c8292c0;

   4: fn 0xffffffff889c3240 - pc 0xffffffff889c32c2 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bdd7; rbp                0x0; rsp 0xffff80007c8292d0;

   5: fn 0xffffffff889a0820 - pc 0xffffffff889a084e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bdd7; rbp                0x0; rsp 0xffff80007c829360;

   6: fn 0xffffffff884926a0 - pc 0xffffffff884926d0 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bdd7; rbp                0x0; rsp 0xffff80007c8293b0;

   7: fn 0xffffffff88492720 - pc 0xffffffff88492803 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bdd7; rbp                0x0; rsp 0xffff80007c8293d0;

   8: fn 0xffffffff883fc0d0 - pc 0xffffffff883fc3a7 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bdd7; rbp                0x0; rsp 0xffff80007c829460;

   9: fn 0xffffffff883fb970 - pc 0xffffffff883fbbb4 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bdd7; rbp                0x0; rsp 0xffff80007c829690;

  10: fn 0xffffffff883f86d0 - pc 0xffffffff883f8b70 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bdd7; rbp                0x0; rsp 0xffff80007c8298e0;

  11: fn 0xffffffff8845e8c0 - pc 0xffffffff8845e8f9 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bdd7; rbp                0x0; rsp 0xffff80007c829ef0;

  12: fn 0xffffffff8849f8e0 - pc 0xffffffff884cbcf5 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bdd7; rbp                0x0; rsp 0xffff80007c829f30;

  13: fn 0xffffffff883fa0e0 - pc 0xffffffff883fa16e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bdd7; rbp                0x0; rsp 0xffff80007c8403d0;

  14: fn 0xffffffff883c1170 - pc 0xffffffff883c1cef / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bdd7; rbp                0x0; rsp 0xffff80007c840570;

  15: fn 0xffffffff880de7f0 - pc 0xffffffff880de7fe / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bdd7; rbp                0x0; rsp 0xffff80007c840f90;

  16: fn 0xffffffff88858620 - pc 0xffffffff88858636 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bdd7; rbp                0x0; rsp 0xffff80007c840fb0;

  17: fn 0xffffffff888333d0 - pc 0xffffffff88833439 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bdd7; rbp                0x0; rsp 0xffff80007c840fd0;


     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883bdd7; rbp                0x0; rsp 0xffff80007c841000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:113
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) 73ctk84we8vz1kw7hv2sr00er:?
(  4) ??:?
(  5) ??:?
(  6) /root/asterinas/kernel/src/process/posix_thread/robust_list.rs:64
(  7) /root/asterinas/kernel/src/process/posix_thread/robust_list.rs:103
(  8) /root/asterinas/kernel/src/process/posix_thread/exit.rs:62
(  9) /root/asterinas/kernel/src/process/posix_thread/exit.rs:36
( 10) /root/asterinas/kernel/src/process/exit.rs:23
( 11) /root/asterinas/kernel/src/syscall/exit_group.rs:13
( 12) /root/asterinas/kernel/src/syscall/mod.rs:157
( 13) /root/asterinas/kernel/src/syscall/mod.rs:328
( 14) /root/asterinas/kernel/src/thread/task.rs:69
( 15) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 16) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 17) /root/asterinas/ostd/src/task/mod.rs:175
make: *** [Makefile:172: run] Error 1
```


# syscall-????-6fcad6ce-user_task_entry-unwrap
Reachable unwrap panic in `user_task_entry()`

### Reference


### Describe the bug
There is a reachable unwrap panic in `user_task_entry()` at kernel/src/thread/task.rs:77.

https://github.com/asterinas/asterinas/blob/25a918d132277eff7ea202cacc4bfa284de2dec5/kernel/src/thread/task.rs#L77

This is because of the `EFAULT` in `write_val` at process/signal/mod.rs:166:

https://github.com/asterinas/asterinas/blob/25a918d132277eff7ea202cacc4bfa284de2dec5/kernel/src/process/signal/mod.rs#L166

### To Reproduce
I'm tring to construct a minimal PoC.

### Expected behavior
Asterinas shall handle this error and not panic.

### Environment
- Official docker asterinas/asterinas:0.9.4
- Intel(R) Xeon(R) Gold 6230R CPU @ 2.10GHz
- Asterinas version: main 25a918d

### Logs
```
[   193.024] ERROR: Uncaught panic: called `Result::unwrap()` on an `Err` value: Error { errno: EFAULT, msg: None }
at /root/asterinas/kernel/src/thread/task.rs:77
Printing stack trace:
   1: fn 0xffffffff8881c7b0 - pc 0xffffffff8881c81a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff889c6896; rdi 0xffffffff887ea837; rbp                0x0; rsp 0xffffdfffff9e0990;

   2: fn 0xffffffff8804a7f0 - pc 0xffffffff8804b50c / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff889c6896; rdi 0xffffffff887ea837; rbp                0x0; rsp 0xffffdfffff9e09f0;

   3: fn 0xffffffff8804a7e0 - pc 0xffffffff8804a7ea / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff889c6896; rdi 0xffffffff887ea837; rbp                0x0; rsp 0xffffdfffff9e1010;

   4: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff889c6896; rdi 0xffffffff887ea837; rbp                0x0; rsp 0xffffdfffff9e1020;

   5: fn 0xffffffff889fbcc0 - pc 0xffffffff889fbd04 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff889c6896; rdi 0xffffffff887ea837; rbp                0x0; rsp 0xffffdfffff9e1030;

   6: fn 0xffffffff889f5a50 - pc 0xffffffff889f5b95 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff889c6896; rdi 0xffffffff887ea837; rbp                0x0; rsp 0xffffdfffff9e1070;

   7: fn 0xffffffff88221f70 - pc 0xffffffff88222d57 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff889c6896; rdi 0xffffffff887ea837; rbp                0x0; rsp 0xffffdfffff9e1170;

   8: fn 0xffffffff88560f90 - pc 0xffffffff88560f96 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff889c6896; rdi 0xffffffff887ea837; rbp                0x0; rsp 0xffffdfffff9e1bf0;

   9: fn 0xffffffff886253f0 - pc 0xffffffff88625407 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff889c6896; rdi 0xffffffff887ea837; rbp                0x0; rsp 0xffffdfffff9e1c00;

  10: fn 0xffffffff886280f0 - pc 0xffffffff88628108 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff889c6896; rdi 0xffffffff887ea837; rbp                0x0; rsp 0xffffdfffff9e1c20;

  11: fn 0xffffffff88622390 - pc 0xffffffff886223bf / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff889c6896; rdi 0xffffffff887ea837; rbp                0x0; rsp 0xffffdfffff9e1c40;

  12: fn 0xffffffff885e7210 - pc 0xffffffff885e7222 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff889c6896; rdi 0xffffffff887ea837; rbp                0x0; rsp 0xffffdfffff9e1c90;

  13: fn 0xffffffff883ca670 - pc 0xffffffff883ca686 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff889c6896; rdi 0xffffffff887ea837; rbp                0x0; rsp 0xffffdfffff9e1cc0;

  14: fn 0xffffffff88169f50 - pc 0xffffffff88169f5c / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff889c6896; rdi 0xffffffff887ea837; rbp                0x0; rsp 0xffffdfffff9e1f00;

  15: fn 0xffffffff8855dee0 - pc 0xffffffff8855deee / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff889c6896; rdi 0xffffffff887ea837; rbp                0x0; rsp 0xffffdfffff9e1f30;

  16: fn 0xffffffff888caf00 - pc 0xffffffff888caf28 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff889c6896; rdi 0xffffffff887ea837; rbp                0x0; rsp 0xffffdfffff9e1f50;

  17: fn 0xffffffff88847f20 - pc 0xffffffff88847fe7 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff889c6896; rdi 0xffffffff887ea837; rbp                0x0; rsp 0xffffdfffff9e1f90;


     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff889c6896; rdi 0xffffffff887ea837; rbp                0x0; rsp 0xffffdfffff9e2000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panic.rs:107
(  2) /root/asterinas/kernel/src/thread/oops.rs:116
(  3) ??:?
(  4) 4bdh3ctc2ehskuj9t0av352n0:?
(  5) ??:?
(  6) ??:?
(  7) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/result.rs:1103
(  8) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:250
(  9) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.3/src/panicking.rs:56
( 10) egnwob4oghfgpic2tyf3fftva:?
( 11) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.3/src/panicking.rs:42
( 12) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.3/src/panic.rs:87
( 13) /root/asterinas/kernel/src/thread/oops.rs:54
( 14) /root/asterinas/kernel/src/thread/task.rs:95
( 15) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:250
( 16) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2454
( 17) /root/asterinas/ostd/src/task/mod.rs:162
make: *** [Makefile:194: run] Error 1
```


# syscall-getrandom-6fcad6c-sys_getrandom-capacity_overflow
Capacity overflow in `sys_getrandom()`

### Reference
https://github.com/asterinas/asterinas/issues/

### Describe the bug
There is a capacity overflow in `sys_getrandom()` at kernel/src/syscall/getrandom.rs:14 when call syscall `getrandom` with large `count` (i.e., 0xffff_ffff_ffff_ffff).

https://github.com/asterinas/asterinas/blob/8b1c69987f422f53f6a9075cf374aaea9e52d870/kernel/src/syscall/getrandom.rs#L14

### To Reproduce
1. Compile a program which calls `getrandom`: 
```C
#define _GNU_SOURCE
#include <errno.h>
#include <linux/random.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

int main() {
  unsigned char buffer[16];
  size_t num_bytes = -1;

  syscall(SYS_getrandom, buffer, num_bytes, 0);
  perror("getrandom");

  return 0;
}
```
2. Run the compiled program in Asterinas.

### Expected behavior

Asterinas reports capacity overflow and is terminated.

### Environment
- Official docker asterinas/asterinas:0.8.1
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 6fcad6c

### Logs
```
~ # /root/getrandom.c
panicked at /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/raw_vec.rs:25:5:
capacity overflow
Printing stack trace:
   1: fn 0xffffffff887e71e0 - pc 0xffffffff887e724a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3d7; rbp                0x0; rsp 0xffff80000aa297b0;

   2: fn 0xffffffff887e6ff0 - pc 0xffffffff887e7140 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3d7; rbp                0x0; rsp 0xffff80000aa29810;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3d7; rbp                0x0; rsp 0xffff80000aa29960;

   4: fn 0xffffffff889c4840 - pc 0xffffffff889c48c2 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3d7; rbp                0x0; rsp 0xffff80000aa29970;

   5: fn 0xffffffff88989d30 - pc 0xffffffff88989d59 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3d7; rbp                0x0; rsp 0xffff80000aa29a00;

   6: fn 0xffffffff8898b460 - pc 0xffffffff8898b49e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3d7; rbp                0x0; rsp 0xffff80000aa29a40;

   7: fn 0xffffffff8874ef00 - pc 0xffffffff8874ef5f / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3d7; rbp                0x0; rsp 0xffff80000aa29a80;

   8: fn 0xffffffff887440d0 - pc 0xffffffff88744104 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3d7; rbp                0x0; rsp 0xffff80000aa29ae0;

   9: fn 0xffffffff8874bb60 - pc 0xffffffff8874bb7f / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3d7; rbp                0x0; rsp 0xffff80000aa29b80;

  10: fn 0xffffffff880cce50 - pc 0xffffffff880ccee5 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3d7; rbp                0x0; rsp 0xffff80000aa29ba0;

  11: fn 0xffffffff8843b8b0 - pc 0xffffffff88471e17 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3d7; rbp                0x0; rsp 0xffff80000aa29f30;

  12: fn 0xffffffff880fc950 - pc 0xffffffff880fc9de / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3d7; rbp                0x0; rsp 0xffff80000aa403d0;

  13: fn 0xffffffff88530380 - pc 0xffffffff88530eff / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3d7; rbp                0x0; rsp 0xffff80000aa40570;

  14: fn 0xffffffff88396fa0 - pc 0xffffffff88396fae / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3d7; rbp                0x0; rsp 0xffff80000aa40f90;

  15: fn 0xffffffff88859c20 - pc 0xffffffff88859c36 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3d7; rbp                0x0; rsp 0xffff80000aa40fb0;

  16: fn 0xffffffff888349d0 - pc 0xffffffff88834a39 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3d7; rbp                0x0; rsp 0xffff80000aa40fd0;


     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883d3d7; rbp                0x0; rsp 0xffff80000aa41000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:113
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) 73ctk84we8vz1kw7hv2sr00er:?
(  4) ??:?
(  5) alloc.4436a8b0cd505e33-cgu.0:?
(  6) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/raw_vec.rs:594
(  7) ??:?
(  8) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/vec/spec_from_elem.rs:52
(  9) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/vec/mod.rs:2737
( 10) /root/asterinas/kernel/src/syscall/getrandom.rs:15
( 11) /root/asterinas/kernel/src/syscall/mod.rs:171
( 12) /root/asterinas/kernel/src/syscall/mod.rs:328
( 13) /root/asterinas/kernel/src/thread/task.rs:69
( 14) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 15) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 16) /root/asterinas/ostd/src/task/mod.rs:175
make: *** [Makefile:172: run] Error 1
```

# syscall-utimes-2bda8d1-timespec_t::from-assert
Reachable Assertion Panic in `timespec_t::from()` by negative `tv_sec` value

### Reference
https://github.com/asterinas/asterinas/issues/1348

### Describe the bug
There is a reachable assertion in `timespec_t::from()` at kernel/src/time/mod.rs:50 when call syscall `utimes` with negative `tv_sec` value.

https://github.com/asterinas/asterinas/blob/2bda8d17c42652c6c9276a83e88600567b55fea6/kernel/src/time/mod.rs#L46-L53

### To Reproduce
1. Compile a program which calls `utimes`: 
```C
#define _GNU_SOURCE
#include <stdio.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) {
  struct timeval times[2];
  times[0].tv_sec = -1;
  times[0].tv_usec = 10;
  times[1].tv_sec = -1;
  times[1].tv_usec = 10;

  syscall(SYS_utimes, "/dev/null", times);
  perror("utimes");

  return 0;
}
```
2. Run the compiled program in Asterinas.

### Expected behavior

Asterinas reports panic and is terminated.

### Environment
- Official docker asterinas/asterinas:0.8.1
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 2bda8d1

### Logs
```
~ # /root/utimes.c 
panicked at /root/asterinas/kernel/src/time/mod.rs:50:9:
assertion failed: sec >= 0
Printing stack trace:
   1: fn 0xffffffff88860d60 - pc 0xffffffff88860dca / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b1a7; rbp                0x0; rsp 0xffff80000b029770;

   2: fn 0xffffffff88860b70 - pc 0xffffffff88860cc0 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b1a7; rbp                0x0; rsp 0xffff80000b0297d0;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b1a7; rbp                0x0; rsp 0xffff80000b029920;

   4: fn 0xffffffff889d2bf0 - pc 0xffffffff889d2c72 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b1a7; rbp                0x0; rsp 0xffff80000b029930;

   5: fn 0xffffffff889d2d90 - pc 0xffffffff889d2dd0 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b1a7; rbp                0x0; rsp 0xffff80000b0299c0;

   6: fn 0xffffffff88344840 - pc 0xffffffff883448a6 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b1a7; rbp                0x0; rsp 0xffff80000b029a30;

   7: fn 0xffffffff8859d700 - pc 0xffffffff8859d9ab / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b1a7; rbp                0x0; rsp 0xffff80000b029a70;

   8: fn 0xffffffff8859bd60 - pc 0xffffffff8859bdc4 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b1a7; rbp                0x0; rsp 0xffff80000b029c90;

   9: fn 0xffffffff885507b0 - pc 0xffffffff8857df4d / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b1a7; rbp                0x0; rsp 0xffff80000b029f70;

  10: fn 0xffffffff883f79e0 - pc 0xffffffff883f7a6e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b1a7; rbp                0x0; rsp 0xffff80000b040410;

  11: fn 0xffffffff8848e0d0 - pc 0xffffffff8848ec76 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b1a7; rbp                0x0; rsp 0xffff80000b0405b0;

  12: fn 0xffffffff8820f570 - pc 0xffffffff8820f57e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b1a7; rbp                0x0; rsp 0xffff80000b040f90;

  13: fn 0xffffffff88898970 - pc 0xffffffff88898986 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b1a7; rbp                0x0; rsp 0xffff80000b040fb0;

  14: fn 0xffffffff88888810 - pc 0xffffffff88888879 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b1a7; rbp                0x0; rsp 0xffff80000b040fd0;


     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b1a7; rbp                0x0; rsp 0xffff80000b041000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:113
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) cbtrlxuzeok8ueszfa474zvwu:?
(  4) ??:?
(  5) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/panicking.rs:220
(  6) ??:?
(  7) /root/asterinas/kernel/src/syscall/utimens.rs:195
(  8) /root/asterinas/kernel/src/syscall/utimens.rs:71
(  9) /root/asterinas/kernel/src/syscall/mod.rs:164
( 10) /root/asterinas/kernel/src/syscall/mod.rs:328
( 11) /root/asterinas/kernel/src/thread/task.rs:69
( 12) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 13) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 14) /root/asterinas/ostd/src/task/mod.rs:175
make: *** [Makefile:173: run] Error 1
```

# syscall-utimes-2bda8d1-set_timeout-assert
Reachable Panic in `set_timeout()` by negative `tv_sec` value

### Reference
https://github.com/asterinas/asterinas/issues/1349

### Describe the bug
There is a reachable panic in `set_timeout()` at kernel/src/time/core/timer.rs:80 when call syscall `select` with negative `tv_sec` value.

https://github.com/asterinas/asterinas/blob/2bda8d17c42652c6c9276a83e88600567b55fea6/kernel/src/time/core/timer.rs#L80

```rust
1126 #[stable(feature = "duration", since = "1.3.0")]
1127 impl Add for Duration {
1128     type Output = Duration;
1129 
1130     #[inline]
1131     fn add(self, rhs: Duration) -> Duration {
1132         self.checked_add(rhs).expect("overflow when adding durations")
1133     }
1134 }
```

### To Reproduce
1. Compile a program which calls `select`: 
```C
#define _GNU_SOURCE
#include <stdio.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) {
  struct timeval times[2];
  times[0].tv_sec = -1;
  times[0].tv_usec = 10;
  times[1].tv_sec = -1;
  times[1].tv_usec = 10;

  syscall(SYS_select, NULL, NULL, NULL, NULL, times);
  perror("select");

  return 0;
}

```
2. Run the compiled program in Asterinas.

### Expected behavior

Asterinas reports panic and is terminated.

### Environment
- Official docker asterinas/asterinas:0.8.1
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main 2bda8d1

### Logs
```
~ # /root/select.c 
panicked at /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/time.rs:1132:31:
overflow when adding durations
Printing stack trace:
   1: fn 0xffffffff88860cf0 - pc 0xffffffff88860d5a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b137; rbp                0x0; rsp 0xffff80000b2a7f60;

   2: fn 0xffffffff88860b00 - pc 0xffffffff88860c50 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b137; rbp                0x0; rsp 0xffff80000b2a7fc0;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b137; rbp                0x0; rsp 0xffff80000b2a8110;

   4: fn 0xffffffff889d2b80 - pc 0xffffffff889d2c02 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b137; rbp                0x0; rsp 0xffff80000b2a8120;

   5: fn 0xffffffff889d2e10 - pc 0xffffffff889d2ea0 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b137; rbp                0x0; rsp 0xffff80000b2a81b0;

   6: fn 0xffffffff889bc590 - pc 0xffffffff889bc5b3 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b137; rbp                0x0; rsp 0xffff80000b2a8250;

   7: fn 0xffffffff8838ea60 - pc 0xffffffff8838eab1 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b137; rbp                0x0; rsp 0xffff80000b2a8270;

   8: fn 0xffffffff883f9820 - pc 0xffffffff883f9858 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b137; rbp                0x0; rsp 0xffff80000b2a82c0;

   9: fn 0xffffffff883f5d70 - pc 0xffffffff883f5e29 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b137; rbp                0x0; rsp 0xffff80000b2a82f0;

  10: fn 0xffffffff8847c600 - pc 0xffffffff8847c835 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b137; rbp                0x0; rsp 0xffff80000b2a8470;

  11: fn 0xffffffff88477450 - pc 0xffffffff88477657 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b137; rbp                0x0; rsp 0xffff80000b2a85d0;

  12: fn 0xffffffff88476f10 - pc 0xffffffff88476faa / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b137; rbp                0x0; rsp 0xffff80000b2a86d0;

  13: fn 0xffffffff88175bf0 - pc 0xffffffff88175c52 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b137; rbp                0x0; rsp 0xffff80000b2a8750;

  14: fn 0xffffffff88175a30 - pc 0xffffffff88175a8e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b137; rbp                0x0; rsp 0xffff80000b2a87a0;

  15: fn 0xffffffff88119af0 - pc 0xffffffff88119e37 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b137; rbp                0x0; rsp 0xffff80000b2a8850;

  16: fn 0xffffffff884d43d0 - pc 0xffffffff884d46ee / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b137; rbp                0x0; rsp 0xffff80000b2a8b50;

  17: fn 0xffffffff884d3660 - pc 0xffffffff884d3eee / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b137; rbp                0x0; rsp 0xffff80000b2a8f20;

  18: fn 0xffffffff884d3430 - pc 0xffffffff884d3501 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b137; rbp                0x0; rsp 0xffff80000b2a9e40;

  19: fn 0xffffffff882fe4e0 - pc 0xffffffff88307573 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b137; rbp                0x0; rsp 0xffff80000b2a9f70;

  20: fn 0xffffffff88183d20 - pc 0xffffffff88183dae / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b137; rbp                0x0; rsp 0xffff80000b2c0410;

  21: fn 0xffffffff88194640 - pc 0xffffffff881951e6 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b137; rbp                0x0; rsp 0xffff80000b2c05b0;

  22: fn 0xffffffff88565200 - pc 0xffffffff8856520e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b137; rbp                0x0; rsp 0xffff80000b2c0f90;

  23: fn 0xffffffff88898900 - pc 0xffffffff88898916 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b137; rbp                0x0; rsp 0xffff80000b2c0fb0;

  24: fn 0xffffffff888887a0 - pc 0xffffffff88888809 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b137; rbp                0x0; rsp 0xffff80000b2c0fd0;


     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff8883b137; rbp                0x0; rsp 0xffff80000b2c1000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:113
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) cgzpxvl9zvceljk9gh76mj70a:?
(  4) ??:?
(  5) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/panicking.rs:271
(  6) ??:?
(  7) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/option.rs:923
(  8) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/time.rs:1133
(  9) /root/asterinas/kernel/src/time/core/timer.rs:80
( 10) /root/asterinas/kernel/src/time/wait.rs:62
( 11) /root/asterinas/kernel/src/process/signal/pause.rs:111
( 12) /root/asterinas/kernel/src/process/signal/pause.rs:59
( 13) /root/asterinas/kernel/src/process/signal/poll.rs:218
( 14) /root/asterinas/kernel/src/process/signal/poll.rs:171
( 15) /root/asterinas/kernel/src/syscall/poll.rs:88
( 16) /root/asterinas/kernel/src/syscall/select.rs:136
( 17) /root/asterinas/kernel/src/syscall/select.rs:67
( 18) /root/asterinas/kernel/src/syscall/select.rs:26
( 19) /root/asterinas/kernel/src/syscall/mod.rs:198
( 20) /root/asterinas/kernel/src/syscall/mod.rs:328
( 21) /root/asterinas/kernel/src/thread/task.rs:69
( 22) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 23) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 24) /root/asterinas/ostd/src/task/mod.rs:175
make: *** [Makefile:173: run] Error 1
```

# syscall-openat-ae4ac38-FsResolver::create_new_file-unwrap
Reachable unwrap in `FsResolver::create_new_file()`

### Reference
https://github.com/asterinas/asterinas/issues/

### Describe the bug
There is a reachable panic in `FsResolver::create_new_file()` at kernel/src/fs/fs_resolver.rs:134 when call syscall `openat`.

https://github.com/asterinas/asterinas/blob/ae4ac384713e63232b74915593ebdef680049d31/kernel/src/fs/fs_resolver.rs#L134

### To Reproduce
1. Compile a program which calls `openat`: 
```C
#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

int main() {
  FILE *fp = fopen("/proc/sys", "r");
  int dfd = fileno(fp);
  fcntl(dfd, F_SETFL, 0x44c00);
  const char *filename = ".//dev/random";
  int flags = 0x81040;
  mode_t mode = 0666;
  int fd = syscall(SYS_openat, dfd, filename, flags, mode);

  perror("openat");
  return 0;
}
```
2. Run the compiled program in Asterinas.

### Expected behavior

Asterinas reports panic and is terminated.

### Environment
- Official docker asterinas/asterinas:0.8.3
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main ae4ac38

### Logs
```
panicked at /root/asterinas/kernel/src/fs/fs_resolver.rs:134:42:
called `Option::unwrap()` on a `None` value
Printing stack trace:
   1: fn 0xffffffff887859c0 - pc 0xffffffff88785a2a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88864a27; rbp                0x0; rsp 0xffff8000017a9310;

   2: fn 0xffffffff887857d0 - pc 0xffffffff88785920 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88864a27; rbp                0x0; rsp 0xffff8000017a9370;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88864a27; rbp                0x0; rsp 0xffff8000017a94c0;

   4: fn 0xffffffff889a20f0 - pc 0xffffffff889a2172 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88864a27; rbp                0x0; rsp 0xffff8000017a94d0;

   5: fn 0xffffffff889a2290 - pc 0xffffffff889a22d0 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88864a27; rbp                0x0; rsp 0xffff8000017a9560;

   6: fn 0xffffffff8898bae0 - pc 0xffffffff8898baf5 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88864a27; rbp                0x0; rsp 0xffff8000017a95d0;

   7: fn 0xffffffff885e4cb0 - pc 0xffffffff885e4d8e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88864a27; rbp                0x0; rsp 0xffff8000017a95e0;

   8: fn 0xffffffff885e4080 - pc 0xffffffff885e4491 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88864a27; rbp                0x0; rsp 0xffff8000017a97e0;

   9: fn 0xffffffff8811fe70 - pc 0xffffffff881206c0 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88864a27; rbp                0x0; rsp 0xffff8000017a9a50;

  10: fn 0xffffffff88063b80 - pc 0xffffffff88091cef / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88864a27; rbp                0x0; rsp 0xffff8000017a9f30;

  11: fn 0xffffffff88061d50 - pc 0xffffffff88061dde / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88864a27; rbp                0x0; rsp 0xffff8000017c03d0;

  12: fn 0xffffffff88402130 - pc 0xffffffff88402cf6 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88864a27; rbp                0x0; rsp 0xffff8000017c0570;

  13: fn 0xffffffff88476190 - pc 0xffffffff8847619e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88864a27; rbp                0x0; rsp 0xffff8000017c0f90;

  14: fn 0xffffffff887ad5c0 - pc 0xffffffff887ad5d6 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88864a27; rbp                0x0; rsp 0xffff8000017c0fb0;

  15: fn 0xffffffff887ef6e0 - pc 0xffffffff887ef749 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88864a27; rbp                0x0; rsp 0xffff8000017c0fd0;


     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88864a27; rbp                0x0; rsp 0xffff8000017c1000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:113
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) ezxtz4txsqany8s5wl6db2xrn:?
(  4) ??:?
(  5) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/panicking.rs:220
(  6) ??:?
(  7) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/option.rs:960
(  8) /root/asterinas/kernel/src/fs/fs_resolver.rs:69
(  9) /root/asterinas/kernel/src/syscall/open.rs:34
( 10) /root/asterinas/kernel/src/syscall/mod.rs:178
( 11) /root/asterinas/kernel/src/syscall/mod.rs:328
( 12) /root/asterinas/kernel/src/thread/task.rs:69
( 13) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 14) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 15) /root/asterinas/ostd/src/task/mod.rs:192
make: *** [Makefile:173: run] Error 1
```

# syscall-???-c4cb0f1-wake_robust_list-unwrap
Reachable unwrap in `wake_robust_list()`

### Reference
https://github.com/asterinas/asterinas/issues/1391

### Describe the bug
There is a reachable unwrap panic in `wake_robust_list()` at kernel/src/process/posix_thread/exit.rs:62 caused by invalid `futex_addr` (i.e., 0xffff) which is checked by `check_vaddr()`.

https://github.com/asterinas/asterinas/blob/c4cb0f1aefad470af18ee878f7959acedaa83768/kernel/src/process/posix_thread/exit.rs#L52-L62

### To Reproduce
I'm trying to construct a minimal PoC.

### Expected behavior

Asterinas reports panic and is terminated.

### Environment
- Official docker asterinas/asterinas:0.8.3
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main c4cb0f1

### Logs
```
panicked at /root/asterinas/kernel/src/process/posix_thread/exit.rs:62:44:
called `Result::unwrap()` on an `Err` value: Error { errno: EFAULT, msg: Some("Bad user space pointer specified") }
Printing stack trace:
   1: fn 0xffffffff8883c310 - pc 0xffffffff8883c37a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887c4937; rbp                0x0; rsp 0xffff800003429070;

   2: fn 0xffffffff8883c120 - pc 0xffffffff8883c270 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887c4937; rbp                0x0; rsp 0xffff8000034290d0;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887c4937; rbp                0x0; rsp 0xffff800003429220;

   4: fn 0xffffffff889802b0 - pc 0xffffffff88980332 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887c4937; rbp                0x0; rsp 0xffff800003429230;

   5: fn 0xffffffff8895f2e0 - pc 0xffffffff8895f40b / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887c4937; rbp                0x0; rsp 0xffff8000034292c0;

   6: fn 0xffffffff88156ad0 - pc 0xffffffff88156ec5 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887c4937; rbp                0x0; rsp 0xffff8000034293c0;

   7: fn 0xffffffff881563f0 - pc 0xffffffff8815651e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887c4937; rbp                0x0; rsp 0xffff8000034295f0;

   8: fn 0xffffffff88376a50 - pc 0xffffffff88377aeb / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887c4937; rbp                0x0; rsp 0xffff800003429830;

   9: fn 0xffffffff882f4310 - pc 0xffffffff882f4349 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887c4937; rbp                0x0; rsp 0xffff800003429ec0;

  10: fn 0xffffffff884f2740 - pc 0xffffffff8851eb55 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887c4937; rbp                0x0; rsp 0xffff800003429f00;

  11: fn 0xffffffff883783b0 - pc 0xffffffff8837843e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887c4937; rbp                0x0; rsp 0xffff8000034403a0;

  12: fn 0xffffffff8824ab40 - pc 0xffffffff8824b791 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887c4937; rbp                0x0; rsp 0xffff800003440540;

  13: fn 0xffffffff882775c0 - pc 0xffffffff882775ce / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887c4937; rbp                0x0; rsp 0xffff800003440f90;

  14: fn 0xffffffff887c8df0 - pc 0xffffffff887c8e06 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887c4937; rbp                0x0; rsp 0xffff800003440fb0;

  15: fn 0xffffffff88871fe0 - pc 0xffffffff88872045 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887c4937; rbp                0x0; rsp 0xffff800003440fd0;


     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887c4937; rbp                0x0; rsp 0xffff800003441000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:113
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) cj0to132njfaiuer8top26r2s:?
(  4) ??:?
(  5) ??:?
(  6) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/result.rs:1102
(  7) /root/asterinas/kernel/src/process/posix_thread/exit.rs:35
(  8) /root/asterinas/kernel/src/process/exit.rs:26
(  9) /root/asterinas/kernel/src/syscall/exit_group.rs:13
( 10) /root/asterinas/kernel/src/syscall/mod.rs:157
( 11) /root/asterinas/kernel/src/syscall/mod.rs:328
( 12) /root/asterinas/kernel/src/thread/task.rs:70
( 13) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 14) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 15) /root/asterinas/ostd/src/task/mod.rs:173
make: *** [Makefile:174: run] Error 1
```

# syscall-???-c4cb0f1-use_alternate_signal_stack-integer_overflow
Integer overflow in `use_alternate_signal_stack()`

### Reference
https://github.com/asterinas/asterinas/issues/

### Describe the bug
There is a integer overflow in `use_alternate_signal_stack()` at kernel/src/process/signal/mod.rs:257 .

https://github.com/asterinas/asterinas/blob/c4cb0f1aefad470af18ee878f7959acedaa83768/kernel/src/process/signal/mod.rs#L257

### To Reproduce
I'm trying to construct a minimal PoC.

### Expected behavior

Asterinas reports panic and is terminated.

### Environment
- Official docker asterinas/asterinas:0.8.3
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main c4cb0f1

### Logs
```
panicked at /root/asterinas/kernel/src/process/signal/mod.rs:257:25:
attempt to add with overflow
Printing stack trace:
   1: fn 0xffffffff8879f5f0 - pc 0xffffffff8879f65a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887a1387; rbp                0x0; rsp 0xffff8000068bea20;

   2: fn 0xffffffff8879f400 - pc 0xffffffff8879f550 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887a1387; rbp                0x0; rsp 0xffff8000068bea80;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887a1387; rbp                0x0; rsp 0xffff8000068bebd0;

   4: fn 0xffffffff88980860 - pc 0xffffffff889808e2 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887a1387; rbp                0x0; rsp 0xffff8000068bebe0;

   5: fn 0xffffffff8895dd70 - pc 0xffffffff8895dd9e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887a1387; rbp                0x0; rsp 0xffff8000068bec70;

   6: fn 0xffffffff885ce740 - pc 0xffffffff885ce8c8 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887a1387; rbp                0x0; rsp 0xffff8000068becc0;

   7: fn 0xffffffff885cd260 - pc 0xffffffff885cdbcc / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887a1387; rbp                0x0; rsp 0xffff8000068bed80;

   8: fn 0xffffffff885cc000 - pc 0xffffffff885cd122 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887a1387; rbp                0x0; rsp 0xffff8000068bfce0;

   9: fn 0xffffffff880c8d00 - pc 0xffffffff880c99dd / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887a1387; rbp                0x0; rsp 0xffff8000068c0540;

  10: fn 0xffffffff8816f210 - pc 0xffffffff8816f21e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887a1387; rbp                0x0; rsp 0xffff8000068c0f90;

  11: fn 0xffffffff888333e0 - pc 0xffffffff888333f6 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887a1387; rbp                0x0; rsp 0xffff8000068c0fb0;

  12: fn 0xffffffff887e1f70 - pc 0xffffffff887e1fd5 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887a1387; rbp                0x0; rsp 0xffff8000068c0fd0;


     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff887a1387; rbp                0x0; rsp 0xffff8000068c1000;
```

# syscall-rt_sigsuspend-f793259-sys_rt_sigsuspend-assert
Readchable assert in `sys_rt_sigsuspend()` by invalid `sigmask_size`

### Reference
https://github.com/asterinas/asterinas/issues/

### Describe the bug
There is a reachable assertion in `sys_rt_sigsuspend()` at kernel/src/syscall/rt_sigsuspend.rs:25 by invalid `sigmask_size`.

https://github.com/asterinas/asterinas/blob/ab8b6afee5e440266233db215d4db73066d541eb/kernel/src/syscall/rt_sigsuspend.rs#L25

It should directly return instead of assert:  
https://elixir.bootlin.com/linux/v6.10.5/source/kernel/signal.c#L4720

### To Reproduce
1. Compile a program which calls `rt_sigsuspend`: 
```C
#define _GNU_SOURCE
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

int main() {
  sigset_t mask;
  syscall(SYS_rt_sigsuspend, &mask, 9);
  perror("syscall rt_sigsuspend");

  return 0;
}
```
2. Run the compiled program in Asterinas.

### Expected behavior

Asterinas reports panic and is terminated.

### Environment
- Official docker asterinas/asterinas:0.8.3
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
- Asterinas version: main f793259

### Logs
```
~ # /root/rt_sigsuspend.c 
panicked at /root/asterinas/kernel/src/syscall/rt_sigsuspend.rs:25:5:
assertion failed: sigmask_size == core::mem::size_of::<SigMask>()
Printing stack trace:
   1: fn 0xffffffff8884cc10 - pc 0xffffffff8884cc7a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88832ff7; rbp                0x0; rsp 0xffff80007f1298d0;

   2: fn 0xffffffff8884ca20 - pc 0xffffffff8884cb70 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88832ff7; rbp                0x0; rsp 0xffff80007f129930;

   3: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88832ff7; rbp                0x0; rsp 0xffff80007f129a80;

   4: fn 0xffffffff889955b0 - pc 0xffffffff88995632 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88832ff7; rbp                0x0; rsp 0xffff80007f129a90;

   5: fn 0xffffffff88995750 - pc 0xffffffff88995790 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88832ff7; rbp                0x0; rsp 0xffff80007f129b20;

   6: fn 0xffffffff884d7100 - pc 0xffffffff884d7476 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88832ff7; rbp                0x0; rsp 0xffff80007f129b90;

   7: fn 0xffffffff885b5e90 - pc 0xffffffff885d9a88 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88832ff7; rbp                0x0; rsp 0xffff80007f129f00;

   8: fn 0xffffffff885b5690 - pc 0xffffffff885b571e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88832ff7; rbp                0x0; rsp 0xffff80007f1403a0;

   9: fn 0xffffffff884d7950 - pc 0xffffffff884d85a1 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88832ff7; rbp                0x0; rsp 0xffff80007f140540;

  10: fn 0xffffffff8804cc90 - pc 0xffffffff8804cc9e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88832ff7; rbp                0x0; rsp 0xffff80007f140f90;

  11: fn 0xffffffff887d8970 - pc 0xffffffff887d8986 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88832ff7; rbp                0x0; rsp 0xffff80007f140fb0;

  12: fn 0xffffffff888865b0 - pc 0xffffffff88886615 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88832ff7; rbp                0x0; rsp 0xffff80007f140fd0;


     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi                0x1; rdi 0xffffffff88832ff7; rbp                0x0; rsp 0xffff80007f141000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panicking.rs:113
(  2) /root/asterinas/ostd/src/panicking.rs:59
(  3) 9rjb2mawuypgylucnupzvapu0:?
(  4) ??:?
(  5) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/panicking.rs:220
(  6) /root/asterinas/kernel/src/syscall/rt_sigsuspend.rs:26
(  7) /root/asterinas/kernel/src/syscall/mod.rs:164
(  8) /root/asterinas/kernel/src/syscall/mod.rs:328
(  9) /root/asterinas/kernel/src/thread/task.rs:70
( 10) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:79
( 11) /root/.rustup/toolchains/nightly-2024-06-20-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2077
( 12) /root/asterinas/ostd/src/task/mod.rs:173
make: *** [Makefile:184: run] Error 1
```

# syscall-???-96a153d-Pages::with-panic
Misuse of Mutex/RwMutex breaks atomic mode by `Pages::with` and `handle_page_faults_around`

### Reference
https://github.com/asterinas/asterinas/issues/

### Describe the bug
In `Pages::with` Asterinas triggered a panic in

https://github.com/asterinas/asterinas/blob/7e2e9cebf69ea193ec72cad05ac46c2555919c04/kernel/src/vm/vmo/mod.rs#L157

And a spinlock is held in `handle_page_faults_around`
https://github.com/asterinas/asterinas/blob/7e2e9cebf69ea193ec72cad05ac46c2555919c04/kernel/src/vm/vmar/vm_mapping.rs#L337

### To Reproduce
Trying to construct a PoC...
Could be related to `pwritev`.

### Expected behavior
Asterinas shall not break atomic mode and panic.

### Environment
- Official docker asterinas/asterinas:0.9.3
- Intel(R) Xeon(R) Gold 6230R CPU @ 2.10GHz
- Asterinas version: main 96a153d

### Logs
```
Printing stack trace:
   1: fn 0xffffffff8885aba0 - pc 0xffffffff8885ac0a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa07b00;

   2: fn 0xffffffff8804a380 - pc 0xffffffff8804b09c / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa07b60;

   3: fn 0xffffffff8804a370 - pc 0xffffffff8804a37a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa08180;

   4: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa08190;

   5: fn 0xffffffff889d9e20 - pc 0xffffffff889d9e64 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa081a0;

   6: fn 0xffffffff888b8350 - pc 0xffffffff888b84b1 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa081e0;

   7: fn 0xffffffff887f26d0 - pc 0xffffffff887f26fc / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa082c0;

   8: fn 0xffffffff888a1800 - pc 0xffffffff888a193b / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa083f0;

   9: fn 0xffffffff888df870 - pc 0xffffffff888df8aa / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa08480;

  10: fn 0xffffffff888d2910 - pc 0xffffffff888d294f / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa084d0;

  11: fn 0xffffffff888d2870 - pc 0xffffffff888d2882 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa08500;

  12: fn 0xffffffff882c8df0 - pc 0xffffffff882c8f3a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa08510;

  13: fn 0xffffffff882d40c0 - pc 0xffffffff882d4200 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa085a0;

  14: fn 0xffffffff88067f60 - pc 0xffffffff88067f70 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa08680;

  15: fn 0xffffffff8852c8b0 - pc 0xffffffff8852c92c / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa08690;

  16: fn 0xffffffff88098e80 - pc 0xffffffff88098e8a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa08750;

  17: fn 0xffffffff8852ec20 - pc 0xffffffff8852ec32 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa08760;

  18: fn 0xffffffff885195e0 - pc 0xffffffff885195ea / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa08770;

  19: fn 0xffffffff885158b0 - pc 0xffffffff88515a52 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa08780;

  20: fn 0xffffffff885145e0 - pc 0xffffffff8851480a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa08dc0;

  21: fn 0xffffffff8851b1b0 - pc 0xffffffff8851b557 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa09440;

  22: fn 0xffffffff88521ac0 - pc 0xffffffff88521b44 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa095b0;

  23: fn 0xffffffff8821c450 - pc 0xffffffff8821c47c / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa09650;

  24: fn 0xffffffff8821c2d0 - pc 0xffffffff8821c41e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa097b0;

  25: fn 0xffffffff88520320 - pc 0xffffffff885203ad / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa09890;

  26: fn 0xffffffff8889c3e0 - pc 0xffffffff8889c426 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa09900;

  27: fn 0xffffffff888d96c0 - pc 0xffffffff888d97fe / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa09940;

  28: fn 0xffffffff888d8ed0 - pc 0xffffffff888d90c2 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa09b70;

  29: fn                0x0 - pc 0xffffffff888af6c6 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffdfffffa07b08; rdi 0xffffffff88845ba7; rbp                0x0; rsp 0xffffdfffffa0a0a0;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panic.rs:107
(  2) /root/asterinas/kernel/src/thread/oops.rs:116
(  3) ??:?
(  4) ept1839qj6y3dcbaooc51x16d:?
(  5) ??:?
(  6) ??:?
(  7) /root/asterinas/ostd/src/task/processor.rs:46
(  8) /root/asterinas/ostd/src/task/scheduler/mod.rs:240
(  9) /root/asterinas/ostd/src/task/scheduler/mod.rs:121
( 10) /root/asterinas/ostd/src/sync/wait.rs:271
( 11) /root/asterinas/ostd/src/sync/wait.rs:199
( 12) /root/asterinas/ostd/src/sync/wait.rs:226
( 13) /root/asterinas/ostd/src/sync/wait.rs:81
( 14) /root/asterinas/ostd/src/sync/mutex.rs:37
( 15) /root/asterinas/kernel/src/vm/vmo/mod.rs:157
( 16) /root/asterinas/kernel/src/vm/vmo/mod.rs:372
( 17) /root/asterinas/kernel/src/vm/vmo/mod.rs:435
( 18) /root/asterinas/kernel/src/vm/vmar/vm_mapping.rs:898
( 19) /root/asterinas/kernel/src/vm/vmar/vm_mapping.rs:341
( 20) /root/asterinas/kernel/src/vm/vmar/vm_mapping.rs:220
( 21) /root/asterinas/kernel/src/vm/vmar/mod.rs:319
( 22) /root/asterinas/kernel/src/vm/vmar/static_cap.rs:176
( 23) /root/asterinas/kernel/src/thread/exception.rs:64
( 24) /root/asterinas/kernel/src/thread/exception.rs:56
( 25) /root/asterinas/kernel/src/vm/vmar/mod.rs:728
( 26) /root/asterinas/ostd/src/mm/vm_space.rs:142
( 27) /root/asterinas/ostd/src/arch/x86/trap/mod.rs:269
( 28) /root/asterinas/ostd/src/arch/x86/trap/mod.rs:237
( 29) ??:?
```

# syscall-???-7e2e9ce-Logger::log-deadlock
Deadlock in `Logger::log` when triggering multiple times of page fault

### Reference
https://github.com/asterinas/asterinas/issues/1534

### Describe the bug
Asterinas panic when triggering multiple times of page fault and then a deadlock happens.

Asterinas hangs because of the lock in `Logger::log` at `src/logger.rs:40`:
https://github.com/asterinas/asterinas/blob/7e2e9cebf69ea193ec72cad05ac46c2555919c04/ostd/src/logger.rs#L40

### To Reproduce
1. compile a program which trigger page fault:
```C
#include <unistd.h>

int main() {
  for (int i = 0; i < 5; i++)
    fork();
  int a = *((const char *)0xdeadbeaf);
}
```
2. run the program multiple times (there is a chance that you cannot reproduce this time, close and reopen the Asterinas and run the cmd again):
```bash
$ for i in $(seq 1 200); do ./root/pagefault.c; done
```

### Expected behavior
Asterinas shall not panic when multiple times of page fault, and shall not hang when dumping stack trace.

### Environment
- Official docker asterinas/asterinas:0.9.4
- Intel(R) Xeon(R) Gold 6230R CPU @ 2.10GHz
- Asterinas version: main 7e2e9ce

### Logs
```
~ # for i in $(seq 1 200); do ./root/pagefault.c; done
Segmentation fault
Segmentation fault
Segmentation fault
Segmentation fault
Segmentation fault
Segmentation fault
Segmentation fault
Segmentation fault
Segmentation fault
Segmentation fault
Segmentation fault
Segmentation fault
Segmentation fault
Segmentation fault
Segmentation fault
Segmentation fault
Segmentation fault
Segmentation fault
Segmentation fault
Segmentation fault
Segmentation fault
Segmentation fault
Segmentation fault
Segmentation fault
Segmentation fault
Segmentation fault
Segmentation fault
Segmentation fault
Segmentation fault
Segmentation fault
Printing stack trace:
```

The GDB log is:
```
core::sync::atomic::AtomicBool::compare_exchange (self=0x20101887ac0f6, current=225, new=136, success=255, 
    failure=255)
    at /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/sync/atomic.rs:792
792         pub fn compare_exchange(
(gdb) bt
#0  core::sync::atomic::AtomicBool::compare_exchange (self=0x20101887ac0f6, current=225, new=136, success=255, 
    failure=255)
    at /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/sync/atomic.rs:792
#1  0xffffffff8882f5c0 in ostd::sync::spin::SpinLock<(), ostd::sync::spin::LocalIrqDisabled>::try_acquire_lock<(), ostd::sync::spin::LocalIrqDisabled> (
    self=0xffffffff88e152c3 <<ostd::logger::Logger as log::Log>::log::RECORD_LOCK>) at src/sync/spin.rs:157
#2  0xffffffff8882f148 in ostd::sync::spin::SpinLock<(), ostd::sync::spin::LocalIrqDisabled>::acquire_lock<(), ostd::sync::spin::LocalIrqDisabled> (self=0xffffffff88e152c3 <<ostd::logger::Logger as log::Log>::log::RECORD_LOCK>)
    at src/sync/spin.rs:151
#3  0xffffffff8882e96f in ostd::sync::spin::SpinLock<(), ostd::sync::spin::LocalIrqDisabled>::lock<(), ostd::sync::spin::LocalIrqDisabled> (self=0xffffffff88e152c3 <<ostd::logger::Logger as log::Log>::log::RECORD_LOCK>)
    at src/sync/spin.rs:114
#4  0xffffffff888b8685 in ostd::logger::{impl#0}::log (self=0x1, record=0xffffdfffffdbc780) at src/logger.rs:40
#5  0xffffffff8898476b in log::__private_api::log_impl (args=..., level=log::Level::Error, kvs=...)
    at src/__private_api.rs:61
#6  0xffffffff8897f566 in log::__private_api::log<()> (args=..., level=log::Level::Error, 
    target_module_path_and_loc=0xffffdfffffdbcb68, kvs=())
    at /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/log-0.4.22/src/__private_api.rs:72
#7  0xffffffff8804afe2 in aster_nix::thread::oops::panic_handler (info=0xffffdfffffdbce30)
    at src/thread/oops.rs:102
#8  0xffffffff8804a6da in aster_nix::thread::oops::__ostd_panic_handler (info=0xffffdfffffdbce30)
    at src/thread/oops.rs:82
#9  0xffffffff8804900a in aster_nix_osdk_bin::panic (info=0xffffdfffffdbce30) at src/main.rs:11
#10 0xffffffff889b4874 in core::panicking::panic_fmt (fmt=...) at src/panicking.rs:74
#11 0xffffffff888b465e in ostd::arch::x86::trap::handle_user_page_fault (f=0xffffdfffffdbd5b0, 
    page_fault_addr=24) at src/arch/x86/trap/mod.rs:280
#12 0xffffffff888b3c52 in ostd::arch::x86::trap::trap_handler (f=0xffffdfffffdbd5b0)
    at src/arch/x86/trap/mod.rs:237
#13 0xffffffff8888a266 in __from_kernel ()
#14 0x0000000000000000 in ?? ()
```


# syscall-???-7e2e9ce--hang
Program hangs when triggering page fault

### Reference
https://github.com/asterinas/asterinas/issues/

### Describe the bug
Program hangs in Asterinas when triggering page fault, and exits (or crashes) in Linux.

Asterinas fall into infinite loop in `user_task_entry()`:
https://github.com/asterinas/asterinas/blob/efd49a96e3eb83094f6bc93f2723ef376b29fabb/kernel/src/thread/task.rs#L64-L88

### To Reproduce
1. Compile a program which trigger page fault:
```C
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>

jmp_buf env;

void handle_sigsegv(int sig) {
  printf("Segmentation fault occurred!\n");
  longjmp(env, 1);
}

int main() {
  signal(SIGSEGV, handle_sigsegv);

  int fault_count = 0;
  const int max_faults = 10;

  while (fault_count < max_faults) {
    if (setjmp(env) == 0) {
      int *invalid_ptr = (int *)0xDEADBEEF;
      printf("Will trigger a segmentation fault in %x\n", invalid_ptr);
      *invalid_ptr = 42;
    } else {
      fault_count++;
      printf("Recovered from segmentation fault #%d\n", fault_count);
    }
  }

  printf("Completed after triggering %d segmentation faults.\n", fault_count);
  return 0;
}
```
2. Run the program, it hangs in Asterinas, and exits (or crashes) in Linux.

### Expected behavior
The program shall not hang in Asterinas.

### Environment
- Official docker asterinas/asterinas:0.9.4
- Intel(R) Xeon(R) Gold 6230R CPU @ 2.10GHz
- Asterinas version: main 7e2e9ce

### Logs
In debug mode, it repeated with
```
[32m[  1917.212][0m [93mWARN [0m: [39mpage fault handler failed: addr: 0xdeadbeef, err: Error { errno: EACCES, msg: Some("page fault addr is not in current vmar") }[0m
```

# syscall-fork-7ddfd42-drop_as_last-deadlock
Deadlock in `drop_as_last()`

### Reference
https://github.com/asterinas/asterinas/issues/

### Describe the bug
There is a deadlock in `drop_as_last()` at src/mm/page/meta.rs:156 when call system call `fork` lots of times.
https://github.com/asterinas/asterinas/blob/7ddfd42baa210656127044995d8707fde74fab4d/ostd/src/mm/page/meta.rs#L156

### To Reproduce
1. Compile a program which calls system call `fork` lots of times:
```C
#include <stdio.h>
#include <unistd.h>

int main() {
  for (int i = 0; i < 2000; i++) {
    printf("forking %d\n", i);
    fork();
  }
}
```
2. Run the program, Asterinas will hang.

### Expected behavior
The program shall not hang in Asterinas.

### Environment
- Official docker asterinas/asterinas:0.9.4
- Intel(R) Xeon(R) Gold 6230R CPU @ 2.10GHz
- Asterinas version: main 7ddfd42

### Logs
The GDB log:
```
core::sync::atomic::AtomicBool::compare_exchange (self=0xffffffff88d19b38 <ostd::mm::page::allocator::PAGE_ALLOCATOR>, current=false, new=true, 
    success=core::sync::atomic::Ordering::Acquire, failure=core::sync::atomic::Ordering::Relaxed)
    at /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/sync/atomic.rs:830
--Type <RET> for more, q to quit, c to continue without paging--
830                     Err(x) => Err(x != 0),
(gdb) bt
#0  core::sync::atomic::AtomicBool::compare_exchange (self=0xffffffff88d19b38 <ostd::mm::page::allocator::PAGE_ALLOCATOR>, current=false, new=true, 
    success=core::sync::atomic::Ordering::Acquire, failure=core::sync::atomic::Ordering::Relaxed)
    at /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/sync/atomic.rs:830
#1  0xffffffff887f3610 in ostd::sync::spin::SpinLock<ostd::mm::page::allocator::CountingFrameAllocator, ostd::sync::spin::PreemptDisabled>::try_acquire_lock<ostd::mm::page::allocator::CountingFrameAllocator, ostd::sync::spin::PreemptDisabled> (self=0xffffffff88d19b38 <ostd::mm::page::allocator::PAGE_ALLOCATOR>)
    at src/sync/spin.rs:157
#2  0xffffffff887f3578 in ostd::sync::spin::SpinLock<ostd::mm::page::allocator::CountingFrameAllocator, ostd::sync::spin::PreemptDisabled>::acquire_lock<ostd::mm::page::allocator::CountingFrameAllocator, ostd::sync::spin::PreemptDisabled> (self=0xffffffff88d19b38 <ostd::mm::page::allocator::PAGE_ALLOCATOR>)
    at src/sync/spin.rs:151
#3  0xffffffff887f2ebb in ostd::sync::spin::SpinLock<ostd::mm::page::allocator::CountingFrameAllocator, ostd::sync::spin::PreemptDisabled>::lock<ostd::mm::page::allocator::CountingFrameAllocator, ostd::sync::spin::PreemptDisabled> (self=0xffffffff88d19b38 <ostd::mm::page::allocator::PAGE_ALLOCATOR>)
    at src/sync/spin.rs:114
#4  0xffffffff8882d0f8 in ostd::mm::page::meta::drop_as_last<ostd::mm::page::meta::KernelStackMeta> (ptr=0xffffe00001ffff90) at src/mm/page/meta.rs:156
#5  0xffffffff88799be8 in ostd::mm::page::{impl#4}::drop<ostd::mm::page::meta::KernelStackMeta> (self=0xffff8001cb2bf840) at src/mm/page/mod.rs:188
#6  0xffffffff88792cda in core::ptr::drop_in_place<ostd::mm::page::Page<ostd::mm::page::meta::KernelStackMeta>> ()
    at /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ptr/mod.rs:574
#7  0xffffffff887933b6 in core::ptr::drop_in_place<[ostd::mm::page::Page<ostd::mm::page::meta::KernelStackMeta>]> ()
    at /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ptr/mod.rs:574
#8  0xffffffff88793c8f in alloc::vec::{impl#25}::drop<ostd::mm::page::Page<ostd::mm::page::meta::KernelStackMeta>, alloc::alloc::Global> (
    self=0xffffdfff346ca158) at /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/vec/mod.rs:3719
#9  0xffffffff8878e7f3 in core::ptr::drop_in_place<alloc::vec::Vec<ostd::mm::page::Page<ostd::mm::page::meta::KernelStackMeta>, alloc::alloc::Global>> ()
    at /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ptr/mod.rs:574
#10 0xffffffff88873f5b in ostd::mm::page::allocator::alloc<ostd::mm::page::meta::KernelStackMeta, ostd::task::kernel_stack::{impl#0}::new_with_guard_page::{closure_env#0}> (len=262144, metadata_fn=...) at src/mm/page/allocator.rs:131
#11 0xffffffff8884ca4e in ostd::task::kernel_stack::KernelStack::new_with_guard_page () at src/task/kernel_stack.rs:44
#12 0xffffffff8879bdce in ostd::task::TaskOptions::build (self=<error reading variable: Cannot access memory at address 0x0>) at src/task/mod.rs:169
#13 0xffffffff883d5e9d in aster_nix::thread::task::create_new_user_task (user_space=..., thread_ref=...) at src/thread/task.rs:92
#14 0xffffffff8857f72e in aster_nix::process::posix_thread::builder::{impl#0}::build::{closure#0} (weak_task=0xffffdfff346cb378)
    at src/process/posix_thread/builder.rs:126
#15 0xffffffff880b87d0 in alloc::sync::Arc<ostd::task::Task, alloc::alloc::Global>::new_cyclic_in<ostd::task::Task, alloc::alloc::Global, aster_nix::process::posix_thread::builder::{impl#0}::build::{closure_env#0}> (data_fn=..., alloc=...)
    at /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/sync.rs:795
#16 0xffffffff880a7aa6 in alloc::sync::Arc<ostd::task::Task, alloc::alloc::Global>::new_cyclic<ostd::task::Task, aster_nix::process::posix_thread::builder::{impl#0}::build::{closure_env#0}> (data_fn=...)
    at /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/sync.rs:454
#17 0xffffffff8841760d in aster_nix::process::posix_thread::builder::PosixThreadBuilder::build (self=...) at src/process/posix_thread/builder.rs:89
#18 0xffffffff885df034 in aster_nix::process::process::builder::ProcessBuilder::build (self=...) at src/process/process/builder.rs:194
#19 0xffffffff8832b1fb in aster_nix::process::clone::clone_child_process (ctx=0xffffdfff346e58a8, parent_context=0xffffdfff346e53d0, clone_args=...)
    at src/process/clone.rs:345
#20 0xffffffff88329766 in aster_nix::process::clone::clone_child (ctx=0xffffdfff346e58a8, parent_context=0xffffdfff346e53d0, clone_args=...)
    at src/process/clone.rs:192
#21 0xffffffff8809c9b3 in aster_nix::syscall::clone::sys_clone (clone_flags=18874385, new_sp=0, parent_tidptr=0, child_tidptr=268441232, tls=0, 
    ctx=0xffffdfff346e58a8, parent_context=0xffffdfff346e53d0) at src/syscall/clone.rs:26
#22 0xffffffff884e30a9 in aster_nix::syscall::arch::x86::syscall_dispatch (syscall_number=56, args=..., ctx=0xffffdfff346e58a8, user_ctx=0xffffdfff346e53d0)
    at src/syscall/mod.rs:208
#23 0xffffffff884d081e in aster_nix::syscall::handle_syscall (ctx=0xffffdfff346e58a8, user_ctx=0xffffdfff346e53d0) at src/syscall/mod.rs:321
#24 0xffffffff883d6beb in aster_nix::thread::task::create_new_user_task::user_task_entry () at src/thread/task.rs:70
#25 0xffffffff8851d586 in core::ops::function::FnOnce::call_once<fn(), ()> ()
    at /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:250
#26 0xffffffff8857bf87 in unwinding::panicking::catch_unwind::do_call<fn(), ()> (data=0xffffdfff346e5c50)
    at /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.3/src/panicking.rs:54
#27 0xffffffff8857e198 in __rust_try ()
#28 0xffffffff8857beef in unwinding::panicking::catch_unwind<unwinding::panic::RustPanic, (), fn()> (f=0xffdfff346e5c5000)
    at /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.3/src/panicking.rs:42
#29 0xffffffff8857bdd2 in unwinding::panic::catch_unwind<(), fn()> (f=0xffffff884c0d4600)
    at /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.3/src/panic.rs:87
#30 0xffffffff884c0d46 in aster_nix::thread::oops::catch_panics_as_oops<fn(), ()> (f=0x80026def5a100000) at src/thread/oops.rs:54
#31 0xffffffff8858ddbc in aster_nix::thread::task::create_new_user_task::{closure#0} () at src/thread/task.rs:95
#32 0xffffffff8851ba1e in core::ops::function::FnOnce::call_once<aster_nix::thread::task::create_new_user_task::{closure_env#0}, ()> ()
    at /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:250
#33 0xffffffff888bd4b8 in alloc::boxed::{impl#48}::call_once<(), (dyn core::ops::function::FnOnce<(), Output=()> + core::marker::Send + core::marker::Sync), alloc::alloc::Global> (self=..., args=()) at /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2454
#34 0xffffffff8879c467 in ostd::task::{impl#1}::build::kernel_task_entry () at src/task/mod.rs:161
#35 0x0000000000000000 in ?? ()
Backtrace stopped: Cannot access memory at address 0xffffdfff346e6000
(gdb) n
833         }
(gdb) 
ostd::sync::spin::SpinLock<ostd::mm::page::allocator::CountingFrameAllocator, ostd::sync::spin::PreemptDisabled>::try_acquire_lock<ostd::mm::page::allocator::CountingFrameAllocator, ostd::sync::spin::PreemptDisabled> (self=0xffffffff88d19b38 <ostd::mm::page::allocator::PAGE_ALLOCATOR>) at src/sync/spin.rs:161
161         }
(gdb) 
ostd::sync::spin::SpinLock<ostd::mm::page::allocator::CountingFrameAllocator, ostd::sync::spin::PreemptDisabled>::acquire_lock<ostd::mm::page::allocator::CountingFrameAllocator, ostd::sync::spin::PreemptDisabled> (self=0xffffffff88d19b38 <ostd::mm::page::allocator::PAGE_ALLOCATOR>) at src/sync/spin.rs:152
152                 core::hint::spin_loop();
(gdb) 
151             while !self.try_acquire_lock() {
(gdb) 
152                 core::hint::spin_loop();
(gdb) 
151             while !self.try_acquire_lock() {
(gdb) 
152                 core::hint::spin_loop();
```

# syscall-fork-7ddfd42-drop_as_last-unwrap

An unwrap in PageTableNode::alloc() at ostd/src/mm/page_table/node/mod.rs:263

### Logs
```
forking 14
[   112.809] ERROR: Uncaught panic: called `Option::unwrap()` on a `None` value
at /root/asterinas/ostd/src/mm/page_table/node/mod.rs:263
Printing stack trace:
   1: fn 0xffffffff88824e20 - pc 0xffffffff88824e8a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff345fea90;

   2: fn 0xffffffff880dc7d0 - pc 0xffffffff880dd24d / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff345feaf0;

   3: fn 0xffffffff880dc7c0 - pc 0xffffffff880dc7ca / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff345ff000;

   4: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff345ff010;

   5: fn 0xffffffff889b8500 - pc 0xffffffff889b8544 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff345ff020;

   6: fn 0xffffffff889b8620 - pc 0xffffffff889b8660 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff345ff060;

   7: fn 0xffffffff889b5ef0 - pc 0xffffffff889b5f05 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff345ff0d0;

   8: fn 0xffffffff8887ce10 - pc 0xffffffff8887cebe / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff345ff0e0;

   9: fn 0xffffffff8888c2e0 - pc 0xffffffff8888c90b / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff345ff1d0;

  10: fn 0xffffffff885184a0 - pc 0xffffffff885189a7 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff345ff4c0;

  11: fn 0xffffffff884bf780 - pc 0xffffffff884bf79d / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff345ff6f0;

  12: fn 0xffffffff885866e0 - pc 0xffffffff88587431 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff345ff720;

  13: fn 0xffffffff885865d0 - pc 0xffffffff8858666b / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff34600140;

  14: fn 0xffffffff8808abe0 - pc 0xffffffff8808ac4a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff346001b0;

  15: fn 0xffffffff8807e790 - pc 0xffffffff8807e7bf / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff346002d0;

  16: fn 0xffffffff8832bd60 - pc 0xffffffff8832bd9e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff346003f0;

  17: fn 0xffffffff8832a440 - pc 0xffffffff8832a52d / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff34600460;

  18: fn 0xffffffff88329630 - pc 0xffffffff88329766 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff34601d10;

  19: fn 0xffffffff8809c7e0 - pc 0xffffffff8809c9b3 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff34601ff0;

  20: fn 0xffffffff884d0c70 - pc 0xffffffff884e30a9 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff34602930;

  21: fn 0xffffffff884d0790 - pc 0xffffffff884d081e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff34618fd0;

  22: fn 0xffffffff883d5f10 - pc 0xffffffff883d6beb / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff34619170;

  23: fn 0xffffffff8851d580 - pc 0xffffffff8851d586 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff34619bf0;

  24: fn 0xffffffff8857bf70 - pc 0xffffffff8857bf87 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff34619c00;

  25: fn 0xffffffff8857e180 - pc 0xffffffff8857e198 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff34619c20;

  26: fn 0xffffffff8857bec0 - pc 0xffffffff8857beef / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff34619c40;

  27: fn 0xffffffff8857bdc0 - pc 0xffffffff8857bdd2 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff34619c90;

  28: fn 0xffffffff884c0d30 - pc 0xffffffff884c0d46 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff34619cc0;

  29: fn 0xffffffff8858ddb0 - pc 0xffffffff8858ddbc / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff34619f00;

  30: fn 0xffffffff8851ba10 - pc 0xffffffff8851ba1e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff34619f30;

  31: fn 0xffffffff888bd490 - pc 0xffffffff888bd4b8 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff34619f50;

  32: fn 0xffffffff8879c3a0 - pc 0xffffffff8879c467 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff34619f90;


     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8877a576; rdi 0xffffffff888b9e47; rbp                0x0; rsp 0xffffdfff3461a000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panic.rs:107
(  2) /root/asterinas/kernel/src/thread/oops.rs:118
(  3) ??:?
(  4) d3fk0tre0con5wkjc3r9gnowf:?
(  5) ??:?
(  6) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/panicking.rs:220
(  7) ??:?
(  8) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/option.rs:968
(  9) /root/asterinas/ostd/src/mm/page_table/cursor.rs:439
( 10) /root/asterinas/ostd/src/mm/page_table/cursor.rs:811
( 11) /root/asterinas/ostd/src/mm/vm_space.rs:423
( 12) /root/asterinas/kernel/src/vm/vmar/mod.rs:710
( 13) /root/asterinas/kernel/src/vm/vmar/mod.rs:653
( 14) /root/asterinas/kernel/src/vm/vmar/static_cap.rs:154
( 15) /root/asterinas/kernel/src/process/process_vm/mod.rs:99
( 16) /root/asterinas/kernel/src/process/clone.rs:406
( 17) /root/asterinas/kernel/src/process/clone.rs:273
( 18) /root/asterinas/kernel/src/process/clone.rs:192
( 19) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/result.rs:1101
( 20) /root/asterinas/kernel/src/syscall/mod.rs:208
( 21) /root/asterinas/kernel/src/syscall/mod.rs:328
( 22) /root/asterinas/kernel/src/thread/task.rs:70
( 23) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:250
( 24) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.3/src/panicking.rs:56
( 25) bx7z2gwn51hajqi9fiplhggs6:?
( 26) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.3/src/panicking.rs:42
( 27) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.3/src/panic.rs:87
( 28) /root/asterinas/kernel/src/thread/oops.rs:54
( 29) /root/asterinas/kernel/src/thread/task.rs:95
( 30) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:250
( 31) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2454
( 32) /root/asterinas/ostd/src/task/mod.rs:161
make: *** [Makefile:206: gdb_server] Error 1
```

# syscall-fork-7ddfd42-print_stack_trace-deadlock
Deadlock in `print_stack_trace()`

### Reference
https://github.com/asterinas/asterinas/issues/

### Describe the bug
There is a deadlock in `print_stack_trace()` at src/panic.rs:62 when user runs lots of times of a program which calls system call `fork` miltiple times.
https://github.com/asterinas/asterinas/blob/7ddfd42baa210656127044995d8707fde74fab4d/ostd/src/panic.rs#L62

### To Reproduce
1. Compile a program which calls system call `fork` miltiple times:
```C
#include <stdio.h>
#include <unistd.h>

int main() {
  for (int i = 0; i < 10; i++) {
    printf("forking %d\n", i);
    fork();
  }
}
```
2. Run the program lots of times, Asterinas will panic and hang.
```bash
$ for i in $(seq 1 100); do echo "Running iteration $i"; ./root/fork-10.c; done
```

### Expected behavior
The program shall not panic and hang in Asterinas.

### Logs
```
~ # for i in $(seq 1 100); do echo "Running iteration $i"; ./root/fork-10.c; done
...
Running iteration 29
...
forking 9
Printing stack trace:
```

And the GDB log is:
```
0xffffffff888d636c in core::sync::atomic::atomic_compare_exchange<u8> (dst=0xffffffff88d19af1 <ostd::panic::print_stack_trace::BACKTRACE_PRINT_LOCK>, old=0, 
    new=1, success=core::sync::atomic::Ordering::Acquire, failure=core::sync::atomic::Ordering::Relaxed)
    at /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/sync/atomic.rs:3400
3400            match (success, failure) {
(gdb) bt
#0  0xffffffff888d636c in core::sync::atomic::atomic_compare_exchange<u8> (dst=0xffffffff88d19af1 <ostd::panic::print_stack_trace::BACKTRACE_PRINT_LOCK>, old=0, 
    new=1, success=core::sync::atomic::Ordering::Acquire, failure=core::sync::atomic::Ordering::Relaxed)
    at /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/sync/atomic.rs:3400
#1  0xffffffff887a516b in core::sync::atomic::AtomicBool::compare_exchange (self=0xffffffff88d19af1 <ostd::panic::print_stack_trace::BACKTRACE_PRINT_LOCK>, 
    current=false, new=true, success=core::sync::atomic::Ordering::Acquire, failure=core::sync::atomic::Ordering::Relaxed)
    at /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/sync/atomic.rs:827
#2  0xffffffff887f39d0 in ostd::sync::spin::SpinLock<(), ostd::sync::spin::PreemptDisabled>::try_acquire_lock<(), ostd::sync::spin::PreemptDisabled> (
    self=0xffffffff88d19af1 <ostd::panic::print_stack_trace::BACKTRACE_PRINT_LOCK>) at src/sync/spin.rs:157
#3  0xffffffff887f3548 in ostd::sync::spin::SpinLock<(), ostd::sync::spin::PreemptDisabled>::acquire_lock<(), ostd::sync::spin::PreemptDisabled> (
    self=0xffffffff88d19af1 <ostd::panic::print_stack_trace::BACKTRACE_PRINT_LOCK>) at src/sync/spin.rs:151
#4  0xffffffff887f2afb in ostd::sync::spin::SpinLock<(), ostd::sync::spin::PreemptDisabled>::lock<(), ostd::sync::spin::PreemptDisabled> (
    self=0xffffffff88d19af1 <ostd::panic::print_stack_trace::BACKTRACE_PRINT_LOCK>) at src/sync/spin.rs:114
#5  0xffffffff88824e30 in ostd::panic::print_stack_trace () at src/panic.rs:62
#6  0xffffffff880dd24d in aster_nix::thread::oops::panic_handler (info=0xffffdfffffdbe3f0) at src/thread/oops.rs:113
#7  0xffffffff880dc7ca in aster_nix::thread::oops::__ostd_panic_handler (info=0xffffdfffffdbe3f0) at src/thread/oops.rs:82
#8  0xffffffff8804900a in aster_nix_osdk_bin::panic (info=0xffffdfffffdbe3f0) at src/main.rs:11
#9  0xffffffff889b8544 in core::panicking::panic_fmt (fmt=...) at src/panicking.rs:74
#10 0xffffffff88887b78 in ostd::arch::x86::trap::handle_kernel_page_fault (f=0xffffdfffffdbec10, page_fault_vaddr=18446744073709551525)
    at src/arch/x86/trap/mod.rs:293
#11 0xffffffff88886ed1 in ostd::arch::x86::trap::trap_handler (f=0xffffdfffffdbec10) at src/arch/x86/trap/mod.rs:239
#12 0xffffffff88815c0a in __from_kernel ()
#13 0xffffdfffffdbed00 in ?? ()
#14 0x0000000000000000 in ?? ()
(gdb) n
3404                (Acquire, Relaxed) => intrinsics::atomic_cxchg_acquire_relaxed(dst, old, new),
(gdb) 
3399        let (val, ok) = unsafe {
(gdb) 
3420        if ok { Ok(val) } else { Err(val) }
(gdb) 
3420        if ok { Ok(val) } else { Err(val) }
(gdb) 
3421    }
(gdb) 
core::sync::atomic::AtomicBool::compare_exchange (self=0xffffffff88d19af1 <ostd::panic::print_stack_trace::BACKTRACE_PRINT_LOCK>, current=false, new=true, success=core::sync::atomic::Ordering::Acquire, failure=core::sync::atomic::Ordering::Relaxed) at /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/sync/atomic.rs:826
826                 match unsafe {
(gdb) 
830                     Err(x) => Err(x != 0),
(gdb) 
833         }
(gdb) 
ostd::sync::spin::SpinLock<(), ostd::sync::spin::PreemptDisabled>::try_acquire_lock<(), ostd::sync::spin::PreemptDisabled> (self=0xffffffff88d19af1 <ostd::panic::print_stack_trace::BACKTRACE_PRINT_LOCK>) at src/sync/spin.rs:161
161         }
(gdb) 
ostd::sync::spin::SpinLock<(), ostd::sync::spin::PreemptDisabled>::acquire_lock<(), ostd::sync::spin::PreemptDisabled> (self=0xffffffff88d19af1 <ostd::panic::print_stack_trace::BACKTRACE_PRINT_LOCK>) at src/sync/spin.rs:152
152                 core::hint::spin_loop();
(gdb) 
151             while !self.try_acquire_lock() {
(gdb) 
152                 core::hint::spin_loop();
...
```

# syscall-mmap-7ddfd42-handle_page_fault-panic
Reachable panic in `handle_page_fault()` due to page_fault

### Reference
https://github.com/asterinas/asterinas/issues/

### Describe the bug
There is a reachable panic in `handle_page_fault()` at kernel/src/vm/vmar/vm_mapping.rs:262 when user trigger a non-COW page fault on mapped address.
https://github.com/asterinas/asterinas/blob/7ddfd42baa210656127044995d8707fde74fab4d/kernel/src/vm/vmar/vm_mapping.rs#L262

### To Reproduce
I'm trying to construct a PoC...

### Expected behavior
Asterinas shall not panic.

### Logs
```
[    92.445] ERROR: Uncaught panic: non-COW page fault should not happen on mapped address
at /root/asterinas/kernel/src/vm/vmar/vm_mapping.rs:262
Printing stack trace:
   1: fn 0xffffffff8884a240 - pc 0xffffffff8884a2aa / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8879f996; rdi 0xffffffff888df267; rbp                0x0; rsp 0xffffdfffffd54030;

   2: fn 0xffffffff88628730 - pc 0xffffffff8862944c / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8879f996; rdi 0xffffffff888df267; rbp                0x0; rsp 0xffffdfffffd54090;

   3: fn 0xffffffff88628720 - pc 0xffffffff8862872a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8879f996; rdi 0xffffffff888df267; rbp                0x0; rsp 0xffffdfffffd546b0;

   4: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8879f996; rdi 0xffffffff888df267; rbp                0x0; rsp 0xffffdfffffd546c0;

   5: fn 0xffffffff889dd920 - pc 0xffffffff889dd964 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8879f996; rdi 0xffffffff888df267; rbp                0x0; rsp 0xffffdfffffd546d0;

   6: fn 0xffffffff884e7f60 - pc 0xffffffff884e89b7 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8879f996; rdi 0xffffffff888df267; rbp                0x0; rsp 0xffffdfffffd54710;

   7: fn 0xffffffff885a6020 - pc 0xffffffff885a63c7 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8879f996; rdi 0xffffffff888df267; rbp                0x0; rsp 0xffffdfffffd54d90;

   8: fn 0xffffffff88628360 - pc 0xffffffff886283e4 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8879f996; rdi 0xffffffff888df267; rbp                0x0; rsp 0xffffdfffffd54f00;

   9: fn 0xffffffff883c7070 - pc 0xffffffff883c709c / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8879f996; rdi 0xffffffff888df267; rbp                0x0; rsp 0xffffdfffffd54fa0;

  10: fn 0xffffffff883c6e40 - pc 0xffffffff883c6eba / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8879f996; rdi 0xffffffff888df267; rbp                0x0; rsp 0xffffdfffffd55100;

  11: fn 0xffffffff88274190 - pc 0xffffffff88274e82 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8879f996; rdi 0xffffffff888df267; rbp                0x0; rsp 0xffffdfffffd55170;

  12: fn 0xffffffff88536e60 - pc 0xffffffff88536e66 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8879f996; rdi 0xffffffff888df267; rbp                0x0; rsp 0xffffdfffffd55bf0;

  13: fn 0xffffffff882f9140 - pc 0xffffffff882f9157 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8879f996; rdi 0xffffffff888df267; rbp                0x0; rsp 0xffffdfffffd55c00;

  14: fn 0xffffffff882fd430 - pc 0xffffffff882fd448 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8879f996; rdi 0xffffffff888df267; rbp                0x0; rsp 0xffffdfffffd55c20;

  15: fn 0xffffffff882f7690 - pc 0xffffffff882f76bf / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8879f996; rdi 0xffffffff888df267; rbp                0x0; rsp 0xffffdfffffd55c40;

  16: fn 0xffffffff882ebfc0 - pc 0xffffffff882ebfd2 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8879f996; rdi 0xffffffff888df267; rbp                0x0; rsp 0xffffdfffffd55c90;

  17: fn 0xffffffff880c2650 - pc 0xffffffff880c2666 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8879f996; rdi 0xffffffff888df267; rbp                0x0; rsp 0xffffdfffffd55cc0;

  18: fn 0xffffffff8857e020 - pc 0xffffffff8857e02c / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8879f996; rdi 0xffffffff888df267; rbp                0x0; rsp 0xffffdfffffd55f00;

  19: fn 0xffffffff885352f0 - pc 0xffffffff885352fe / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8879f996; rdi 0xffffffff888df267; rbp                0x0; rsp 0xffffdfffffd55f30;

  20: fn 0xffffffff888e28b0 - pc 0xffffffff888e28d8 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8879f996; rdi 0xffffffff888df267; rbp                0x0; rsp 0xffffdfffffd55f50;

  21: fn 0xffffffff887c17c0 - pc 0xffffffff887c1887 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8879f996; rdi 0xffffffff888df267; rbp                0x0; rsp 0xffffdfffffd55f90;

  22: fn                0x0 - pc  0x2321c002c70b200 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8879f996; rdi 0xffffffff888df267; rbp                0x0; rsp 0xffffdfffffd56000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panic.rs:107
(  2) /root/asterinas/kernel/src/thread/oops.rs:116
(  3) ??:?
(  4) d3fk0tre0con5wkjc3r9gnowf:?
(  5) ??:?
(  6) /root/asterinas/kernel/src/vm/vmar/vm_mapping.rs:262
(  7) /root/asterinas/kernel/src/vm/vmar/mod.rs:319
(  8) /root/asterinas/kernel/src/vm/vmar/static_cap.rs:176
(  9) /root/asterinas/kernel/src/thread/exception.rs:64
( 10) /root/asterinas/kernel/src/thread/exception.rs:34
( 11) /root/asterinas/kernel/src/thread/task.rs:69
( 12) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:250
( 13) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.3/src/panicking.rs:56
( 14) 1qj6ckhxwa3w3asfep2v0d5bo:?
( 15) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.3/src/panicking.rs:42
( 16) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.3/src/panic.rs:87
( 17) /root/asterinas/kernel/src/thread/oops.rs:54
( 18) /root/asterinas/kernel/src/thread/task.rs:95
( 19) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:250
( 20) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2454
( 21) /root/asterinas/ostd/src/task/mod.rs:161
```

# ?-?-54e2ffb-InitStack::new-logic_error
Asterinas cannot run any program when initialize stack at `MAX_USERSPACE_VADDR`

### Reference
https://github.com/asterinas/asterinas/issues/1592

### Describe the bug
Asterinas cannot run any program when initialize stack at `MAX_USERSPACE_VADDR`

When initialize the stack, Asterinas would generate a random value `nr_pages_padding` and set the stack top as `MAX_USERSPACE_VADDR - nr_pages_padding * PAGE_SIZE`. 
However, when `nr_pages_padding` is 0, Asterinas fail to run any program.

https://github.com/asterinas/asterinas/blob/54e2ffbb71a1e4a49ce1a30e263cc8fdca6b1d7d/kernel/src/process/process_vm/init_stack/mod.rs#L121-L128

### To Reproduce
1. Start Asterinas with 0 value `nr_pages_padding`, we can use this patch to achieve it:
```diff
diff --git a/kernel/src/process/process_vm/init_stack/mod.rs b/kernel/src/process/process_vm/init_stack/mod.rs
index 38373044..95385f4f 100644
--- a/kernel/src/process/process_vm/init_stack/mod.rs
+++ b/kernel/src/process/process_vm/init_stack/mod.rs
@@ -20,7 +20,10 @@ use core::{
 
 use align_ext::AlignExt;
 use aster_rights::Full;
-use ostd::mm::{vm_space::VmItem, VmIo, VmSpace, MAX_USERSPACE_VADDR};
+use ostd::{
+    early_println,
+    mm::{vm_space::VmItem, VmIo, VmSpace, MAX_USERSPACE_VADDR},
+};
 
 use self::aux_vec::{AuxKey, AuxVec};
 use crate::{
@@ -120,11 +123,17 @@ impl Clone for InitStack {
 
 impl InitStack {
     pub(super) fn new() -> Self {
-        let nr_pages_padding = {
-            let mut random_nr_pages_padding: u8 = 0;
-            getrandom(random_nr_pages_padding.as_bytes_mut()).unwrap();
-            random_nr_pages_padding as usize
-        };
+        let nr_pages_padding = loop {
+            let nr_pages_padding = {
+                let mut random_nr_pages_padding: u8 = 0;
+                getrandom(random_nr_pages_padding.as_bytes_mut()).unwrap();
+                random_nr_pages_padding as usize
+            };
+            early_println!("nr_pages_padding is {}", nr_pages_padding);
+            if nr_pages_padding == 0 {
+                break nr_pages_padding;
+            }
+        };
         let initial_top = MAX_USERSPACE_VADDR - PAGE_SIZE * nr_pages_padding;
         let max_size = INIT_STACK_SIZE;
 

```
2. Run any program, Asterinas will report `sh: xxx: Bad address`

### Expected behavior
Asterinas shall run program successfully.

### Environment
- Official docker asterinas/asterinas:0.9.4
- Intel(R) Xeon(R) Gold 6230R CPU @ 2.10GHz
- Asterinas version: main 54e2ffb

### Logs
```
[kernel] Hello world from kernel!


   _   ___ _____ ___ ___ ___ _  _   _   ___
  /_\ / __|_   _| __| _ \_ _| \| | /_\ / __|
 / _ \\__ \ | | | _||   /| || .` |/ _ \\__ \
/_/ \_\___/ |_| |___|_|_\___|_|\_/_/ \_\___/


nr_pages_padding is 41
nr_pages_padding is 240
nr_pages_padding is 45
nr_pages_padding is 167
nr_pages_padding is 254
nr_pages_padding is 2
nr_pages_padding is 62
nr_pages_padding is 45
nr_pages_padding is 200
nr_pages_padding is 94
nr_pages_padding is 225
nr_pages_padding is 54
nr_pages_padding is 0
~ # cat /proc/1/cmdline 
sh: cat: Bad address

~ # /root/fstat.c 
sh: /root/fstat.c: Bad address

~ # /root/lock.c 
sh: /root/lock.c: Bad address
```

# syscall-mmap-e6c613f-VmarInner::alloc_free_region-panic
Reachable assert in `VmarInner::alloc_free_region()` caused by `mmap`

### Reference
https://github.com/asterinas/asterinas/issues/1604

### Describe the bug
There is a reachable assert in `VmarInner::alloc_free_region()` at kernel/src/vm/vmar/mod.rs:204 when user calls mmap multiple times.

https://github.com/asterinas/asterinas/blob/e6c613f53841983765a7b3c56ea9958775c76199/kernel/src/vm/vmar/mod.rs#L205

### To Reproduce
I'm trying to construct a minimal PoC...

### Expected behavior
Asterinas shall not reach the assertion.

### Environment
- Official docker asterinas/asterinas:0.9.4
- Intel(R) Xeon(R) Gold 6230R CPU @ 2.10GHz
- Asterinas version: main e6c613f

### Logs
```
addr = 0x0, len = 0x2, perms = READ, option = MMapOptions { typ: Shared, flags: MAP_ANONYMOUS | MAP_32BIT | MAP_GROWSDOWN | MAP_DENYWRITE | MAP_EXECUTABLE | MAP_LOCKED | MAP_NORESERVE | MAP_POPULATE | MAP_STACK | MAP_HUGETLB }, fd = 34, offset = 0x0
range is 0..4096, last_end is 65536
[    78.468] ERROR: Uncaught panic:
        assertion failed: range.start >= last_end
        at /root/asterinas/kernel/src/vm/vmar/mod.rs:204
        on CPU 0 by thread Some(Thread { task: (Weak), data: Any { .. }, status: AtomicThreadStatus(1), priority: AtomicPriority(120), cpu_affinity: AtomicCpuSet { bits: [1] } })
Printing stack trace:
   1: fn 0xffffffff888169f0 - pc 0xffffffff88816a5a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd7af20;

   2: fn 0xffffffff8804abf0 - pc 0xffffffff8804bc8b / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd7af80;

   3: fn 0xffffffff8804abe0 - pc 0xffffffff8804abea / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd7b750;

   4: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd7b760;

   5: fn 0xffffffff889cd160 - pc 0xffffffff889cd1a4 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd7b770;

   6: fn 0xffffffff889cd280 - pc 0xffffffff889cd2c0 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd7b7b0;

   7: fn 0xffffffff88293ab0 - pc 0xffffffff88293dfe / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd7b820;

   8: fn 0xffffffff88296400 - pc 0xffffffff88296f30 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd7bb80;

   9: fn 0xffffffff883721c0 - pc 0xffffffff8837234f / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd7c2f0;

  10: fn 0xffffffff88373050 - pc 0xffffffff88373192 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd7c620;

  11: fn 0xffffffff882b8970 - pc 0xffffffff882b9d8f / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd7c880;

  12: fn 0xffffffff882b86e0 - pc 0xffffffff882b87f1 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd7d620;

  13: fn 0xffffffff885d9e80 - pc 0xffffffff885d9edf / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd7d7a0;

  14: fn 0xffffffff88175230 - pc 0xffffffff8817525e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd7d7d0;

  15: fn 0xffffffff88179710 - pc 0xffffffff88179728 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd7d830;

  16: fn 0xffffffff88170240 - pc 0xffffffff88170296 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd7d850;

  17: fn 0xffffffff88166a30 - pc 0xffffffff88166a51 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd7d8f0;

  18: fn 0xffffffff8858b9d0 - pc 0xffffffff88591573 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd7d930;

  19: fn 0xffffffff8858ae20 - pc 0xffffffff8858aeae / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd98fd0;

  20: fn 0xffffffff883d91c0 - pc 0xffffffff883d9e1c / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd99170;

  21: fn 0xffffffff883347d0 - pc 0xffffffff883347d6 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd99bf0;

  22: fn 0xffffffff881765a0 - pc 0xffffffff881765b7 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd99c00;

  23: fn 0xffffffff88179710 - pc 0xffffffff88179728 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd99c20;

  24: fn 0xffffffff88169120 - pc 0xffffffff8816914f / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd99c40;

  25: fn 0xffffffff88167c40 - pc 0xffffffff88167c52 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd99c90;

  26: fn 0xffffffff8804a6d0 - pc 0xffffffff8804a6e6 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd99cc0;

  27: fn 0xffffffff881c5c40 - pc 0xffffffff881c5c4c / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd99f00;

  28: fn 0xffffffff88332bd0 - pc 0xffffffff88332bde / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd99f30;

  29: fn 0xffffffff8879a2f0 - pc 0xffffffff8879a318 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd99f50;

  30: fn 0xffffffff8887b980 - pc 0xffffffff8887ba47 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd99f90;

  31: fn                0x0 - pc 0xffffdfff00000000 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886fd416; rdi 0xffffffff88850997; rbp                0x0; rsp 0xffffdfffffd9a000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panic.rs:107
(  2) /root/asterinas/kernel/src/thread/oops.rs:127
(  3) ??:?
(  4) 4h8iq3k9ykvh9t1ey1jx1w3og:?
(  5) ??:?
(  6) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/panicking.rs:220
(  7) 1crifhtzhp0w3c55fh50vuddg:?
(  8) /root/asterinas/kernel/src/vm/vmar/mod.rs:416
(  9) /root/asterinas/kernel/src/vm/vmar/vm_mapping.rs:97
( 10) /root/asterinas/kernel/src/vm/vmar/vm_mapping.rs:573
( 11) /root/asterinas/kernel/src/syscall/mmap.rs:155
( 12) /root/asterinas/kernel/src/syscall/mmap.rs:29
( 13) /root/asterinas/kernel/src/syscall/mod.rs:221
( 14) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.3/src/panicking.rs:54
( 15) 9maf1tev5ibbpauw8j61kc7a1:?
( 16) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.3/src/panicking.rs:42
( 17) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.3/src/panic.rs:87
( 18) /root/asterinas/kernel/src/syscall/mod.rs:284
( 19) /root/asterinas/kernel/src/syscall/mod.rs:338
( 20) /root/asterinas/kernel/src/thread/task.rs:70
( 21) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:250
( 22) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.3/src/panicking.rs:56
( 23) 9maf1tev5ibbpauw8j61kc7a1:?
( 24) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.3/src/panicking.rs:42
( 25) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.3/src/panic.rs:87
( 26) /root/asterinas/kernel/src/thread/oops.rs:54
( 27) /root/asterinas/kernel/src/thread/task.rs:95
( 28) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:250
( 29) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2454
( 30) /root/asterinas/ostd/src/task/mod.rs:175
( 31) ??:0
make: *** [Makefile:194: run] Error 1
```

# syscall-getpeername-9da6af0-read_updated_state-panic
`as_ref` fail in `read_updated_state`

### Reference
https://github.com/asterinas/asterinas/issues/1649

### Describe the bug
There is a `as_ref` failure in `read_updated_state()` at src/net/socket/ip/stream/mod.rs:122 when user calls `getpeername`/`accept` under some conditions.

https://github.com/asterinas/asterinas/blob/9da6af03943c15456cdfd781021820a7da78ea40/kernel/src/net/socket/ip/stream/mod.rs#L119-L122

### To Reproduce
I'm trying to construct a minimal PoC...

### Expected behavior
Asterinas shall not fail in this `as_ref`.

### Environment
- Official docker asterinas/asterinas:0.9.4
- Intel(R) Xeon(R) Gold 6230R CPU @ 2.10GHz
- Asterinas version: main 9da6af0

### Logs
```
Uncaught panic:
	the value has already been removed from the Takeable
	at /root/asterinas/kernel/src/net/socket/ip/stream/mod.rs:122
	on CPU 0 by thread Some(Thread { task: (Weak), data: Any { .. }, status: AtomicThreadStatus(1), priority: AtomicPriority(120), cpu_affinity: AtomicCpuSet { bits: [1] } })[0m
Printing stack trace:
   1: fn 0xffffffff888123e0 - pc 0xffffffff8881244a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5c8960;

   2: fn 0xffffffff8804acb0 - pc 0xffffffff8804bd4b / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5c89c0;

   3: fn 0xffffffff8804aca0 - pc 0xffffffff8804acaa / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5c9190;

   4: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5c91a0;

   5: fn 0xffffffff889da620 - pc 0xffffffff889da664 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5c91b0;

   6: fn 0xffffffff889da830 - pc 0xffffffff889da8cd / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5c91f0;

   7: fn 0xffffffff889d7f30 - pc 0xffffffff889d7f53 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5c9290;

   8: fn 0xffffffff881d6a10 - pc 0xffffffff881d6a5e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5c92b0;

   9: fn 0xffffffff8861f020 - pc 0xffffffff8861f0bc / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5c92f0;

  10: fn 0xffffffff88622870 - pc 0xffffffff8862289b / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5c9380;

  11: fn 0xffffffff88493d10 - pc 0xffffffff884940e1 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5c9470;

  12: fn 0xffffffff884e1280 - pc 0xffffffff884e12bb / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5c97b0;

  13: fn 0xffffffff884dbf90 - pc 0xffffffff884dbfbe / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5c97d0;

  14: fn 0xffffffff884e04b0 - pc 0xffffffff884e04c8 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5c9830;

  15: fn 0xffffffff884d6c20 - pc 0xffffffff884d6c76 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5c9850;

  16: fn 0xffffffff884cd9d0 - pc 0xffffffff884cd9f1 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5c98f0;

  17: fn 0xffffffff8823b9b0 - pc 0xffffffff88250bef / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5c9930;

  18: fn 0xffffffff8823aef0 - pc 0xffffffff8823af7e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5e4fd0;

  19: fn 0xffffffff88356f40 - pc 0xffffffff88357b9c / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5e5170;

  20: fn 0xffffffff8836fe00 - pc 0xffffffff8836fe06 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5e5bf0;

  21: fn 0xffffffff884ddae0 - pc 0xffffffff884ddaf7 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5e5c00;

  22: fn 0xffffffff884e04b0 - pc 0xffffffff884e04c8 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5e5c20;

  23: fn 0xffffffff884d4be0 - pc 0xffffffff884d4c0f / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5e5c40;

  24: fn 0xffffffff884ce650 - pc 0xffffffff884ce662 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5e5c90;

  25: fn 0xffffffff8804a6a0 - pc 0xffffffff8804a6b6 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5e5cc0;

  26: fn 0xffffffff8858ecd0 - pc 0xffffffff8858ecdc / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5e5f00;

  27: fn 0xffffffff8836ef00 - pc 0xffffffff8836ef0e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5e5f30;

  28: fn 0xffffffff88808880 - pc 0xffffffff888088a8 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5e5f50;

  29: fn 0xffffffff887b3fa0 - pc 0xffffffff887b406e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5e5f90;


     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff8870a756; rdi 0xffffffff887d19f7; rbp                0x0; rsp 0xffffdfffff5e6000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) ostd/src/panic.rs:107
(  2) kernel/src/thread/oops.rs:127
(  3) ??:?
(  4) emvdohosl2oxfa2p2dlqt4t4a:?
(  5) ??:?
(  6) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/panicking.rs:271
(  7) ??:?
(  8) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/option.rs:930
(  9) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/takeable-0.2.2/src/lib.rs:169
( 10) kernel/src/net/socket/ip/stream/mod.rs:552
( 11) kernel/src/syscall/getpeername.rs:20
( 12) kernel/src/syscall/mod.rs:172
( 13) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.4/src/panicking.rs:54
( 14) 67yfr1o61dqnmg700sea05njg:?
( 15) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.4/src/panicking.rs:42
( 16) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.4/src/panic.rs:87
( 17) kernel/src/syscall/mod.rs:284
( 18) kernel/src/syscall/mod.rs:338
( 19) kernel/src/thread/task.rs:70
( 20) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:250
( 21) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.4/src/panicking.rs:56
( 22) 67yfr1o61dqnmg700sea05njg:?
( 23) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.4/src/panicking.rs:42
( 24) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.4/src/panic.rs:87
( 25) kernel/src/thread/oops.rs:54
( 26) kernel/src/thread/task.rs:95
( 27) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:250
( 28) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2454
( 29) ostd/src/task/mod.rs:178
```

# syscall-execve-5313689-check_segment_align-panic
Reachable assertion in `check_segment_align` when running program whose `p_offset` and `p_filesz` is 0

### Reference
https://github.com/asterinas/asterinas/issues/

### Describe the bug
There is a reachable assertion in `check_segment_align()` at kernel/src/process/program_loader/elf/load_elf.rs:377 when user runs specific program.

https://github.com/asterinas/asterinas/blob/5313689d6fa91596f4bc52bf6fb866bf6a7a706f/kernel/src/process/program_loader/elf/load_elf.rs#L377

### To Reproduce
Download the sample program and run.

### Expected behavior
Asterinas shall not panic when run the program.

### Environment
- Official docker asterinas/asterinas:0.9.4
- Intel(R) Xeon(R) Gold 6230R CPU @ 2.10GHz
- Asterinas version: main 5313689

### Logs
```
~ # /root/syscall-execve-5313689-check_segment_align-panic
[    65.057] DEBUG: allocate free region, map_size = 0x1000, offset = Some(10000000), align = 0x1000, can_overwrite = false
[    65.058] DEBUG: allocate free region, map_size = 0x3fff000, offset = Some(10001000), align = 0x1000, can_overwrite = false
[    65.059] DEBUG: mem range = 0x400000 - 0x400528, mem_size = 0x528
[    65.060] DEBUG: file range = 0x0 - 0x528, file_size = 0x528
[    65.061] DEBUG: allocate free region, map_size = 0x1000, offset = Some(400000), align = 0x1000, can_overwrite = true
[    65.062] DEBUG: mem range = 0x401000 - 0x49765d, mem_size = 0x9665d
[    65.063] DEBUG: file range = 0x1000 - 0x9765d, file_size = 0x9665d
[    65.064] DEBUG: allocate free region, map_size = 0x97000, offset = Some(401000), align = 0x1000, can_overwrite = true
[    65.065] DEBUG: mem range = 0x498000 - 0x4c053c, mem_size = 0x2853c
[    65.066] DEBUG: file range = 0x98000 - 0xc053c, file_size = 0x2853c
[    65.067] DEBUG: allocate free region, map_size = 0x29000, offset = Some(498000), align = 0x1000, can_overwrite = true
[    65.068] ERROR: Uncaught panic:
        assertion failed: program_header.offset % align == program_header.virtual_addr % align
        at /root/asterinas/kernel/src/process/program_loader/elf/load_elf.rs:377
        on CPU 0 by thread Some(Thread { task: (Weak), data: Any { .. }, status: AtomicThreadStatus(1), priority: AtomicPriority(120), cpu_affinity: AtomicCpuSet { bits: [1] } })
Printing stack trace:
   1: fn 0xffffffff887b38d0 - pc 0xffffffff887b393a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e4ab6; rdi 0xffffffff887caab7; rbp                0x0; rsp 0xffffdfffffd7cef0;

   2: fn 0xffffffff8804ac00 - pc 0xffffffff8804bb89 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e4ab6; rdi 0xffffffff887caab7; rbp                0x0; rsp 0xffffdfffffd7cf50;

   3: fn 0xffffffff8804abf0 - pc 0xffffffff8804abfa / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e4ab6; rdi 0xffffffff887caab7; rbp                0x0; rsp 0xffffdfffffd7d6d0;

   4: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e4ab6; rdi 0xffffffff887caab7; rbp                0x0; rsp 0xffffdfffffd7d6e0;

   5: fn 0xffffffff889b4550 - pc 0xffffffff889b4594 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e4ab6; rdi 0xffffffff887caab7; rbp                0x0; rsp 0xffffdfffffd7d6f0;

   6: fn 0xffffffff889b4670 - pc 0xffffffff889b46b0 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e4ab6; rdi 0xffffffff887caab7; rbp                0x0; rsp 0xffffdfffffd7d730;

   7: fn 0xffffffff884ac530 - pc 0xffffffff884ac77f / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e4ab6; rdi 0xffffffff887caab7; rbp                0x0; rsp 0xffffdfffffd7d7a0;

   8: fn 0xffffffff884a9e10 - pc 0xffffffff884aa0cf / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e4ab6; rdi 0xffffffff887caab7; rbp                0x0; rsp 0xffffdfffffd7d880;

   9: fn 0xffffffff884a9550 - pc 0xffffffff884a982b / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e4ab6; rdi 0xffffffff887caab7; rbp                0x0; rsp 0xffffdfffffd7dab0;

  10: fn 0xffffffff884a8130 - pc 0xffffffff884a843c / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e4ab6; rdi 0xffffffff887caab7; rbp                0x0; rsp 0xffffdfffffd7ddd0;

  11: fn 0xffffffff883baa30 - pc 0xffffffff883bb806 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e4ab6; rdi 0xffffffff887caab7; rbp                0x0; rsp 0xffffdfffffd7e450;

  12: fn 0xffffffff8859d0c0 - pc 0xffffffff8859de86 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e4ab6; rdi 0xffffffff887caab7; rbp                0x0; rsp 0xffffdfffffd7fa00;

  13: fn 0xffffffff8859c010 - pc 0xffffffff8859c2f3 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e4ab6; rdi 0xffffffff887caab7; rbp                0x0; rsp 0xffffdfffffd80a40;

  14: fn 0xffffffff88559dc0 - pc 0xffffffff8856e159 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e4ab6; rdi 0xffffffff887caab7; rbp                0x0; rsp 0xffffdfffffd80c80;

  15: fn 0xffffffff880f47a0 - pc 0xffffffff880f482e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e4ab6; rdi 0xffffffff887caab7; rbp                0x0; rsp 0xffffdfffffd98fd0;

  16: fn 0xffffffff88351c80 - pc 0xffffffff883528dc / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e4ab6; rdi 0xffffffff887caab7; rbp                0x0; rsp 0xffffdfffffd99170;

  17: fn 0xffffffff88431120 - pc 0xffffffff88431126 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e4ab6; rdi 0xffffffff887caab7; rbp                0x0; rsp 0xffffdfffffd99bf0;

  18: fn 0xffffffff883d3b50 - pc 0xffffffff883d3b67 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e4ab6; rdi 0xffffffff887caab7; rbp                0x0; rsp 0xffffdfffffd99c00;

  19: fn 0xffffffff883d5c20 - pc 0xffffffff883d5c38 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e4ab6; rdi 0xffffffff887caab7; rbp                0x0; rsp 0xffffdfffffd99c20;

  20: fn 0xffffffff883d3a30 - pc 0xffffffff883d3a5f / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e4ab6; rdi 0xffffffff887caab7; rbp                0x0; rsp 0xffffdfffffd99c40;

  21: fn 0xffffffff8814b800 - pc 0xffffffff8814b812 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e4ab6; rdi 0xffffffff887caab7; rbp                0x0; rsp 0xffffdfffffd99c90;

  22: fn 0xffffffff88512160 - pc 0xffffffff88512176 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e4ab6; rdi 0xffffffff887caab7; rbp                0x0; rsp 0xffffdfffffd99cc0;

  23: fn 0xffffffff88342230 - pc 0xffffffff8834223c / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e4ab6; rdi 0xffffffff887caab7; rbp                0x0; rsp 0xffffdfffffd99f00;

  24: fn 0xffffffff8842f100 - pc 0xffffffff8842f10e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e4ab6; rdi 0xffffffff887caab7; rbp                0x0; rsp 0xffffdfffffd99f30;

  25: fn 0xffffffff8886e4d0 - pc 0xffffffff8886e4f8 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e4ab6; rdi 0xffffffff887caab7; rbp                0x0; rsp 0xffffdfffffd99f50;

  26: fn 0xffffffff8881b240 - pc 0xffffffff8881b30e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e4ab6; rdi 0xffffffff887caab7; rbp                0x0; rsp 0xffffdfffffd99f90;


     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e4ab6; rdi 0xffffffff887caab7; rbp                0x0; rsp 0xffffdfffffd9a000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panic.rs:107
(  2) /root/asterinas/kernel/src/thread/oops.rs:124
(  3) ??:?
(  4) 3ogy39h8ns04itidb7sl4v5he:?
(  5) ??:?
(  6) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/panicking.rs:220
(  7) bia737865l7dhslq1c9skho6z:?
(  8) /root/asterinas/kernel/src/process/program_loader/elf/load_elf.rs:209
(  9) /root/asterinas/kernel/src/process/program_loader/elf/load_elf.rs:128
( 10) /root/asterinas/kernel/src/process/program_loader/elf/load_elf.rs:46
( 11) /root/asterinas/kernel/src/process/program_loader/mod.rs:66
( 12) /root/asterinas/kernel/src/syscall/execve.rs:117
( 13) /root/asterinas/kernel/src/syscall/execve.rs:33
( 14) /root/asterinas/kernel/src/syscall/mod.rs:175
( 15) /root/asterinas/kernel/src/syscall/mod.rs:329
( 16) /root/asterinas/kernel/src/thread/task.rs:70
( 17) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:250
( 18) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.4/src/panicking.rs:56
( 19) 97rarjgzbtso0b5qwey8u6xa1:?
( 20) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.4/src/panicking.rs:42
( 21) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.4/src/panic.rs:87
( 22) /root/asterinas/kernel/src/thread/oops.rs:54
( 23) /root/asterinas/kernel/src/thread/task.rs:95
( 24) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:250
( 25) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2454
( 26) /root/asterinas/ostd/src/task/mod.rs:178
make: *** [Makefile:210: run] Error 1
```

# syscall-execve-5313689-read_updated_state-panic
Reachable assertion in `is_intersected` when run specific program 

### Reference
https://github.com/asterinas/asterinas/issues/

### Describe the bug
There is a reachable assertion in `is_intersected()` at kernel/src/vm/vmar/mod.rs:672 when user runs specific program.

https://github.com/asterinas/asterinas/blob/5313689d6fa91596f4bc52bf6fb866bf6a7a706f/kernel/src/vm/vmar/mod.rs#L671-L674

### To Reproduce
Just run the provided sample program.

### Expected behavior
Asterinas shall not panic when run the program.

### Environment
- Official docker asterinas/asterinas:0.9.4
- Intel(R) Xeon(R) Gold 6230R CPU @ 2.10GHz
- Asterinas version: main 5313689

### Logs
```
[    44.911] ERROR: Uncaught panic:
        assertion failed: is_intersected(range1, range2)
        at /root/asterinas/kernel/src/vm/vmar/mod.rs:672
        on CPU 0 by thread Some(Thread { task: (Weak), data: Any { .. }, status: AtomicThreadStatus(1), priority: AtomicPriority(120), cpu_affinity: AtomicCpuSet { bits: [1] } })
Printing stack trace:
   1: fn 0xffffffff887b37e0 - pc 0xffffffff887b384a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd7aef0;

   2: fn 0xffffffff8804ac00 - pc 0xffffffff8804bb89 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd7af50;

   3: fn 0xffffffff8804abf0 - pc 0xffffffff8804abfa / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd7b6d0;

   4: fn 0xffffffff88049000 - pc 0xffffffff8804900a / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd7b6e0;

   5: fn 0xffffffff889b4460 - pc 0xffffffff889b44a4 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd7b6f0;

   6: fn 0xffffffff889b4580 - pc 0xffffffff889b45c0 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd7b730;

   7: fn 0xffffffff882fdbc0 - pc 0xffffffff882fdd02 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd7b7a0;

   8: fn 0xffffffff882f9b30 - pc 0xffffffff882f9fb5 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd7b880;

   9: fn 0xffffffff88163d00 - pc 0xffffffff88164ca5 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd7bf50;

  10: fn 0xffffffff884aa480 - pc 0xffffffff884abe03 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd7c8a0;

  11: fn 0xffffffff884a9d20 - pc 0xffffffff884aa02e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd7d880;

  12: fn 0xffffffff884a9460 - pc 0xffffffff884a973b / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd7dab0;

  13: fn 0xffffffff884a8040 - pc 0xffffffff884a834c / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd7ddd0;

  14: fn 0xffffffff883ba940 - pc 0xffffffff883bb716 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd7e450;

  15: fn 0xffffffff8859cfd0 - pc 0xffffffff8859dd96 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd7fa00;

  16: fn 0xffffffff8859bf20 - pc 0xffffffff8859c203 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd80a40;

  17: fn 0xffffffff88559cd0 - pc 0xffffffff8856e069 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd80c80;

  18: fn 0xffffffff880f47d0 - pc 0xffffffff880f485e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd98fd0;

  19: fn 0xffffffff88351b90 - pc 0xffffffff883527ec / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd99170;

  20: fn 0xffffffff88431030 - pc 0xffffffff88431036 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd99bf0;

  21: fn 0xffffffff883d3a60 - pc 0xffffffff883d3a77 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd99c00;

  22: fn 0xffffffff883d5b30 - pc 0xffffffff883d5b48 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd99c20;

  23: fn 0xffffffff883d3940 - pc 0xffffffff883d396f / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd99c40;

  24: fn 0xffffffff8814b830 - pc 0xffffffff8814b842 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd99c90;

  25: fn 0xffffffff88512070 - pc 0xffffffff88512086 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd99cc0;

  26: fn 0xffffffff88342140 - pc 0xffffffff8834214c / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd99f00;

  27: fn 0xffffffff8842f010 - pc 0xffffffff8842f01e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd99f30;

  28: fn 0xffffffff8886e3e0 - pc 0xffffffff8886e408 / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd99f50;

  29: fn 0xffffffff8881b150 - pc 0xffffffff8881b21e / registers:

     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd99f90;


     rax                0x1; rdx                0x8; rcx                0x0; rbx                0x0;
     rsi 0xffffffff886e49c6; rdi 0xffffffff887ca9c7; rbp                0x0; rsp 0xffffdfffffd9a000;

[OSDK] The kernel seems panicked. Parsing stack trace for source lines:
(  1) /root/asterinas/ostd/src/panic.rs:107
(  2) /root/asterinas/kernel/src/thread/oops.rs:124
(  3) ??:?
(  4) 3ogy39h8ns04itidb7sl4v5he:?
(  5) ??:?
(  6) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/panicking.rs:220
(  7) /root/asterinas/kernel/src/vm/vmar/mod.rs:673
(  8) /root/asterinas/kernel/src/vm/vmar/mod.rs:166
(  9) /root/asterinas/kernel/src/vm/vmar/mod.rs:598
( 10) /root/asterinas/kernel/src/process/program_loader/elf/load_elf.rs:339
( 11) /root/asterinas/kernel/src/process/program_loader/elf/load_elf.rs:210
( 12) /root/asterinas/kernel/src/process/program_loader/elf/load_elf.rs:128
( 13) /root/asterinas/kernel/src/process/program_loader/elf/load_elf.rs:46
( 14) /root/asterinas/kernel/src/process/program_loader/mod.rs:66
( 15) /root/asterinas/kernel/src/syscall/execve.rs:117
( 16) /root/asterinas/kernel/src/syscall/execve.rs:33
( 17) /root/asterinas/kernel/src/syscall/mod.rs:175
( 18) /root/asterinas/kernel/src/syscall/mod.rs:329
( 19) /root/asterinas/kernel/src/thread/task.rs:70
( 20) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:250
( 21) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.4/src/panicking.rs:56
( 22) 97rarjgzbtso0b5qwey8u6xa1:?
( 23) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.4/src/panicking.rs:42
( 24) /root/.cargo/registry/src/index.crates.io-6f17d22bba15001f/unwinding-0.2.4/src/panic.rs:87
( 25) /root/asterinas/kernel/src/thread/oops.rs:54
( 26) /root/asterinas/kernel/src/thread/task.rs:95
( 27) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/core/src/ops/function.rs:250
( 28) /root/.rustup/toolchains/nightly-2024-10-12-x86_64-unknown-linux-gnu/lib/rustlib/src/rust/library/alloc/src/boxed.rs:2454
( 29) /root/asterinas/ostd/src/task/mod.rs:178
make: *** [Makefile:210: run] Error 1
```