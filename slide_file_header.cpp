#include <ostream>
#include "slide_file_header.hpp"

std::ostream& operator<<(std::ostream& os, const SlideFileHeader& header)
{
    os << "ID string      : " << header.id_string() << "\n";
    os << "Type indicator : " << header.type_indicator() << "\n";
    os << "Level indicator: " << header.level_indicator() << "\n";
    os << "High X dot     : " << header.high_x_dot() << "\n";
    os << "High Y dot     : " << header.high_y_dot() << "\n";
    os << "Aspect ratio   : " << header.aspect_ratio() << "\n";
    os << "Hardware fill  : " << header.hardware_fill() << "\n";
    os << "Byte order     : " << (header.endian() == Endian::LE ?
                                  "Little-endian" :
                                  "Big-endian") << "\n";
    return os;
}
