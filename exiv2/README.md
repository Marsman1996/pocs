# poc3-us2Data

## Test Environment
Ubuntu 16.04, 64bit  
Exiv2 (master b6a8d39)

## How to trigger
`$ exiv2 $POC`

## Reference
https://github.com/Exiv2/exiv2/issues/452  

## Credits
Yanhao(unfuzzable123@gmail.com)  
Marsman1996(lqliuyuwei@outlook.com)

## Details
### Asan report
```
./build_asan/bin/exiv2 ../poc3-us2Data 
=================================================================
==32757==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x60200000ed9d at pc 0x7ffa7b2f16b5 bp 0x7ffce41c48e0 sp 0x7ffce41c48d0
WRITE of size 1 at 0x60200000ed9d thread T0
    #0 0x7ffa7b2f16b4 in Exiv2::us2Data(unsigned char*, unsigned short, Exiv2::ByteOrder) /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/types.cpp:368
    #1 0x7ffa7b251c52 in long Exiv2::toData<unsigned short>(unsigned char*, unsigned short, Exiv2::ByteOrder) /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/include/exiv2/value.hpp:1449
    #2 0x7ffa7b261cd9 in Exiv2::ValueType<unsigned short>::copy(unsigned char*, Exiv2::ByteOrder) const /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/include/exiv2/value.hpp:1611
    #3 0x7ffa7b24be58 in Exiv2::Exifdatum::copy(unsigned char*, Exiv2::ByteOrder) const /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/exif.cpp:357
    #4 0x7ffa7b2ed083 in Exiv2::TiffImage::readMetadata() /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/tiffimage.cpp:199
    #5 0x43a413 in Action::Print::printSummary() /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/actions.cpp:287
    #6 0x439c8b in Action::Print::run(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/actions.cpp:247
    #7 0x4222d7 in main /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/exiv2.cpp:167
    #8 0x7ffa7a5d482f in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2082f)
    #9 0x421ce8 in _start (/home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/build_asan/bin/exiv2+0x421ce8)

0x60200000ed9d is located 0 bytes to the right of 13-byte region [0x60200000ed90,0x60200000ed9d)
allocated by thread T0 here:
    #0 0x7ffa7ba316b2 in operator new[](unsigned long) (/usr/lib/x86_64-linux-gnu/libasan.so.2+0x996b2)
    #1 0x7ffa7b2ef51f in Exiv2::DataBuf::alloc(long) /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/types.cpp:156
    #2 0x7ffa7b2ecfe6 in Exiv2::TiffImage::readMetadata() /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/tiffimage.cpp:198
    #3 0x43a413 in Action::Print::printSummary() /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/actions.cpp:287
    #4 0x439c8b in Action::Print::run(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/actions.cpp:247
    #5 0x4222d7 in main /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/exiv2.cpp:167
    #6 0x7ffa7a5d482f in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2082f)

SUMMARY: AddressSanitizer: heap-buffer-overflow /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/types.cpp:368 Exiv2::us2Data(unsigned char*, unsigned short, Exiv2::ByteOrder)
Shadow bytes around the buggy address:
  0x0c047fff9d60: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff9d70: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff9d80: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff9d90: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff9da0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
=>0x0c047fff9db0: fa fa 00[05]fa fa 00 00 fa fa fd fd fa fa 02 fa
  0x0c047fff9dc0: fa fa 00 00 fa fa fd fa fa fa fd fd fa fa fd fd
  0x0c047fff9dd0: fa fa fd fa fa fa fd fa fa fa fd fa fa fa fd fa
  0x0c047fff9de0: fa fa fd fd fa fa fd fa fa fa fd fd fa fa fd fa
  0x0c047fff9df0: fa fa 00 fa fa fa 00 fa fa fa 00 fa fa fa fd fd
  0x0c047fff9e00: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07 
  Heap left redzone:       fa
  Heap right redzone:      fb
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack partial redzone:   f4
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
==32757==ABORTING
```

# poc5-l2Data

## Test Environment
Ubuntu 16.04, 64bit  
Exiv2 (master b6a8d39)

## How to trigger
`$ exiv2 $POC`

## Reference
https://github.com/Exiv2/exiv2/issues/454  

## Credits
Yanhao(unfuzzable123@gmail.com)  
Marsman1996(lqliuyuwei@outlook.com)

## Details
### Asan report
```
./build_asan/bin/exiv2 ../poc5-l2Data 
=================================================================
==32768==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x60200000edc0 at pc 0x7f6ccb2dacb5 bp 0x7fffcc342080 sp 0x7fffcc342070
WRITE of size 1 at 0x60200000edc0 thread T0
    #0 0x7f6ccb2dacb4 in Exiv2::l2Data(unsigned char*, int, Exiv2::ByteOrder) /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/types.cpp:419
    #1 0x7f6ccb23acf9 in long Exiv2::toData<int>(unsigned char*, int, Exiv2::ByteOrder) (/home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/build_asan/lib/libexiv2.so.0+0x33bcf9)
    #2 0x7f6ccb24836f in Exiv2::ValueType<int>::copy(unsigned char*, Exiv2::ByteOrder) const /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/include/exiv2/value.hpp:1611
    #3 0x7f6ccb234e58 in Exiv2::Exifdatum::copy(unsigned char*, Exiv2::ByteOrder) const /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/exif.cpp:357
    #4 0x7f6ccb2d6083 in Exiv2::TiffImage::readMetadata() /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/tiffimage.cpp:199
    #5 0x43a413 in Action::Print::printSummary() /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/actions.cpp:287
    #6 0x439c8b in Action::Print::run(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/actions.cpp:247
    #7 0x4222d7 in main /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/exiv2.cpp:167
    #8 0x7f6cca5bd82f in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2082f)
    #9 0x421ce8 in _start (/home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/build_asan/bin/exiv2+0x421ce8)

0x60200000edc0 is located 0 bytes to the right of 16-byte region [0x60200000edb0,0x60200000edc0)
allocated by thread T0 here:
    #0 0x7f6ccba1a6b2 in operator new[](unsigned long) (/usr/lib/x86_64-linux-gnu/libasan.so.2+0x996b2)
    #1 0x7f6ccb2d851f in Exiv2::DataBuf::alloc(long) /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/types.cpp:156
    #2 0x7f6ccb2d5fe6 in Exiv2::TiffImage::readMetadata() /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/tiffimage.cpp:198
    #3 0x43a413 in Action::Print::printSummary() /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/actions.cpp:287
    #4 0x439c8b in Action::Print::run(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/actions.cpp:247
    #5 0x4222d7 in main /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/exiv2.cpp:167
    #6 0x7f6cca5bd82f in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2082f)

SUMMARY: AddressSanitizer: heap-buffer-overflow /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/types.cpp:419 Exiv2::l2Data(unsigned char*, int, Exiv2::ByteOrder)
Shadow bytes around the buggy address:
  0x0c047fff9d60: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff9d70: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff9d80: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff9d90: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff9da0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
=>0x0c047fff9db0: fa fa fa fa fa fa 00 00[fa]fa 00 00 fa fa fd fd
  0x0c047fff9dc0: fa fa 02 fa fa fa 00 00 fa fa fd fa fa fa fd fd
  0x0c047fff9dd0: fa fa fd fd fa fa fd fa fa fa fd fa fa fa fd fa
  0x0c047fff9de0: fa fa fd fd fa fa fd fa fa fa fd fd fa fa fd fa
  0x0c047fff9df0: fa fa 00 fa fa fa 00 fa fa fa 00 fa fa fa fd fd
  0x0c047fff9e00: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07 
  Heap left redzone:       fa
  Heap right redzone:      fb
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack partial redzone:   f4
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
==32768==ABORTING
```

# poc7-s2Data

## Test Environment
Ubuntu 16.04, 64bit  
Exiv2 (master b6a8d39)

## How to trigger
`$ exiv2 $POC`

## Reference
https://github.com/Exiv2/exiv2/issues/456  

## Credits
Yanhao(unfuzzable123@gmail.com)  
Marsman1996(lqliuyuwei@outlook.com)

## Details
### Asan report
```
./build_asan/bin/exiv2 ../poc7-s2Data 
=================================================================
==32777==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x611000009e8e at pc 0x7f0eef7aaae2 bp 0x7fff03e52fa0 sp 0x7fff03e52f90
WRITE of size 1 at 0x611000009e8e thread T0
    #0 0x7f0eef7aaae1 in Exiv2::s2Data(unsigned char*, short, Exiv2::ByteOrder) /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/types.cpp:404
    #1 0x7f0eef70acd1 in long Exiv2::toData<short>(unsigned char*, short, Exiv2::ByteOrder) (/home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/build_asan/lib/libexiv2.so.0+0x33bcd1)
    #2 0x7f0eef719103 in Exiv2::ValueType<short>::copy(unsigned char*, Exiv2::ByteOrder) const /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/include/exiv2/value.hpp:1611
    #3 0x7f0eef704e58 in Exiv2::Exifdatum::copy(unsigned char*, Exiv2::ByteOrder) const /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/exif.cpp:357
    #4 0x7f0eef7a6083 in Exiv2::TiffImage::readMetadata() /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/tiffimage.cpp:199
    #5 0x43a413 in Action::Print::printSummary() /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/actions.cpp:287
    #6 0x439c8b in Action::Print::run(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/actions.cpp:247
    #7 0x4222d7 in main /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/exiv2.cpp:167
    #8 0x7f0eeea8d82f in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2082f)
    #9 0x421ce8 in _start (/home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/build_asan/bin/exiv2+0x421ce8)

0x611000009e8e is located 0 bytes to the right of 206-byte region [0x611000009dc0,0x611000009e8e)
allocated by thread T0 here:
    #0 0x7f0eefeea6b2 in operator new[](unsigned long) (/usr/lib/x86_64-linux-gnu/libasan.so.2+0x996b2)
    #1 0x7f0eef7a851f in Exiv2::DataBuf::alloc(long) /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/types.cpp:156
    #2 0x7f0eef7a5fe6 in Exiv2::TiffImage::readMetadata() /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/tiffimage.cpp:198
    #3 0x43a413 in Action::Print::printSummary() /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/actions.cpp:287
    #4 0x439c8b in Action::Print::run(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/actions.cpp:247
    #5 0x4222d7 in main /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/exiv2.cpp:167
    #6 0x7f0eeea8d82f in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2082f)

SUMMARY: AddressSanitizer: heap-buffer-overflow /home/marsman/Desktop/crashana/exiv2/exiv2-b6a8d39613a28603a989bd5278798db8a54eeb1e/src/types.cpp:404 Exiv2::s2Data(unsigned char*, short, Exiv2::ByteOrder)
Shadow bytes around the buggy address:
  0x0c227fff9380: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c227fff9390: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c227fff93a0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c227fff93b0: fa fa fa fa fa fa fa fa 00 00 00 00 00 00 00 00
  0x0c227fff93c0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x0c227fff93d0: 00[06]fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c227fff93e0: fd fd fd fd fd fd fd fd fd fd fd fd fd fd fd fd
  0x0c227fff93f0: fd fd fd fd fd fd fd fd fd fd fd fd fd fd fd fd
  0x0c227fff9400: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c227fff9410: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c227fff9420: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07 
  Heap left redzone:       fa
  Heap right redzone:      fb
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack partial redzone:   f4
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
==32777==ABORTING
```