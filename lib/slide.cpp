#include <cstring>   // basename
#include <memory>    // std::unique_ptr
#include <fstream>
#include <sstream>
#include "slide.hpp"
#include "slide_parser.hpp"
#include "slide_record.hpp"
#include "slide_record_visitor_ostream.hpp"
#include "slide_util.hpp"

namespace libslide {

Slide Slide::from_file(const std::string& filename)
{
    std::ifstream is{filename, std::ios::binary | std::ios::ate};
    if (is.is_open()) {
        // Determine file size.
        size_t size = is.tellg();
        std::unique_ptr<uint8_t[]> buf{new uint8_t[size]};
        is.seekg(0);
        // Read the whole file.
        if (is.read((char*)buf.get(), size)) {
            auto name = strip_ext(basename(filename));
            return from_buf(name, buf.get(), size);
        } else {
            std::stringstream ss;
            ss << "Slide file read failed: " << filename << "\n";
            throw std::runtime_error{ss.str()};
        }
    } else {
        std::stringstream ss;
        ss << "Slide file open failed: " << filename << "\n";
        throw std::runtime_error{ss.str()};
    }
}

Slide Slide::from_buf(const std::string& name,
                      const uint8_t* buf, size_t size)
{
    auto [header, records, offset] = parse_slide(buf, size);
    return Slide{name, header, records, offset};
}

Slide::Slide(const std::string& name,
             const SlideHeader& header,
             const std::vector<SlideRecord*>& records,
             size_t size)
    : _name{name},
      _header{header},
      _records{records},
      _size{size}
    {}

Slide::Slide(Slide&& old)
    : _name{old._name},
      _header{old._header},
      _records{old._records},
      _size{old._size}
{
    old._records = {};
}

Slide::~Slide()
{
    for (auto& record : _records) {
        delete record;
    }
    _records = {};
}

void Slide::visit_records(SlideRecordVisitor& visitor) const
{
    for (auto& record : _records) {
        record->visit(visitor);
    }
}

std::ostream& operator<<(std::ostream& os, const Slide& slide)
{
    auto& header = slide.header();
    auto version = (int)header.level_indicator();

    os << "Info:\n";
    os << "    Type         : " << header.id_string() << " " << version << ".0" << "\n";
    os << "    Name         : " << slide.name() << "\n";
    os << "    Size         : " << slide.size() << "\n";
    os << "    Width        : " << header.high_x_dot() << "\n";
    os << "    Height       : " << header.high_y_dot() << "\n";
    os << "    Aspect Ratio : " << header.aspect_ratio() << "\n";
    os << "    Byte Order   : " << (header.endian() == Endian::LE ?
                                  "Little-endian" :
                                  "Big-endian") << "\n";

    os << "Records:\n";
    SlideRecordVisitorOStream visitor{os, 4};
    slide.visit_records(visitor);

    return os;
}

} // namespace libslide
