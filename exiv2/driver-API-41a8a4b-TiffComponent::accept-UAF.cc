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