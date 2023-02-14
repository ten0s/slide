#ifndef __SLIDE_PARSER_HPP__
#define __SLIDE_PARSER_HPP__

#include <cstddef> // size_t
#include <cstdint> // uint8_t
#include <vector>

namespace libslide {

class SlideHeader;
class SlideRecord;

std::tuple<SlideHeader,
           std::vector<SlideRecord*>,
           size_t>
parse_slide(const uint8_t* buf, size_t size);

} // namespace libslide

#endif // __SLIDE_PARSER_HPP__