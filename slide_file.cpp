#include <algorithm> // std::for_each
#include <cstring>   // basename
#include <memory>    // std::unique_ptr
#include <fstream>
#include <sstream>
#include "slide_file.hpp"
#include "slide_parser.hpp"
#include "slide_record.hpp"
#include "slide_record_visitor_ostream.hpp"

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
            return from_buf(filename, buf.get(), size);
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

SlideFile SlideFile::from_buf(const std::string& filename,
                              const uint8_t* buf, size_t size)
{
    auto [header, records, _] = parse_slide_file(buf, size);
    return SlideFile{filename, std::move(header), std::move(records)};
}

SlideFile::SlideFile(const std::string& filename,
                     const SlideFileHeader& header,
                     const std::vector<SlideRecord*>& records)
    : _filename{filename},
      _header{header},
      _records{records}
{
    // Make name without path and extension
    std::string base = basename(_filename.c_str());
    _name = base.substr(0, base.rfind("."));
}

SlideFile::SlideFile(SlideFile&& old)
    : _filename{old._filename},
      _name{old._name},
      _header{old._header},
      _records{old._records}
{
    old._records = {};
}

SlideFile::~SlideFile()
{
    std::for_each(_records.begin(), _records.end(),
        [](SlideRecord* record) { delete record; }
    );
}

void SlideFile::visit_records(SlideRecordVisitor& visitor) const
{
    std::for_each(_records.begin(), _records.end(),
        [&visitor](SlideRecord* record) { record->visit(visitor); }
    );
}

std::ostream& operator<<(std::ostream& os, const SlideFile& file)
{
    os << "Slide File: " << file.filename() << "\n";
    os << "Slide Name: " << file.name() << "\n";

    os << "Header:\n";
    os << file.header();

    os << "Records:\n";
    SlideRecordVisitorOStream visitor{os};
    file.visit_records(visitor);

    return os;
}
