#include <cstring> // strncmp
#include <cstddef> // offsetof
#include <iomanip>
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

template <typename T>
T read(const uint8_t buf[sizeof(T)], Endian endian) {
    union { uint8_t in[sizeof(T)]; T out; } x;
    switch (endian) {
    case Endian::LE:
        for (size_t i = 0; i < sizeof(T); ++i) {
            x.in[i] = buf[i];
        }
        break;
    case Endian::BE:
        for (size_t i = 0; i < sizeof(T); ++i) {
            x.in[i] = buf[sizeof(T)-1-i];
        }
        break;
    default:
        throw runtime_error("Unknown endian");
    }
    return x.out;
}

template <typename T>
uint8_t high_order_byte(T val, Endian endian) {
    union { T in; uint8_t out[sizeof(T)]; } x;
    x.in = val;
    switch (endian) {
    case Endian::LE:
        return x.out[sizeof(T)-1];
    case Endian::BE:
        return x.out[0];
    default:
        throw runtime_error("Unknown endian");
    }
}

template <typename T>
uint8_t low_order_byte(T val, Endian endian) {
    union { T in; uint8_t out[sizeof(T)]; } x;
    x.in = val;
    switch (endian) {
    case Endian::LE:
        return x.out[0];
    case Endian::BE:
        return x.out[sizeof(T)-1];
    default:
        throw runtime_error("Unknown endian");
    }
}

std::pair<SlideFileHeader, size_t>
parse_slide_file_header(const uint8_t* buf, size_t size)
{
    Endian endian = Endian::UNK;
    short high_x_dot;
    short high_y_dot;
    double aspect_ratio;
    short hardware_fill;

    std::string id_string{"AutoCAD Slide"};
    if (strncmp((char*)buf, id_string.c_str(), 13) != 0 ||
        buf[13] != 0x0d || buf[14] != 0x0a ||
        buf[15] != 0x1a || buf[16] != 0x00) {
        ostringstream ss;
        ss << "Invalid slide file header: " << id_string;
        throw std::runtime_error{ss.str()};
    }

    char type_indicator = buf[offsetof(HeaderV1, type_indicator)];
    char level_indicator = buf[offsetof(HeaderV1, level_indicator)];

    switch (level_indicator) {
    case 1: { // Old version
        // Determine endianess by looking at the end of
        // the buffer inspecting the End of File marker.
        switch (read<uint16_t>(&buf[size-2], Endian::LE)) {
        case 0xfc00:
            endian = Endian::LE;
            break;
        case 0x00fc:
            endian = Endian::BE;
            break;
        default:
            throw new runtime_error("End of File is not found");
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
            uint32_t tmp = read<uint32_t>(buf+offsetof(HeaderV2, aspect_ratio), Endian::LE);
            aspect_ratio = tmp / 10'000'000.0;
        }

        hardware_fill = read<uint16_t>(buf+offsetof(HeaderV2, hardware_fill), endian);
        break;
    }
    default:
        ostringstream ss;
        ss << "Unknown slide file version: " << level_indicator;
        throw std::runtime_error{ss.str()};
    }

    SlideFileHeader header{
        id_string,
        type_indicator,
        level_indicator,
        high_x_dot,
        high_y_dot,
        aspect_ratio,
        hardware_fill,
        endian};

    size_t offset = level_indicator == 1 ? sizeof(HeaderV1) : sizeof(HeaderV2);

    return {header, offset};
}

std::pair<SlideDraw*, size_t>
parse_slide_draw(const uint8_t* buf, size_t /*size*/, Endian endian)
{
    SlideDraw* draw = nullptr;
    size_t offset = 0;

    auto head = read<uint16_t>(buf, endian);
    auto hob = high_order_byte<uint16_t>(head, endian);
    auto lob = low_order_byte<uint16_t>(head, endian);

    if (hob <= 0x7f) {
        // Vector. Bytes: 8
        auto x0 = read<uint16_t>(buf+0*sizeof(uint16_t), endian);
        auto y0 = read<uint16_t>(buf+1*sizeof(uint16_t), endian);
        auto x1 = read<uint16_t>(buf+2*sizeof(uint16_t), endian);
        auto y1 = read<uint16_t>(buf+3*sizeof(uint16_t), endian);
        draw = new SlideDrawVector(x0, y0, x1, y1);
        offset = 8;
    } else if (hob == 0xfb) {
        // Offset vector. Bytes: 5
        auto dx0 = lob;
        auto dy0 = read<int8_t>(buf+2, endian);
        auto dx1 = read<int8_t>(buf+3, endian);
        auto dy1 = read<int8_t>(buf+4, endian);
        draw = new SlideDrawOffsetVector(dx0, dy0, dx1, dy1);
        offset = 5;
    } else if (hob == 0xfc) {
        // End of file. Bytes: 2
        draw = nullptr;
        offset = 2;
    } else if (hob == 0xfd) {
        // Solid fill. Bytes: 6
        throw runtime_error("Solid fill not implemented yet");
        //offset = 6;
    } else if (hob == 0xfe) {
        // Common endpoint vector. Bytes: 3
        auto x0 = lob;
        auto y0 = read<int8_t>(buf+2, endian);
        draw = new SlideDrawCommonEndpoint(x0, y0);
        offset = 3;
    } else if (hob == 0xff) {
         // New color. Bytes: 2
        auto color = lob;
        draw = new SlideDrawColor(color);
        offset = 2;
    } else {
        ostringstream ss;
        ss << "Unknown draw code: 0x"
           << setfill('0') << setw(2) << hex << int(hob);
        throw std::runtime_error{ss.str()};
    }

    return {draw, offset};
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
