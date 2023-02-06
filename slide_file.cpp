#include <algorithm> // std::for_each
#include <cstring>   // basename
#include <memory>    // std::unique_ptr
#include <fstream>
#include <sstream>
#include "slide_file.hpp"
#include "slide_parser.hpp"
#include "slide_record.hpp"
#include "slide_record_visitor_ostream.hpp"
#include "slide_util.hpp"

SlideFile SlideFile::from_file(const std::string& filename)
{
    std::ifstream is{filename, std::ios::binary | std::ios::ate};
    if (is.is_open()) {
        // Determine file size.
        size_t size = is.tellg();
        std::unique_ptr<uint8_t[]> buf{new uint8_t[size]};
        is.seekg(0);
        // Read the whole file.
        if (is.read((char*)buf.get(), size)) {
            auto name = to_upper(strip_ext(basename(filename)));
            return from_buf(name, buf.get(), size);
        } else {
            std::stringstream ss;
            ss << "File read failed: " << filename << "\n";
            throw std::runtime_error{ss.str()};
        }
    } else {
        std::stringstream ss;
        ss << "File open failed: " << filename << "\n";
        throw std::runtime_error{ss.str()};
    }
}

SlideFile SlideFile::from_buf(const std::string& name,
                              const uint8_t* buf, size_t size)
{
    auto [header, records, _] = parse_slide_file(buf, size);
    return SlideFile{name, header, records, size};
}

SlideFile::SlideFile(const std::string& name,
                     const SlideFileHeader& header,
                     const std::vector<SlideRecord*>& records,
                     size_t size)
    : _name{name},
      _header{header},
      _records{records},
      _size{size}
    {}

SlideFile::SlideFile(SlideFile&& old)
    : _name{old._name},
      _header{old._header},
      _records{old._records},
      _size{old._size}
{
    old._records = {};
}

SlideFile::~SlideFile()
{
    std::for_each(
        _records.begin(), _records.end(),
        [](SlideRecord* record) { delete record; }
    );
    _records = {};
}

void SlideFile::visit_records(SlideRecordVisitor& visitor) const
{
    std::for_each(
        _records.begin(), _records.end(),
        [&visitor](SlideRecord* record) {
            record->visit(visitor);
        }
    );
}

std::ostream& operator<<(std::ostream& os, const SlideFile& file)
{
    os << "Slide Name: " << file.name() << "\n";
    os << "Slide Size: " << file.size() << "\n";

    os << "Header:\n";
    os << file.header();

    os << "Records:\n";
    SlideRecordVisitorOStream visitor{os};
    file.visit_records(visitor);

    return os;
}
