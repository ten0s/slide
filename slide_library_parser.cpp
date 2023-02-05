#include <tuple>
#include <vector>
#include "slide_library_header.hpp"
#include "slide_parser.hpp"

std::tuple<SlideLibraryHeader, std::vector<SlideDirectory*>, size_t>
parse_slide_library(const uint8_t* buf, size_t size)
{
    auto [header, offset] = parse_slide_library_header(buf, size);

    std::vector<SlideDirectory*> dirs;
    while (offset < size) {
        auto [dir, delta] = parse_slide_directory(buf+offset, size-offset);
        offset += delta;
        if (dir) {
            dirs.push_back(dir);
        } else {
            break;
        }
    }

    return {std::move(header), std::move(dirs), offset};
}
