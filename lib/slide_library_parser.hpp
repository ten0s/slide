#ifndef __SLIDE_LIBRARY_PARSER_HPP__
#define __SLIDE_LIBRARY_PARSER_HPP__

#include <cstddef> // size_t
#include <cstdint> // uint8_t
#include <vector>

namespace libslide {

class Slide;
class SlideLibraryHeader;
class SlideLibraryDirectory;

std::tuple<SlideLibraryHeader,
           std::vector<SlideLibraryDirectory*>,
           std::vector<Slide*>,
           size_t>
parse_slide_library(const uint8_t* buf, size_t size);

} // namespace libslide

#endif // __SLIDE_LIBRARY_PARSER_HPP__
