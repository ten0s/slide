#include <tuple>
#include "slide_file_header.hpp"
#include "slide_record.hpp"
#include "slide_parser.hpp"

std::tuple<SlideFileHeader, std::vector<SlideRecord*>, size_t>
parse_slide_file(const uint8_t* buf, size_t size)
{
    auto [header, offset] = parse_slide_file_header(buf, size);
    Endian endian = header.endian();

    std::vector<SlideRecord*> records;
    while (offset < size) {
        auto [record, delta] = parse_slide_record(buf+offset, size-offset, endian);
        offset += delta;
        if (record) {
            records.push_back(record);
        } else {
            break;
        }
    }

    return {header, records, offset};
}
