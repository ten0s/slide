#ifndef __SLIDE_HEADER_HPP__
#define __SLIDE_HEADER_HPP__

#include <string>
#include "slide_endian.hpp"

namespace libslide {

class SlideHeader {
public:
    explicit SlideHeader(const std::string& id_string,
                         uint8_t type_indicator,
                         uint8_t level_indicator,
                         uint16_t high_x_dot,
                         uint16_t high_y_dot,
                         double aspect_ration,
                         uint16_t hardware_fill,
                         Endian endian)
        : _id_string{id_string},
          _type_indicator{type_indicator},
          _level_indicator{level_indicator},
          _high_x_dot{high_x_dot},
          _high_y_dot{high_y_dot},
          _aspect_ratio{aspect_ration},
          _hardware_fill{hardware_fill},
          _endian{endian}
        {}

    std::string id_string() const { return _id_string; }
    uint8_t type_indicator() const { return _type_indicator; }
    uint8_t level_indicator() const { return _level_indicator; }
    uint16_t high_x_dot() const { return _high_x_dot; }
    uint16_t high_y_dot() const { return _high_y_dot; }
    double aspect_ratio() const { return _aspect_ratio; }
    uint8_t hardware_fill() const { return _hardware_fill; }
    Endian endian() const { return _endian; }

private:
    std::string _id_string;
    uint8_t _type_indicator;
    uint8_t _level_indicator;
    uint16_t _high_x_dot;
    uint16_t _high_y_dot;
    double _aspect_ratio;
    uint16_t _hardware_fill;
    Endian _endian;
};

std::ostream& operator<<(std::ostream& os, const SlideHeader& header);

} // namespace libslide

#endif // __SLIDE_HEADER_HPP__
