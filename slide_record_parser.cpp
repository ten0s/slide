#include <iomanip>
#include <sstream>
#include "slide_record.hpp"
#include "slide_parser_util.hpp"

std::tuple<SlideRecord*, size_t>
parse_slide_record(const uint8_t* buf, size_t /*size*/, Endian endian)
{
    SlideRecord* record = nullptr;
    size_t offset = 0;

    auto head = read<uint16_t>(buf, endian);
    auto hob = high_order_byte<uint16_t>(head, endian);
    auto lob = low_order_byte<uint16_t>(head, endian);

    if (hob <= 0x7f) {
        // Vector. Bytes: 8
        auto x0 = read<uint16_t>(buf+0*sizeof(uint16_t), endian);
        auto y0 = read<uint16_t>(buf+1*sizeof(uint16_t), endian);
        auto x1 = read<uint16_t>(buf+2*sizeof(uint16_t), endian);
        auto y1 = read<uint16_t>(buf+3*sizeof(uint16_t), endian);
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
    } else if (hob == 0xfd) {
        // Solid fill. Bytes: 6
        throw std::runtime_error{"Solid fill not implemented yet"};
        //offset = 6;
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

    return {record, offset};
}
