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

    SlideFile(SlideFile&&);
    ~SlideFile();

    const std::string& filename() const { return _filename; }
    const std::string& name() const { return _name; }
    const SlideFileHeader& header() const { return _header; }
    void visit_records(SlideRecordVisitor& visitor) const;

private:
    SlideFile(const std::string& filename,
              const SlideFileHeader& header,
              const std::vector<SlideRecord*>& records);
    SlideFile(const SlideFile&) = delete;
    SlideFile& operator=(const SlideFile&) = delete;
    SlideFile& operator=(SlideFile&&) = delete;

private:
    std::string _filename;
    std::string _name;
    SlideFileHeader _header;
    std::vector<SlideRecord*> _records;
};

std::ostream& operator<<(std::ostream& os, const SlideFile& file);

#endif // __SLIDE_FILE_HPP__
