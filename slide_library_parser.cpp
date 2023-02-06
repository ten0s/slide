#include <tuple>
#include <vector>
#include "slide.hpp"
#include "slide_parser.hpp"
#include "slide_library_header.hpp"
#include "slide_library_header_parser.hpp"
#include "slide_library_directory.hpp"
#include "slide_library_directory_parser.hpp"

std::tuple<SlideLibraryHeader,
           std::vector<SlideLibraryDirectory*>,
           std::vector<Slide*>,
           size_t>
parse_slide_library(const uint8_t* buf, size_t size)
{
    auto [header, offset] = parse_slide_library_header(buf, size);
    auto totaloffset = offset;

    std::vector<SlideLibraryDirectory*> dirs;
    std::vector<Slide*> slides;
    while (offset < size) {
        auto [dir, dirsize] = parse_slide_library_directory(buf+offset, size-offset);
        offset += dirsize;
        totaloffset += dirsize;
        if (dir) {
            dirs.push_back(dir);

            auto name = dir->name();
            auto addr = dir->addr();
            auto [sldheader, records, sldsize] = parse_slide(buf+addr, size-addr);
            totaloffset += sldsize;
            Slide* slide = new Slide{name, sldheader, records, sldsize};
            slides.push_back(slide);
        } else {
            break;
        }
    }

    return {header, dirs, slides, totaloffset};
}
