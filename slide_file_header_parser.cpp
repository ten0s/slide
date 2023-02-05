#include <cstring> // strncmp
#include <cstddef> // offsetof
#include <sstream>
#include "slide_file_header.hpp"
#include "slide_parser_lib.hpp"

// The floating-point aspect ratio value and all 2-byte integers are
// written in the native format of the CPU that was used to create the file
// (for 8086-family CPUs, IEEE double-precision, and low-order byte first).
struct HeaderV1 {
    // Generic Part
    char id_string[17];    // AutoCAD Slide CR LF ^Z NUL
    char type_indicator;   // 56
    char level_indicator;  // 01
    char high_x_dot[2];    // LE | BE
    char high_y_dot[2];    // LE | BE
    // Specific
    char aspect_ratio[8];  // Float LE | BE
    char hardware_fill[2]; // 0x00 | 0x02 Unused
};

struct HeaderV2 {
    // Generic Part
    char id_string[17];    // AutoCAD Slide CR LF ^Z NUL
    char type_indicator;   // 56
    char level_indicator;  // 02
    char high_x_dot[2];    // LE | BE
    char high_y_dot[2];    // LE | BE
    // Specific Part
    char aspect_ratio[4];  // LE always
    char hardware_fill[2]; // 0x0002 Unused
    char test_number[2];   // 0x1234 - LE | BE
};

std::pair<SlideFileHeader, size_t>
parse_slide_file_header(const uint8_t* buf, size_t size)
{
    Endian endian = Endian::UNK;
    short high_x_dot;
    short high_y_dot;
    double aspect_ratio;
    short hardware_fill;

    std::string id_str{"AutoCAD Slide"};
    size_t id_str_sz = id_str.size();
    if (strncmp((char*)buf, id_str.c_str(), id_str_sz) != 0 ||
        buf[id_str_sz+0] != 0x0d || buf[id_str_sz+1] != 0x0a ||
        buf[id_str_sz+2] != 0x1a || buf[id_str_sz+3] != 0x00) {
        throw std::runtime_error{"Invalid slide file header"};
    }

    char type_indicator = buf[offsetof(HeaderV1, type_indicator)];
    char level_indicator = buf[offsetof(HeaderV1, level_indicator)];

    switch (level_indicator) {
    case 1: { // Old version
        // Determine endianess by looking at the end of
        // the buffer inspecting the End of File marker.
        switch (read<uint16_t>(buf+size-2, Endian::LE)) {
        case 0xfc00:
            endian = Endian::LE;
            break;
        case 0x00fc:
            endian = Endian::BE;
            break;
        default:
            throw new std::runtime_error{"End of File is not found"};
        }
        high_x_dot = read<uint16_t>(buf+offsetof(HeaderV1, high_x_dot), endian);
        high_y_dot = read<uint16_t>(buf+offsetof(HeaderV1, high_y_dot), endian);

        aspect_ratio = read<double>(buf+offsetof(HeaderV1, aspect_ratio), endian);
        hardware_fill = read<uint16_t>(buf+offsetof(HeaderV1, hardware_fill), endian);
        break;
    }
    case 2: { // New version
        {
            // Determine endianess
            uint16_t tmp = read<uint16_t>(buf+offsetof(HeaderV2, test_number), Endian::LE);
            endian = (tmp == 0x1234 ? Endian::LE : Endian::BE);
        }

        high_x_dot = read<uint16_t>(buf+offsetof(HeaderV2, high_x_dot), endian);
        high_y_dot = read<uint16_t>(buf+offsetof(HeaderV2, high_y_dot), endian);

        {
            // Always written with the least significant byte first.
            uint32_t tmp = read<uint32_t>(buf+offsetof(HeaderV2, aspect_ratio), Endian::LE);
            aspect_ratio = tmp / 10'000'000.0;
        }

        hardware_fill = read<uint16_t>(buf+offsetof(HeaderV2, hardware_fill), endian);
        break;
    }
    default:
        std::ostringstream ss;
        ss << "Unknown slide file version: " << level_indicator;
        throw std::runtime_error{ss.str()};
    }

    SlideFileHeader header{
        id_str,
        type_indicator,
        level_indicator,
        high_x_dot,
        high_y_dot,
        aspect_ratio,
        hardware_fill,
        endian
    };

    size_t offset = level_indicator == 1 ? sizeof(HeaderV1) : sizeof(HeaderV2);

    return {header, offset};
}
