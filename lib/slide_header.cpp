#include <ostream>
#include "slide_header.hpp"

namespace libslide {

std::ostream& operator<<(std::ostream& os, const SlideHeader& header)
{
    os << "ID String      : " << header.id_string() << "\n";
    //os << "Type Indicator : " << (int)header.type_indicator() << "\n";
    os << "Version        : " << (int)header.level_indicator() << "\n";
    os << "Width          : " << header.high_x_dot() << "\n";
    os << "Height         : " << header.high_y_dot() << "\n";
    os << "Aspect Ratio   : " << header.aspect_ratio() << "\n";
    //os << "Hardware Fill  : " << (int)header.hardware_fill() << "\n";
    os << "Byte Order     : " << (header.endian() == Endian::LE ?
                                  "Little-endian" :
                                  "Big-endian") << "\n";
    return os;
}

} // namespace libslide
