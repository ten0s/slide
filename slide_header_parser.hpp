#ifndef __SLIDE_HEADER_PARSER_HPP__
#define __SLIDE_HEADER_PARSER_HPP__

#include <cstddef> // size_t
#include <cstdint> // uint8_t
#include <tuple>

class SlideHeader;

std::tuple<SlideHeader, size_t>
parse_slide_header(const uint8_t* buf, size_t size);

#endif // __SLIDE_HEADER_PARSER_HPP__
