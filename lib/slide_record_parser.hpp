#ifndef __SLIDE_RECORD_PARSER_HPP__
#define __SLIDE_RECORD_PARSER_HPP__

#include <cstddef> // size_t
#include <cstdint> // uint8_t
#include <tuple>
#include "slide_endian.hpp"

namespace libslide {

class SlideRecord;

std::tuple<SlideRecord*, size_t, bool>
parse_slide_record(const uint8_t* buf, size_t size, Endian endian);

} // namespace libslide

#endif // __SLIDE_RECORD_PARSER_HPP__
