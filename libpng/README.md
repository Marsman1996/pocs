# poc-API-44f97f0-png_read_end-SEGV
SEGV when calling API png_read_end

## Reference

## Summary
There is a SEGV when calling API `png_read_end` at pngrio.c:59, `png_default_read_data`.

https://github.com/pnggroup/libpng/blob/44f97f08d729fcc77ea5d08e02cd538523dd7157/pngrio.c#L59

It is because libpng only checks `png_ptr == NULL`, but doesn't check whether the `png_ptr->io_ptr` is valid.

## Test Environment
Ubuntu 24.04.1, 64bit  
libpng (master, 44f97f0)

## How to trigger
1. Compile libpng with AddressSanitizer
2. Compile the fuzz driver, the driver code is:
```CXX
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <png.h>
#include <cstdint>

// A helper function to read a PNG file into a vector
std::vector<uint8_t> read_png_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return {};
    }

    // Get the file size
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read the file into a vector
    std::vector<uint8_t> data(size);
    if (!file.read(reinterpret_cast<char*>(data.data()), size)) {
        std::cerr << "Failed to read file: " << filename << std::endl;
        return {};
    }

    return data;
}

bool test_png_image_read_and_write(const std::string& filename) {
    std::vector<uint8_t> png_data = read_png_file(filename);
    if (png_data.empty()) {
        std::cerr << "Failed to read PNG file" << std::endl;
        return false;
    }

    size_t size = png_data.size();

    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;

    if (png_image_begin_read_from_memory(&image, png_data.data(), size) == 0) {
        std::cerr << "Failed to begin read from memory" << std::endl;
        return false;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        std::cerr << "Failed to create read struct" << std::endl;
        png_image_free(&image);
        return false;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        std::cerr << "Failed to create info struct" << std::endl;
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        png_image_free(&image);
        return false;
    }

    // png_set_scale_16(png_ptr);

    png_read_end(png_ptr, info_ptr);

    png_color background = {0, 0, 0};
    void *buffer = malloc(PNG_IMAGE_SIZE(image));
    if (!buffer) {
        std::cerr << "Failed to allocate buffer" << std::endl;
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_image_free(&image);
        return false;
    }

    if (png_image_finish_read(&image, &background, buffer, 0, nullptr) != 0) {
        std::cerr << "Failed to finish read" << std::endl;
        free(buffer);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_image_free(&image);
        return false;
    }

    png_alloc_size_t memory_bytes = 0;
    if (png_image_write_to_memory(&image, nullptr, &memory_bytes, 0, buffer, 0, nullptr) != 0) {
        std::cerr << "Failed to write to memory" << std::endl;
        free(buffer);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_image_free(&image);
        return false;
    }

    free(buffer);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    png_image_free(&image);

    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <png_file>" << std::endl;
        return 1;
    }

    const std::string filename = argv[1];
    if (test_png_image_read_and_write(filename)) {
        std::cout << "Test passed" << std::endl;
        return 0;
    } else {
        std::cout << "Test failed" << std::endl;
        return 1;
    }
}
```
The compile command is:
```bash
$ clang++ ./driver-API-44f97f0-png_read_end-SEGV.cpp -o driver-API-44f97f0-png_read_end-SEGV -fsanitize=address,undefined -g ./libpng/build/.libs/libpng16.a -I../code -I../bin_asan/include -lz
```
3. Download the [poc file](https://raw.githubusercontent.com/Marsman1996/pocs/master/libpng/poc-API-44f97f0-png_read_end-SEGV) and run the compiled driver

## ASan Report
```
$ ./driver-API-44f97f0-png_read_end-SEGV ./poc-API-44f97f0-png_read_end-SEGV 
AddressSanitizer:DEADLYSIGNAL
=================================================================
==4136910==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000000 (pc 0x7fa019a86431 bp 0x7ffd819971f0 sp 0x7ffd819971b0 T0)
==4136910==The signal is caused by a READ memory access.
==4136910==Hint: address points to the zero page.
    #0 0x7fa019a86431 in _IO_fread libio/iofread.c:37:3
    #1 0x55acfc97808d in fread (/afgenllm/database/libpng/latest/driver/driver-API-44f97f0-png_read_end-SEGV+0x7608d) (BuildId: 22628775557c2115699968fcbc5e0ef17e597b19)
    #2 0x55acfca6e1dc in png_default_read_data /afgenllm/database/libpng/latest//build/../pngrio.c:59:12
    #3 0x55acfca6e0bb in png_read_data /afgenllm/database/libpng/latest//build/../pngrio.c:36:7
    #4 0x55acfcaae3ef in png_crc_error /afgenllm/database/libpng/latest//build/../pngrutil.c:289:4
    #5 0x55acfcaa2680 in png_crc_finish_critical /afgenllm/database/libpng/latest//build/../pngrutil.c:342:8
    #6 0x55acfcaa2352 in png_crc_finish /afgenllm/database/libpng/latest//build/../pngrutil.c:362:11
    #7 0x55acfcaa945e in png_read_IDAT_data /afgenllm/database/libpng/latest//build/../pngrutil.c:4218:13
    #8 0x55acfcaaa613 in png_read_finish_IDAT /afgenllm/database/libpng/latest//build/../pngrutil.c:4346:7
    #9 0x55acfca5a49f in png_read_end /afgenllm/database/libpng/latest//build/../pngread.c:690:7
    #10 0x55acfca3776f in test_png_image_read_and_write(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const&) /afgenllm/database/libpng/latest/driver/./driver-API-44f97f0-png_read_end-SEGV.cpp:67:5
    #11 0x55acfca38650 in main /afgenllm/database/libpng/latest/driver/./driver-API-44f97f0-png_read_end-SEGV.cpp:109:9
    #12 0x7fa019a2a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #13 0x7fa019a2a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #14 0x55acfc95a5e4 in _start (/afgenllm/database/libpng/latest/driver/driver-API-44f97f0-png_read_end-SEGV+0x585e4) (BuildId: 22628775557c2115699968fcbc5e0ef17e597b19)

AddressSanitizer can not provide additional info.
SUMMARY: AddressSanitizer: SEGV libio/iofread.c:37:3 in _IO_fread
==4136910==ABORTING
```