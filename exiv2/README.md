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

# poc-API-41a8a4b-TiffComponent::accept-UAF (CVE-2025-26623)
Use after free error when calling API `Exiv2::TiffParser::encode`

## Reference
https://github.com/Exiv2/exiv2/issues/3168

## Details

#### **Describe the bug**
There is a use-after-free problem in `src/tiffcomposite_int.cpp, Exiv2::Internal::TiffComponent::accept` when calling API `Exiv2::TiffParser::encode`.

#### **To Reproduce**
1. Compile Exiv2 with Address Sanitizer:
```
$ mkdir build && cd build
$ CXX="clang++ -fsanitize=address,fuzzer-no-link -g " CXXFLAGS="-g -O0" cmake ../ -DBUILD_SHARED_LIBS=OFF -DEXIV2_ENABLE_BROTLI=OFF
$ make -j
```
2. Compile the fuzz driver that calls `Exiv2::TiffParser::encode`:  
The fuzz driver code:
```C++
// This fuzz driver is generated for library exiv2, aiming to fuzz the following functions:
// Exiv2::TiffParser::encode at tiffimage.cpp:219:25 in tiffimage.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <exiv2/exif.hpp>
#include <exiv2/image.hpp>
#include <exiv2/tiffimage.hpp>
#include <cstdint>
#include <cstdlib>
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize necessary data structures
    Exiv2::ExifData exifData;
    Exiv2::IptcData iptcData;
    Exiv2::XmpData xmpData;
    Exiv2::MemIo memIo(Data, Size);

    // Fuzz Exiv2::TiffParser::encode
    try {
        Exiv2::BasicIo::UniquePtr io(new Exiv2::MemIo(Data, Size));
        Exiv2::TiffParser::encode(*io, Data, Size, Exiv2::littleEndian, exifData, iptcData, xmpData);
    } catch (...) {
        // Ignore exceptions
    }

    return 0;
}
```
The compile command is:
```
$ clang++ ./driver-API-41a8a4b-TiffComponent::accept-UAF.cc -o driver-API-41a8a4b-TiffComponent::accept-UAF -fsanitize=fuzzer,address,undefined -g ./lib/libexiv2.a -I ../include/ -I ./ -lfmt -lexpat -lz -lINIReader
```
3. Run the fuzz driver with given [PoC file](https://github.com/Marsman1996/pocs/raw/refs/heads/master/exiv2/poc-API-41a8a4b-TiffComponent::accept-UAF):
```
$ ./driver-API-41a8a4b-TiffComponent::accept-UAF ./poc-API-41a8a4b-TiffComponent::accept-UAF
```

#### **Desktop (please complete the following information):**
- OS and version: Linux (Ubuntu 24.04.1 LTS)
- Exiv2 version and source: exiv2 1.00.0.9, master 41a8a4b
- Compiler and version: Clang 18.1.3
- Compilation mode and/or compiler flags: `CXX="clang++ -fsanitize=address,fuzzer-no-link -g " CXXFLAGS="-g -O0" cmake ../ -DBUILD_SHARED_LIBS=OFF -DEXIV2_ENABLE_BROTLI=OFF`

#### **Additional context**
ASan Report:
```
==3699198==ERROR: AddressSanitizer: heap-use-after-free on address 0x5060000007a0 at pc 0x5562460a338a bp 0x7fff5fcf6fe0 sp 0x7fff5fcf6fd8
READ of size 8 at 0x5060000007a0 thread T0
    #0 0x5562460a3389 in Exiv2::Internal::TiffComponent::accept(Exiv2::Internal::TiffVisitor&) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffcomposite_int.cpp:628:5
    #1 0x5562460a3389 in Exiv2::Internal::TiffSubIfd::doAccept(Exiv2::Internal::TiffVisitor&) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffcomposite_int.cpp:667:10
    #2 0x5562460a2f59 in Exiv2::Internal::TiffComponent::accept(Exiv2::Internal::TiffVisitor&) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffcomposite_int.cpp:628:5
    #3 0x5562460a2f59 in Exiv2::Internal::TiffDirectory::doAccept(Exiv2::Internal::TiffVisitor&) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffcomposite_int.cpp:652:16
    #4 0x5562460a30a3 in Exiv2::Internal::TiffComponent::accept(Exiv2::Internal::TiffVisitor&) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffcomposite_int.cpp:628:5
    #5 0x5562460a30a3 in Exiv2::Internal::TiffDirectory::doAccept(Exiv2::Internal::TiffVisitor&) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffcomposite_int.cpp:657:13
    #6 0x556245dc243c in Exiv2::Internal::TiffParserWorker::encode(Exiv2::BasicIo&, unsigned char const*, unsigned long, Exiv2::ExifData&, Exiv2::IptcData&, Exiv2::XmpData&, unsigned int, void (Exiv2::Internal::TiffEncoder::* (*)(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, unsigned int, Exiv2::IfdId))(Exiv2::Internal::TiffEntryBase*, Exiv2::Exifdatum const*), Exiv2::Internal::TiffHeaderBase*, Exiv2::Internal::OffsetWriter*) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffimage_int.cpp:2074:19
    #7 0x556245ca365a in Exiv2::TiffParser::encode(Exiv2::BasicIo&, unsigned char const*, unsigned long, Exiv2::ByteOrder, Exiv2::ExifData&, Exiv2::IptcData&, Exiv2::XmpData&) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffimage.cpp:233:10
    #8 0x556245bad540 in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/build/./driver-API-41a8a4b-TiffComponent::accept-UAF.cc:31:9
    #9 0x556245ab8614 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/exiv2/exiv2/build/driver-API-41a8a4b-TiffComponent::accept-UAF+0x4a4614) (BuildId: 2c25e32e6342924a562a6cd6d5b6dd3e80f4979b)
    #10 0x556245aa1746 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/exiv2/exiv2/build/driver-API-41a8a4b-TiffComponent::accept-UAF+0x48d746) (BuildId: 2c25e32e6342924a562a6cd6d5b6dd3e80f4979b)
    #11 0x556245aa71fa in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/exiv2/exiv2/build/driver-API-41a8a4b-TiffComponent::accept-UAF+0x4931fa) (BuildId: 2c25e32e6342924a562a6cd6d5b6dd3e80f4979b)
    #12 0x556245ad19b6 in main (/home/yuwei/afgen/afgenllm/database/exiv2/exiv2/build/driver-API-41a8a4b-TiffComponent::accept-UAF+0x4bd9b6) (BuildId: 2c25e32e6342924a562a6cd6d5b6dd3e80f4979b)
    #13 0x7f9213e2a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #14 0x7f9213e2a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #15 0x556245a9c314 in _start (/home/yuwei/afgen/afgenllm/database/exiv2/exiv2/build/driver-API-41a8a4b-TiffComponent::accept-UAF+0x488314) (BuildId: 2c25e32e6342924a562a6cd6d5b6dd3e80f4979b)

0x5060000007a0 is located 0 bytes inside of 64-byte region [0x5060000007a0,0x5060000007e0)
freed by thread T0 here:
    #0 0x556245bab5c1 in operator delete(void*) (/home/yuwei/afgen/afgenllm/database/exiv2/exiv2/build/driver-API-41a8a4b-TiffComponent::accept-UAF+0x5975c1) (BuildId: 2c25e32e6342924a562a6cd6d5b6dd3e80f4979b)
    #1 0x55624608e643 in Exiv2::Internal::TiffSubIfd::~TiffSubIfd() /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffcomposite_int.cpp:100:5
    #2 0x55624608eacb in Exiv2::Internal::TiffSubIfd::~TiffSubIfd() /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffcomposite_int.cpp:98:27
    #3 0x55624609d038 in std::default_delete<Exiv2::Internal::TiffComponent>::operator()(Exiv2::Internal::TiffComponent*) const /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/unique_ptr.h:99:2
    #4 0x55624609d038 in std::unique_ptr<Exiv2::Internal::TiffComponent, std::default_delete<Exiv2::Internal::TiffComponent>>::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/unique_ptr.h:404:4
    #5 0x55624609d038 in Exiv2::Internal::TiffDirectory::doAddPath(unsigned short, std::stack<Exiv2::Internal::TiffPathItem, std::deque<Exiv2::Internal::TiffPathItem, std::allocator<Exiv2::Internal::TiffPathItem>>>&, Exiv2::Internal::TiffComponent*, std::unique_ptr<Exiv2::Internal::TiffComponent, std::default_delete<Exiv2::Internal::TiffComponent>>) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffcomposite_int.cpp:471:1
    #6 0x55624609c89c in Exiv2::Internal::TiffComponent::addPath(unsigned short, std::stack<Exiv2::Internal::TiffPathItem, std::deque<Exiv2::Internal::TiffPathItem, std::allocator<Exiv2::Internal::TiffPathItem>>>&, Exiv2::Internal::TiffComponent*, std::unique_ptr<Exiv2::Internal::TiffComponent, std::default_delete<Exiv2::Internal::TiffComponent>>) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffcomposite_int.cpp:417:10
    #7 0x55624609c89c in Exiv2::Internal::TiffDirectory::doAddPath(unsigned short, std::stack<Exiv2::Internal::TiffPathItem, std::deque<Exiv2::Internal::TiffPathItem, std::allocator<Exiv2::Internal::TiffPathItem>>>&, Exiv2::Internal::TiffComponent*, std::unique_ptr<Exiv2::Internal::TiffComponent, std::default_delete<Exiv2::Internal::TiffComponent>>) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffcomposite_int.cpp:449:16
    #8 0x55624609c1b6 in Exiv2::Internal::TiffComponent::addPath(unsigned short, std::stack<Exiv2::Internal::TiffPathItem, std::deque<Exiv2::Internal::TiffPathItem, std::allocator<Exiv2::Internal::TiffPathItem>>>&, Exiv2::Internal::TiffComponent*, std::unique_ptr<Exiv2::Internal::TiffComponent, std::default_delete<Exiv2::Internal::TiffComponent>>) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffcomposite_int.cpp:417:10
    #9 0x556245dcc014 in Exiv2::Internal::TiffCopier::copyObject(Exiv2::Internal::TiffComponent const*) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffvisitor_int.cpp:135:13
    #10 0x5562460a3215 in Exiv2::Internal::TiffSubIfd::doAccept(Exiv2::Internal::TiffVisitor&) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffcomposite_int.cpp:663:11
    #11 0x5562460a2f59 in Exiv2::Internal::TiffComponent::accept(Exiv2::Internal::TiffVisitor&) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffcomposite_int.cpp:628:5
    #12 0x5562460a2f59 in Exiv2::Internal::TiffDirectory::doAccept(Exiv2::Internal::TiffVisitor&) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffcomposite_int.cpp:652:16
    #13 0x5562460a30a3 in Exiv2::Internal::TiffComponent::accept(Exiv2::Internal::TiffVisitor&) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffcomposite_int.cpp:628:5
    #14 0x5562460a30a3 in Exiv2::Internal::TiffDirectory::doAccept(Exiv2::Internal::TiffVisitor&) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffcomposite_int.cpp:657:13
    #15 0x556245dc243c in Exiv2::Internal::TiffParserWorker::encode(Exiv2::BasicIo&, unsigned char const*, unsigned long, Exiv2::ExifData&, Exiv2::IptcData&, Exiv2::XmpData&, unsigned int, void (Exiv2::Internal::TiffEncoder::* (*)(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, unsigned int, Exiv2::IfdId))(Exiv2::Internal::TiffEntryBase*, Exiv2::Exifdatum const*), Exiv2::Internal::TiffHeaderBase*, Exiv2::Internal::OffsetWriter*) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffimage_int.cpp:2074:19
    #16 0x556245ca365a in Exiv2::TiffParser::encode(Exiv2::BasicIo&, unsigned char const*, unsigned long, Exiv2::ByteOrder, Exiv2::ExifData&, Exiv2::IptcData&, Exiv2::XmpData&) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffimage.cpp:233:10
    #17 0x556245bad540 in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/build/./driver-API-41a8a4b-TiffComponent::accept-UAF.cc:31:9
    #18 0x556245ab8614 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/exiv2/exiv2/build/driver-API-41a8a4b-TiffComponent::accept-UAF+0x4a4614) (BuildId: 2c25e32e6342924a562a6cd6d5b6dd3e80f4979b)
    #19 0x556245aa1746 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/exiv2/exiv2/build/driver-API-41a8a4b-TiffComponent::accept-UAF+0x48d746) (BuildId: 2c25e32e6342924a562a6cd6d5b6dd3e80f4979b)
    #20 0x556245aa71fa in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/exiv2/exiv2/build/driver-API-41a8a4b-TiffComponent::accept-UAF+0x4931fa) (BuildId: 2c25e32e6342924a562a6cd6d5b6dd3e80f4979b)
    #21 0x556245ad19b6 in main (/home/yuwei/afgen/afgenllm/database/exiv2/exiv2/build/driver-API-41a8a4b-TiffComponent::accept-UAF+0x4bd9b6) (BuildId: 2c25e32e6342924a562a6cd6d5b6dd3e80f4979b)
    #22 0x7f9213e2a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #23 0x7f9213e2a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #24 0x556245a9c314 in _start (/home/yuwei/afgen/afgenllm/database/exiv2/exiv2/build/driver-API-41a8a4b-TiffComponent::accept-UAF+0x488314) (BuildId: 2c25e32e6342924a562a6cd6d5b6dd3e80f4979b)

previously allocated by thread T0 here:
    #0 0x556245baad41 in operator new(unsigned long) (/home/yuwei/afgen/afgenllm/database/exiv2/exiv2/build/driver-API-41a8a4b-TiffComponent::accept-UAF+0x596d41) (BuildId: 2c25e32e6342924a562a6cd6d5b6dd3e80f4979b)
    #1 0x556245de7188 in std::__detail::_MakeUniq<Exiv2::Internal::TiffDirectory>::__single_object std::make_unique<Exiv2::Internal::TiffDirectory, unsigned short, Exiv2::IfdId>(unsigned short&&, Exiv2::IfdId&&) /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/unique_ptr.h:1070:30
    #2 0x556245de7188 in Exiv2::Internal::TiffReader::visitSubIfd(Exiv2::Internal::TiffSubIfd*) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffvisitor_int.cpp:1163:17
    #3 0x5562460a3215 in Exiv2::Internal::TiffSubIfd::doAccept(Exiv2::Internal::TiffVisitor&) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffcomposite_int.cpp:663:11
    #4 0x5562460a2f59 in Exiv2::Internal::TiffComponent::accept(Exiv2::Internal::TiffVisitor&) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffcomposite_int.cpp:628:5
    #5 0x5562460a2f59 in Exiv2::Internal::TiffDirectory::doAccept(Exiv2::Internal::TiffVisitor&) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffcomposite_int.cpp:652:16
    #6 0x5562460a30a3 in Exiv2::Internal::TiffComponent::accept(Exiv2::Internal::TiffVisitor&) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffcomposite_int.cpp:628:5
    #7 0x5562460a30a3 in Exiv2::Internal::TiffDirectory::doAccept(Exiv2::Internal::TiffVisitor&) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffcomposite_int.cpp:657:13
    #8 0x556245dc1b38 in Exiv2::Internal::TiffParserWorker::parse(unsigned char const*, unsigned long, unsigned int, Exiv2::Internal::TiffHeaderBase*) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffimage_int.cpp:2114:14
    #9 0x556245dc21a3 in Exiv2::Internal::TiffParserWorker::encode(Exiv2::BasicIo&, unsigned char const*, unsigned long, Exiv2::ExifData&, Exiv2::IptcData&, Exiv2::XmpData&, unsigned int, void (Exiv2::Internal::TiffEncoder::* (*)(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, unsigned int, Exiv2::IfdId))(Exiv2::Internal::TiffEntryBase*, Exiv2::Exifdatum const*), Exiv2::Internal::TiffHeaderBase*, Exiv2::Internal::OffsetWriter*) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffimage_int.cpp:2060:21
    #10 0x556245ca365a in Exiv2::TiffParser::encode(Exiv2::BasicIo&, unsigned char const*, unsigned long, Exiv2::ByteOrder, Exiv2::ExifData&, Exiv2::IptcData&, Exiv2::XmpData&) /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffimage.cpp:233:10
    #11 0x556245bad540 in LLVMFuzzerTestOneInput /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/build/./driver-API-41a8a4b-TiffComponent::accept-UAF.cc:31:9
    #12 0x556245ab8614 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/exiv2/exiv2/build/driver-API-41a8a4b-TiffComponent::accept-UAF+0x4a4614) (BuildId: 2c25e32e6342924a562a6cd6d5b6dd3e80f4979b)
    #13 0x556245aa1746 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/yuwei/afgen/afgenllm/database/exiv2/exiv2/build/driver-API-41a8a4b-TiffComponent::accept-UAF+0x48d746) (BuildId: 2c25e32e6342924a562a6cd6d5b6dd3e80f4979b)
    #14 0x556245aa71fa in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/yuwei/afgen/afgenllm/database/exiv2/exiv2/build/driver-API-41a8a4b-TiffComponent::accept-UAF+0x4931fa) (BuildId: 2c25e32e6342924a562a6cd6d5b6dd3e80f4979b)
    #15 0x556245ad19b6 in main (/home/yuwei/afgen/afgenllm/database/exiv2/exiv2/build/driver-API-41a8a4b-TiffComponent::accept-UAF+0x4bd9b6) (BuildId: 2c25e32e6342924a562a6cd6d5b6dd3e80f4979b)
    #16 0x7f9213e2a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #17 0x7f9213e2a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #18 0x556245a9c314 in _start (/home/yuwei/afgen/afgenllm/database/exiv2/exiv2/build/driver-API-41a8a4b-TiffComponent::accept-UAF+0x488314) (BuildId: 2c25e32e6342924a562a6cd6d5b6dd3e80f4979b)

SUMMARY: AddressSanitizer: heap-use-after-free /home/yuwei/afgen/afgenllm/database/exiv2/exiv2/src/tiffcomposite_int.cpp:628:5 in Exiv2::Internal::TiffComponent::accept(Exiv2::Internal::TiffVisitor&)
Shadow bytes around the buggy address:
  0x506000000500: fd fd fd fd fd fd fd fd fa fa fa fa fd fd fd fd
  0x506000000580: fd fd fd fd fa fa fa fa fd fd fd fd fd fd fd fd
  0x506000000600: fa fa fa fa 00 00 00 00 00 00 00 fa fa fa fa fa
  0x506000000680: 00 00 00 00 00 00 00 fa fa fa fa fa 00 00 00 00
  0x506000000700: 00 00 00 fa fa fa fa fa 00 00 00 00 00 00 00 fa
=>0x506000000780: fa fa fa fa[fd]fd fd fd fd fd fd fd fa fa fa fa
  0x506000000800: 00 00 00 00 00 00 00 00 fa fa fa fa fd fd fd fd
  0x506000000880: fd fd fd fd fa fa fa fa fd fd fd fd fd fd fd fd
  0x506000000900: fa fa fa fa fd fd fd fd fd fd fd fd fa fa fa fa
  0x506000000980: 00 00 00 00 00 00 00 fa fa fa fa fa fd fd fd fd
  0x506000000a00: fd fd fd fd fa fa fa fa fd fd fd fd fd fd fd fd
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
==3699198==ABORTING
```