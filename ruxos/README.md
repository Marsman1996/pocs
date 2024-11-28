# syscall-munmap-b1f880b-sys_munmap-unwrap
Reachable unwrap panic in `sys_munmap()` at legacy.rs

### Reference


### Describe the bug
There is a reachable unwrap panic in `sys_munmap()` caused by aligning large `len` (i.e., 0xffffffffffffffff) of a `munmap` syscall.

https://github.com/syswonder/ruxos/blob/b1f880b603cc1e578af16292f3b8a848766ea285/api/ruxos_posix_api/src/imp/mmap/legacy.rs#L72

### To Reproduce
1. Compile a program which calls `munmap`: 
```C
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

int main()
{
    void *addr = mmap(NULL, 4096, 0x3, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    munmap(addr, 0xffffffffffffffff);

    return 0;
}
```
With following features:
```
alloc 
irq
musl
multitask
fs
pipe
poll
rtc
signal
virtio-9p
```
2. Run the compiled program in RuxOS.

### Expected behavior

RuxOS reports panic and is terminated.

### Environment
- RuxOS version: main b1f880b
- ubuntu:22.04 in Docker 
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz

### Logs
```log
[1728388549.205894 0:2 ruxos_posix_api::imp::mmap::legacy:67] sys_munmap <= start: 0xffffff80003fe000, len: 18446744073709551615
[1728388549.206739 0:2 ruxruntime::lang_items:14] panicked at api/ruxos_posix_api/src/imp/mmap/legacy.rs:72:54:
called `Result::unwrap()` on an `Err` value: LayoutError
[1728388549.207796 0:2 ruxhal::platform::x86_pc::misc:16] Shutting down...
```

# syscall-mmap-b1f880b-sys_mmap-unwrap
Reachable unwrap panic in `sys_mmap()` at legacy.rs

### To Reproduce
1. Compile a program which calls `munmap`: 
```C
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

int main()
{
    mmap(NULL, 0x7ffffffffffffffe, 0x3, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    return 0;
}
```
With following features:
```
alloc 
irq
musl
multitask
pipe
poll
rtc
signal
virtio-9p
```
2. Run the compiled program in RuxOS.

### Logs
```log
[1728436128.205115 0:2 ruxmusl::x86_64:10] x86 syscall <= syscall_name: MMAP
[1728436128.205642 0:2 ruxos_posix_api::imp::mmap::legacy:32] sys_mmap <= start: 0x0, len: 9223372036854775806, fd: -1
[1728436128.206396 0:2 ruxruntime::lang_items:14] panicked at api/ruxos_posix_api/src/imp/mmap/legacy.rs:44:54:
called `Result::unwrap()` on an `Err` value: LayoutError
[1728436128.207400 0:2 ruxhal::platform::x86_pc::misc:16] Shutting down...
```

# 