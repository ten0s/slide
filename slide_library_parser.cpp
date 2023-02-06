#include <tuple>
#include <vector>
#include "slide_library_header.hpp"
#include "slide_parser.hpp"
#include "slide_directory.hpp"
#include "slide_file.hpp"

std::tuple<SlideLibraryHeader,
           std::vector<SlideDirectory*>,
           std::vector<SlideFile*>,
           size_t>
parse_slide_library(const uint8_t* buf, size_t size)
{
    auto [header, offset] = parse_slide_library_header(buf, size);
    auto totaloffset = offset;

    std::vector<SlideDirectory*> dirs;
    std::vector<SlideFile*> files;
    while (offset < size) {
        auto [dir, dirsize] = parse_slide_directory(buf+offset, size-offset);
        offset += dirsize;
        totaloffset += dirsize;
        if (dir) {
            dirs.push_back(dir);

            auto name = dir->name();
            auto addr = dir->addr();
            auto [fileheader, records, filesize] = parse_slide_file(buf+addr, size-addr);
            totaloffset += filesize;
            SlideFile* file = new SlideFile{name, fileheader, records, filesize};
            files.push_back(file);
        } else {
            break;
        }
    }

    return {header, dirs, files, totaloffset};
}
