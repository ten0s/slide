#include <cstring> // strncmp
#include <cstddef> // offsetof
#include <sstream>
#include "slide_file.h"

using namespace std;

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

short read_short(const uint8_t buf[2], Endian endian);
int read_int(const uint8_t buf[4], Endian endian);

std::pair<SlideFileHeader, size_t>
parse_slide_file_header(const uint8_t* buf)
{
    Endian endian = Endian::UNK;
    short high_x_dot;
    short high_y_dot;
    float aspect_ratio;
    short hardware_fill;

    // ID String
    std::string id_string{"AutoCAD Slide"};
    if (strncmp((char*)buf, id_string.c_str(), 13) != 0 ||
        buf[13] != 0x0d || buf[14] != 0x0a ||
        buf[15] != 0x1a || buf[16] != 0x00) {
        stringstream ss;
        ss << "Invalid " << id_string << " File";
        throw std::runtime_error{ss.str()};
    }

    // Type indicator
    char type_indicator = buf[offsetof(HeaderV1, type_indicator)];

    // Level indicator
    char level_indicator = buf[offsetof(HeaderV1, level_indicator)];

    switch (level_indicator) {
    case 1: { // Old version
        // No endian inside the file
        // Get hint from command line arg
        throw std::runtime_error("Version 1 not implemented");
    }
    case 2: { // New version
        // Test number (determine endianess)
        union { char in[2]; short out; } x;
        x.in[0] = buf[offsetof(HeaderV2, test_number) + 0];
        x.in[1] = buf[offsetof(HeaderV2, test_number) + 1];
        endian = (x.out == 0x1234 ? Endian::LE : Endian::BE);

        high_x_dot = read_short(&buf[offsetof(HeaderV2, high_x_dot)], endian);
        high_y_dot = read_short(&buf[offsetof(HeaderV2, high_y_dot)], endian);

        int tmp = read_int(&buf[offsetof(HeaderV2, aspect_ratio)], Endian::LE);
        aspect_ratio = tmp / 10'000'000.0;

        hardware_fill = read_short(&buf[offsetof(HeaderV2, hardware_fill)], endian);
        break;
    }
    default:
        throw std::runtime_error{"Unknown version"};
    }

    SlideFileHeader header(
        id_string,
        type_indicator,
        level_indicator,
        high_x_dot,
        high_y_dot,
        aspect_ratio,
        hardware_fill,
        endian);

    size_t offset = level_indicator == 1 ? sizeof(HeaderV1) : sizeof(HeaderV2);

    return {header, offset};
}

std::pair<SlideDraw*, size_t>
parse_slide_draw(const uint8_t* buf)
{
    SlideDraw* draw = nullptr;
    size_t offset = 0;

    switch (buf[1]) {
    case 0xFF: // New color. Bytes: 2
        draw = new SlideDrawColor(buf[0]);
        offset = 2;
        break;
    case 0xFC: // End of file. Bytes: 2
        draw = nullptr;
        offset = 2;
        break;
    default:
        stringstream ss;
        ss << "Unknown draw code: " << hex << buf[1];
        throw std::runtime_error{ss.str()};
    }

    return {draw, offset};
}

short read_short(const uint8_t buf[2], Endian endian) {
    union { uint8_t in[2]; short out; } x;
    if (endian == Endian::LE) {
        x.in[0] = buf[0];
        x.in[1] = buf[1];
    } else {
        x.in[0] = buf[1];
        x.in[1] = buf[0];
    }
    return x.out;
}

int read_int(const uint8_t buf[4], Endian endian) {
    union { uint8_t in[4]; int out; } x;
    if (endian == Endian::LE) {
        x.in[0] = buf[0];
        x.in[1] = buf[1];
        x.in[2] = buf[2];
        x.in[3] = buf[3];
    } else {
        x.in[0] = buf[3];
        x.in[1] = buf[2];
        x.in[2] = buf[1];
        x.in[3] = buf[0];
    }
    return x.out;
}

std::ostream& operator<<(std::ostream& os, const SlideFileHeader& hdr)
{
    using namespace std;
    os << "Slide File Header\n";
    os << "ID string      : " << hdr.id_string() << "\n";
    os << "Type indicator : " << hdr.type_indicator() << "\n";
    os << "Level indicator: " << hdr.level_indicator() << "\n";
    os << "High X dot     : " << hdr.high_x_dot() << "\n";
    os << "High Y dot     : " << hdr.high_y_dot() << "\n";
    os << "Aspect ratio   : " << hdr.aspect_ratio() << "\n";
    os << "Hardware fill  : " << hdr.hardware_fill() << "\n";
    os << "Endianess      : " << (hdr.endian() == Endian::LE ? "LE" : "BE") << "\n";
    return os;
}

std::ostream& operator<<(std::ostream& os, const SlideDrawColor& draw)
{
    os << "COLOR " << draw.color() << "\n";
    return os;
}
