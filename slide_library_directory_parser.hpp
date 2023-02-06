#ifndef __SLIDE_LIBRARY_DIRECTORY_PARSER_HPP__
#define __SLIDE_LIBRARY_DIRECTORY_PARSER_HPP__

#include <cstddef> // size_t
#include <cstdint> // uint8_t
#include <tuple>

class SlideLibraryDirectory;

std::tuple<SlideLibraryDirectory*, size_t>
parse_slide_library_directory(const uint8_t* buf, size_t size);

#endif // __SLIDE_LIBRARY_DIRECTORY_PARSER_HPP__
