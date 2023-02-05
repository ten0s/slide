#ifndef __SLIDE_PARSER_HPP__
#define __SLIDE_PARSER_HPP__

#include <cstddef> // size_t
#include <cstdint> // uint8_t
#include <vector>
#include "slide_endian.hpp"

class SlideFileHeader;
class SlideRecord;
class SlideLibraryHeader;
class SlideDirectory;

std::tuple<SlideFileHeader, std::vector<SlideRecord*>, size_t>
parse_slide_file(const uint8_t* buf, size_t size);

std::pair<SlideFileHeader, size_t>
parse_slide_file_header(const uint8_t* buf, size_t size);

std::pair<SlideRecord*, size_t>
parse_slide_record(const uint8_t* buf, size_t size, Endian endian);

std::tuple<SlideLibraryHeader, std::vector<SlideDirectory*>, size_t>
parse_slide_library(const uint8_t* buf, size_t size);

std::pair<SlideLibraryHeader, size_t>
parse_slide_library_header(const uint8_t* buf, size_t size);

std::pair<SlideDirectory*, size_t>
parse_slide_directory(const uint8_t* buf, size_t size);

#endif // __SLIDE_PARSER_HPP__
