#include "slide_file.h"

using namespace std;

short SlideFileHeader::read_short(const char buf[2], Endian endian) const {
    union { char in[2]; short out; } x;
    if (endian == Endian::LE) {
        x.in[0] = buf[0];
        x.in[1] = buf[1];
    } else {
        x.in[0] = buf[1];
        x.in[1] = buf[0];
    }
    return x.out;
}

int SlideFileHeader::read_int(const char buf[4], Endian endian) const {
    union { char in[4]; int out; } x;
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

SlideFileHeader read_slide_file(const char *name) {
    SlideFileHeader header;

    //ifstream file(name);
    //file.read(reinterpret_cast<char*>(&header), sizeof(header));

    return header;
}

std::ostream& operator<<(std::ostream& os, const SlideFileHeader& hdr)
{
    using namespace std;
    os << "Slide File Header\n";
    os << "ID string      : " << hdr.id_string() << endl;
    os << "Type indicator : " << hdr.type_indicator() << endl;
    os << "Level indicator: " << hdr.level_indicator() << endl;
    os << "High X dot     : " << hdr.high_x_dot() << endl;
    os << "High Y dot     : " << hdr.high_y_dot() << endl;
    os << "Aspect ratio   : " << hdr.aspect_ratio() << endl;
    os << "Hardware fill  : " << hdr.hardware_fill() << endl;
    os << "Endianess      : " << (hdr.endian() == Endian::LE ? "LE" : "BE") << endl;
    return os;
}
