#ifndef __SLIDE_COLORS_HPP__
#define __SLIDE_COLORS_HPP__

#include <cstdint>

struct RGB {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

namespace AutoCAD {

// AutoCAD Color Index (ACI)
// Source: https://gohtx.com/acadcolors.php
extern RGB colors[];

} // namespace AutoCAD

#endif // __SLIDE_COLORS_HPP__
