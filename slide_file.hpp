#ifndef __SLIDE_FILE_HPP__
#define __SLIDE_FILE_HPP__

#include <string>
#include <vector>
#include "slide_file_header.hpp"

class SlideRecord;
class SlideRecordVisitor;

class SlideFile {
public:
    static SlideFile from_file(const std::string& filename);
    static SlideFile from_buf(const std::string& name, const uint8_t* buf, size_t size);

    SlideFile(const std::string& name,
              const SlideFileHeader& header,
              const std::vector<SlideRecord*>& records,
              size_t size);
    SlideFile(SlideFile&&);
    ~SlideFile();

    SlideFile(const SlideFile&) = delete;
    SlideFile& operator=(const SlideFile&) = delete;
    SlideFile& operator=(SlideFile&&) = delete;

    const std::string& name() const { return _name; }
    const SlideFileHeader& header() const { return _header; }
    void visit_records(SlideRecordVisitor& visitor) const;
    size_t size() const { return _size; }

private:
    std::string _name;
    SlideFileHeader _header;
    std::vector<SlideRecord*> _records;
    size_t _size;
};

std::ostream& operator<<(std::ostream& os, const SlideFile& file);

#endif // __SLIDE_FILE_HPP__
