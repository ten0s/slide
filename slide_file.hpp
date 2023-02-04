#ifndef __SLIDE_FILE_HPP__
#define __SLIDE_FILE_HPP__

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "slide_visitor.hpp"
#include "slide_record.hpp"

enum class Endian { UNK, LE, BE };

class SlideFileHeader {
public:
    explicit SlideFileHeader(const std::string& id_string,
                             char type_indicator,
                             char level_indicator,
                             short high_x_dot,
                             short high_y_dot,
                             double aspect_ration,
                             short hardware_fill,
                             Endian endian) :
        _id_string{id_string},
        _type_indicator{type_indicator},
        _level_indicator{level_indicator},
        _high_x_dot{high_x_dot},
        _high_y_dot{high_y_dot},
        _aspect_ratio{aspect_ration},
        _hardware_fill{hardware_fill},
        _endian{endian} {}

    std::string id_string() const { return _id_string; }
    int type_indicator() const { return _type_indicator; }
    int level_indicator() const { return _level_indicator; }
    short high_x_dot() const { return _high_x_dot; }
    short high_y_dot() const { return _high_y_dot; }
    double aspect_ratio() const { return _aspect_ratio; }
    short hardware_fill() const { return _hardware_fill; }
    Endian endian() const { return _endian; }

private:
    std::string _id_string;
    char _type_indicator;
    char _level_indicator;
    short _high_x_dot;
    short _high_y_dot;
    double _aspect_ratio;
    short _hardware_fill;
    Endian _endian;
};

class SlideFile {
public:
    explicit SlideFile(const std::string& name,
                       const SlideFileHeader& header,
                       const std::vector<SlideRecord*>& records)
        : _name{name}, _header{header}, _records{records} {}

    SlideFile(SlideFile&& old)
        : _name{old._name}, _header{old._header}, _records{old._records}
        {
            old._records = {};
        }

    SlideFile(const SlideFile&) = delete;
    SlideFile& operator=(const SlideFile&) = delete;
    SlideFile& operator=(SlideFile&&) = delete;

    ~SlideFile() {
        std::for_each(_records.begin(), _records.end(), [](SlideRecord* record) {
            delete record;
        });
    }

    const std::string& name() const { return _name; }
    const SlideFileHeader& header() const { return _header; }
    void visit_records(SlideRecordVisitor& visitor) const {
        std::for_each(
            _records.begin(), _records.end(),
            [&visitor](SlideRecord* record) { record->visit(visitor); }
        );
    }

private:
    std::string _name;
    SlideFileHeader _header;
    std::vector<SlideRecord*> _records;
};

std::pair<SlideFile, size_t>
parse_slide_file(const std::string& name, const uint8_t* buf, size_t size);

std::pair<SlideFileHeader, size_t>
parse_slide_file_header(const uint8_t* buf, size_t size);

std::pair<SlideRecord*, size_t>
parse_slide_record(const uint8_t* buf, size_t size, Endian endian);

std::ostream& operator<<(std::ostream& os, const SlideFile& header);
std::ostream& operator<<(std::ostream& os, const SlideFileHeader& hdr);

#endif // __SLIDE_FILE_HPP__
