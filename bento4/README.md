# POC19-ReadUI32-SEGV
a NULL Pointer Dereference bug
## Test Environment
Ubuntu 14.04, 64bit, Bento4(master cbebcc9)

## How to trigger
`$ ./mp4tag --list-symbols --list-keys --show-tags $POC`

## Reference
https://github.com/axiomatic-systems/Bento4/issues/422

## Credits
Yanhao(unfuzzable123@gmail.com)  
Marsman1996(lqliuyuwei@outlook.com)

## Details
### Asan report
```
==14819== WARNING: AddressSanitizer failed to allocate 0x000ff00000c0 bytes
ASAN:SIGSEGV
=================================================================
==14819== ERROR: AddressSanitizer: SEGV on unknown address 0x000000000000 (pc 0x00000046a233 sp 0x7ffc0a9ef970 bp 0x7ffc0a9efa00 T0)
AddressSanitizer can not provide additional info.
    #0 0x46a232 in AP4_ByteStream::ReadUI32(unsigned int&) code/Source/C++/Core/Ap4ByteStream.cpp:243
    #1 0x4e75d1 in AP4_TrunAtom::AP4_TrunAtom(unsigned int, unsigned char, unsigned int, AP4_ByteStream&) code/Source/C++/Core/Ap4TrunAtom.cpp:130
    #2 0x4e713b in AP4_TrunAtom::Create(unsigned int, AP4_ByteStream&) code/Source/C++/Core/Ap4TrunAtom.cpp:51
    #3 0x4627b9 in AP4_AtomFactory::CreateAtomFromStream(AP4_ByteStream&, unsigned int, unsigned int, unsigned long long, AP4_Atom*&) code/Source/C++/Core/Ap4AtomFactory.cpp:414
    #4 0x4611c3 in AP4_AtomFactory::CreateAtomFromStream(AP4_ByteStream&, unsigned long long&, AP4_Atom*&) code/Source/C++/Core/Ap4AtomFactory.cpp:231
    #5 0x479d0a in AP4_DrefAtom::AP4_DrefAtom(unsigned int, unsigned char, unsigned int, AP4_ByteStream&, AP4_AtomFactory&) code/Source/C++/Core/Ap4DrefAtom.cpp:84
    #6 0x479956 in AP4_DrefAtom::Create(unsigned int, AP4_ByteStream&, AP4_AtomFactory&) code/Source/C++/Core/Ap4DrefAtom.cpp:50
    #7 0x463367 in AP4_AtomFactory::CreateAtomFromStream(AP4_ByteStream&, unsigned int, unsigned int, unsigned long long, AP4_Atom*&) code/Source/C++/Core/Ap4AtomFactory.cpp:545
    #8 0x4611c3 in AP4_AtomFactory::CreateAtomFromStream(AP4_ByteStream&, unsigned long long&, AP4_Atom*&) code/Source/C++/Core/Ap4AtomFactory.cpp:231
    #9 0x460b39 in AP4_AtomFactory::CreateAtomFromStream(AP4_ByteStream&, AP4_Atom*&) code/Source/C++/Core/Ap4AtomFactory.cpp:151
    #10 0x47d6a6 in AP4_File::ParseStream(AP4_ByteStream&, AP4_AtomFactory&, bool) code/Source/C++/Core/Ap4File.cpp:104
    #11 0x47d1a9 in AP4_File::AP4_File(AP4_ByteStream&, bool) code/Source/C++/Core/Ap4File.cpp:78
    #12 0x45546a in main code/Source/C++/Apps/Mp4Tag/Mp4Tag.cpp:821
    #13 0x7f144e0c7f44 (/lib/x86_64-linux-gnu/libc.so.6+0x21f44)
    #14 0x451308 in _start (bin_asan/bin/mp4tag+0x451308)
SUMMARY: AddressSanitizer: SEGV code/Source/C++/Core/Ap4ByteStream.cpp:243 AP4_ByteStream::ReadUI32(unsigned int&)
==14819== ABORTING
```
