#ifndef __SLIDE_FILE_H__
#define __SLIDE_FILE_H__

#include <ostream>
#include <string>
//#include <vector>

// The floating-point aspect ratio value and all 2-byte integers are
// written in the native format of the CPU that was used to create the file
// (for 8086-family CPUs, IEEE double-precision, and low-order byte first).
struct SlideFileHeaderV1 {
    char id_string[17];    // AutoCAD Slide CR LF ^Z NUL
    char type_indicator;   // 56
    char level_indicator;  // 01
    char high_x_dot[2];    // LE | BE
    char high_y_dot[2];    // LE | BE
    char aspect_ratio[8];  // Float LE | BE
    char hardware_fill[2]; // 0x00 | 0x02 Unused
};

struct SlideFileHeaderV2 {
    char id_string[17];    // AutoCAD Slide CR LF ^Z NUL
    char type_indicator;   // 56
    char level_indicator;  // 02
    char high_x_dot[2];    // LE | BE
    char high_y_dot[2];    // LE | BE
    char aspect_ratio[4];  // LE always
    char hardware_fill[2]; // 0x0002 Unused
    char test_number[2];   // 0x1234 - LE | BE
};

enum class Endian {
    LE,
    BE
};

class SlideFileHeader {
public:

    std::string id_string() const { return _id_string; }
    int type_indicator() const { return _type_indicator; }
    int level_indicator() const { return _level_indicator; }
    short high_x_dot() const { return _high_x_dot; }
    short high_y_dot() const { return _high_y_dot; }
    float aspect_ratio() const { return _aspect_ratio; }
    short hardware_fill() const { return _hardware_fill; }
    Endian endian() const { return _endian; }

//private:
public:
    short read_short(const char buf[2], Endian endian) const;
    int read_int(const char buf[4], Endian endian) const;

//private:
public:
    std::string _id_string;
    char _type_indicator;
    char _level_indicator;
    short _high_x_dot;
    short _high_y_dot;
    float _aspect_ratio;
    short _hardware_fill;
    Endian _endian;
};

std::ostream& operator<<(std::ostream& os, const SlideFileHeader& header);

struct SlideFileData {

};

SlideFileHeader read_slide_file(const char *name);

/*
struct SlideFile {
    SlideFileHeader header;
    std::vector<SlideFileData> data;
};
*/

#endif // __SLIDE_FILE_H__
