#include <cstring> // strncmp
#include <stdexcept>
#include "slide_library_header.hpp"
#include "slide_library_header_parser.hpp"

namespace {

struct Header {
    // "AutoCAD Slide Library 1.0" CR LF ^Z NUL NUL NUL NUL  Header (32 bytes)
    uint8_t id_str[32];
};

} // namespace

namespace libslide {

std::tuple<SlideLibraryHeader, size_t>
parse_slide_library_header(const uint8_t* buf, size_t size)
{
    std::string id_str{"AutoCAD Slide Library 1.0"};
    size_t id_str_sz = id_str.size();
    if (strncmp((char*)buf, id_str.c_str(), id_str_sz) != 0 ||
        buf[id_str_sz+0] != 0x0d || buf[id_str_sz+1] != 0x0a ||
        buf[id_str_sz+2] != 0x1a || buf[id_str_sz+3] != 0x00 ||
        buf[id_str_sz+4] != 0x00 || buf[id_str_sz+5] != 0x00 ||
        buf[id_str_sz+6] != 0x00)
    {
        throw std::runtime_error{"Invalid slide library header"};
    }

    SlideLibraryHeader header{
        id_str
    };

    size_t offset = sizeof(Header);

    return {header, offset};
}

} // namespace libslide
