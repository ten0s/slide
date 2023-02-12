#include <tuple>
#include "slide_parser.hpp"
#include "slide_header.hpp"
#include "slide_header_parser.hpp"
#include "slide_record.hpp"
#include "slide_record_parser.hpp"

namespace libslide {

std::tuple<SlideHeader, std::vector<SlideRecord*>, size_t>
parse_slide(const uint8_t* buf, size_t size)
{
    auto [header, offset] = parse_slide_header(buf, size);
    Endian endian = header.endian();

    std::vector<SlideRecord*> records;
    for (;;) {
        auto [record, delta, stop] = parse_slide_record(buf+offset, size-offset, endian);
        records.push_back(record);
        offset += delta;
        if (stop) {
            break;
        }
    }

    return {header, records, offset};
}

} // namespace libslide
