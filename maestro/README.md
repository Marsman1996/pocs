# syscall-munmap-e7ebdfa-munmap-unwrap
Integer overflow caused by multiply in `munmap()`

### Reference


### Describe the bug
There is a multiply with overflow problem in kernel/src/syscall/munmap.rs, `munmap()`, Maestro.
The `div_ceil()` rounds the result towards positive infinity. 
As a result, when user calls munmap with large `length` (i.e., 0xfffffff0), the following multiplication operation will cause an integer overflow problem.

https://github.com/maestro-os/maestro/blob/e7ebdfacc22040eeb2dcbe3be17e29c2c3192767/kernel/src/syscall/munmap.rs#L45-L46

### To Reproduce
1. Call system call `munmap` with large `length` (i.e., usize::MAX)

### Environment
- Maestro version: main e7ebdfa
- ubuntu:24.04 in Docker 
- 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
