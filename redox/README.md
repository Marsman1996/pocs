# syscall-munmap-f3fc45a-round_up_to_page_size-integer_overflow
Integer overflow in `round_up_to_page_size()`

### Describe the bug
There is an integer overflow in `round_up_to_page_size()` at src/platform/redox/mod.rs:48 when program calls memory related syscall (i.e., `mmap`, `munmap`, and `mprotect`) with large `len`.

https://gitlab.redox-os.org/redox-os/relibc/-/blob/master/src/platform/redox/mod.rs?ref_type=heads#L47-49

### To Reproduce
1. Compile a program which calls system call `munmap` with large `len`
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
2. Run the compiled program

### Logs
I add `overflow-checks = true` for relibc.
```log
user:~# munmap
RELIBC PANIC: panicked at src/platform/redox/mod.rs:48:6:
attempt to add with overflow
```

# syscall-setitimer-7af6dd1-in_exact_chunks-divide_by_zero
Divide by zero panic in `in_exact_chunks()`

### Describe the bug
There is an divide by zero panic in `in_exact_chunks()` at src/syscall/usercopy.rs:86:24 when program calls `setitimer` relibc syscall.

https://gitlab.redox-os.org/redox-os/kernel/-/blob/master/src/syscall/usercopy.rs?ref_type=heads#L85-92

### To Reproduce
1. Compile a program which calls system call `setitimer`
```C
#include <signal.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

void timer_handler(int signum) {
  static int count = 0;
  printf("Timer expired %d times\n", ++count);
}

int main() {
  struct itimerval timer;
  printf("\n\n?\n\n");
  signal(SIGALRM, timer_handler);
  printf("\n\n?\n\n");
  timer.it_value.tv_sec = -1;
  timer.it_value.tv_usec = 0x7FFFFFFF;
  timer.it_interval.tv_sec = -1;
  timer.it_interval.tv_usec = 0x7FFFFFFF;

  printf("\n\n?\n\n");
  setitimer(ITIMER_REAL, &timer, &timer);
  printf("\n\n?\n\n");

  return 0;
}
```
The `recipe` is:
```toml
[build]
template = "custom"
script = """
"${CXX}" \
    "${COOKBOOK_RECIPE}/setitimer.c" \
    -o setitimer \
    -static
mkdir -pv "${COOKBOOK_STAGE}/bin"
cp -v "setitimer" "${COOKBOOK_STAGE}/bin/setitimer"
"""

```
2. Run the compiled program

### Environment
- Official Podman
- Intel(R) Xeon(R) Gold 6230R CPU @ 2.10GHz
- Redox kernel version: main 7af6dd1

### Logs
```log
user:~# setitimer


?



?



?
KERNEL PANIC: panicked at src/syscall/usercopy.rs:86:24:
attempt to divide by zero

  FP ffff80000c9efc10: PC ffffffff800a6f04
    FFFFFFFF800A6DC0+0144
    rust_begin_unwind
  FP ffff80000c9efd00: PC ffffffff800116cf
  FP ffff80000c9efd40: PC ffffffff80012fcc
  FP ffff80000c9efd80: PC ffffffff80087613
  FP ffff80000c9efd90: PC ffffffff8008b7d5
    FFFFFFFF8008B7B0+0025
    kernel::scheme::KernelScheme::kreadoff
  FP ffff80000c9efdb0: PC ffffffff800949f5
    FFFFFFFF800945D0+0425
    kernel::syscall::fs::sys_read
  FP ffff80000c9efe50: PC ffffffff80062bac
    FFFFFFFF80062130+0A7C
    __inner_syscall_instruction
  FP ffff80000c9eff50: PC ffffffff80062f93
    FFFFFFFF80062F50+0043
    kernel::arch::x86_64::interrupt::syscall::syscall_instruction
  00007ffffffffe20: GUARD PAGE
CPU #3, CID 0xffffff7f801423a0
NAME: /usr/bin/setitimer
SYSCALL: read(4, 0x7FFFFFFFFDC8, 32)
HALT
```

# syscall-setsockopt-32fca670-setsockopt-integer_overflow
Integer overflow in `setsockopt()`

### Describe the bug
There is an integer overflow error in `setsockopt()` at src/platform/redox/socket.rs:364:26 when program calls `setsockopt` relibc syscall with large `timeval.tv_usec`.

https://gitlab.redox-os.org/redox-os/relibc/-/blob/master/src/platform/redox/socket.rs?ref_type=heads#L362-365

### To Reproduce
1. Compile a program which calls system call `setsockopt` with large `tv_usec` (i.e., 0x7fffffff)
```C
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

int main() {
  int sockfd;
  struct timeval timeout;

  timeout.tv_sec = 5;
  timeout.tv_usec = 0x7fffffff;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

  return 0;
}
```
2. Run the compiled program

### Environment
- Official Podman
- Intel(R) Xeon(R) Gold 6230R CPU @ 2.10GHz
- Redox relibc version: main 32fca670

### Logs
I add `overflow-checks = true` for relibc.
```log
user:~# setsockopt
RELIBC PANIC: panicked at src/platform/redox/socket.rs:364:26:
attempt to multiply with overflow
```

# 