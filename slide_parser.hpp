#ifndef __SLIDE_PARSER_HPP__
#define __SLIDE_PARSER_HPP__

#include <cstddef> // size_t
#include <cstdint> // uint8_t
#include <vector>
#include "slide_endian.hpp"

class Slide;
class SlideHeader;
class SlideRecord;
class SlideLibraryHeader;
class SlideLibraryDirectory;

std::tuple<SlideHeader,
           std::vector<SlideRecord*>,
           size_t>
parse_slide(const uint8_t* buf, size_t size);

std::pair<SlideHeader, size_t>
parse_slide_header(const uint8_t* buf, size_t size);

std::pair<SlideRecord*, size_t>
parse_slide_record(const uint8_t* buf, size_t size, Endian endian);

std::tuple<SlideLibraryHeader,
           std::vector<SlideLibraryDirectory*>,
           std::vector<Slide*>,
           size_t>
parse_slide_library(const uint8_t* buf, size_t size);

std::pair<SlideLibraryHeader, size_t>
parse_slide_library_header(const uint8_t* buf, size_t size);

std::pair<SlideLibraryDirectory*, size_t>
parse_slide_library_directory(const uint8_t* buf, size_t size);

#endif // __SLIDE_PARSER_HPP__
