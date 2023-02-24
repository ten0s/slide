#include <cassert>
#include <iomanip> // std::setfill, std::setw, std::hex
#include <sstream> // std::ostringstream
#include "slide_records.hpp"
#include "slide_record_parser.hpp"
#include "slide_parser_util.hpp"

namespace libslide {

static
std::vector<std::pair<int16_t, int16_t>>
parse_polygon(size_t n, const uint8_t* buf, Endian endian);

std::tuple<SlideRecord*, size_t, bool>
parse_slide_record(const uint8_t* buf, size_t /*size*/, Endian endian)
{
    SlideRecord* record = nullptr;
    size_t offset = 0;
    bool stop = false;

    auto head = read<int16_t>(buf, endian);
    auto hob = high_order_byte<int16_t>(head);
    auto lob = low_order_byte<int16_t>(head);

    if (hob <= 0x7f) {
        // Vector. Bytes: 8
        auto x0 = read<int16_t>(buf+0*sizeof(int16_t), endian);
        auto y0 = read<int16_t>(buf+1*sizeof(int16_t), endian);
        auto x1 = read<int16_t>(buf+2*sizeof(int16_t), endian);
        auto y1 = read<int16_t>(buf+3*sizeof(int16_t), endian);
        record = new SlideRecordVector{x0, y0, x1, y1};
        offset = 8;
    } else if (hob == 0xfb) {
        // Offset vector. Bytes: 5
        auto dx0 = static_cast<int8_t>(lob);
        auto dy0 = read<int8_t>(buf+2, endian);
        auto dx1 = read<int8_t>(buf+3, endian);
        auto dy1 = read<int8_t>(buf+4, endian);
        record = new SlideRecordOffsetVector{dx0, dy0, dx1, dy1};
        offset = 5;
    } else if (hob == 0xfc) {
        // End of file. Bytes: 2
        record = new SlideRecordEndOfFile{};
        offset = 2;
        stop = true;
    } else if (hob == 0xfd) {
        // Solid fill polygon start record.
        // Bytes: 6 * (Vertices number to follow + 2 (start & end records))
        // The low-order byte is always zero.
        assert(lob == 0);
        auto num = read<int16_t>(buf+1*sizeof(int16_t), endian);
        // y is negative for start record
        auto y = read<int16_t>(buf+2*sizeof(int16_t), endian);
        assert(y < 0);
        auto vertices = parse_polygon(num+1, buf+3*sizeof(int16_t), endian);
        record = new SlideRecordSolidFillPolygon{vertices};
        offset = 6 * (num + 2);
    } else if (hob == 0xfe) {
        // Common endpoint vector. Bytes: 3
        auto x0 = static_cast<int8_t>(lob);
        auto y0 = read<int8_t>(buf+2, endian);
        record = new SlideRecordCommonEndpoint{x0, y0};
        offset = 3;
    } else if (hob == 0xff) {
        // New color. Bytes: 2
        auto color = lob;
        record = new SlideRecordColor{color};
        offset = 2;
    } else {
        std::ostringstream ss;
        ss << "Unknown record code: 0x"
           << std::setfill('0') << std::setw(2) << std::hex << int(hob);
        throw std::runtime_error{ss.str()};
    }

    return {record, offset, stop};
}

static
std::vector<std::pair<int16_t, int16_t>>
parse_polygon(size_t n, const uint8_t* buf, Endian endian)
{
    std::vector<std::pair<int16_t, int16_t>> vertices;
    size_t offset = 0;

    for (size_t i = 0; i < n; ++i) {
        auto head = read<int16_t>(buf+offset, endian);
        auto hob = high_order_byte<int16_t>(head);
        auto lob = low_order_byte<int16_t>(head);

        if (hob == 0xfd) {
            // Solid fill polygon. Bytes: 6
            // The low-order byte is always zero.
            assert(lob == 0);
            auto x = read<int16_t>(buf+offset+1*sizeof(int16_t), endian);
            auto y = read<int16_t>(buf+offset+2*sizeof(int16_t), endian);
            if (y >= 0) {
                vertices.push_back({x, y});
            } else {
                // y is negative for end record
                assert(i == n-1);
            }
            offset += 6;
        } else {
            std::ostringstream ss;
            ss << "Unknown record code: 0x"
               << std::setfill('0') << std::setw(2) << std::hex << int(hob);
            throw std::runtime_error{ss.str()};
        }
    }

    return vertices;
}

} // namespace libslide
