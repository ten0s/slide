#ifndef __SLIDE_HPP__
#define __SLIDE_HPP__

#include <string>
#include <vector>
#include "slide_header.hpp"

namespace libslide {

class SlideRecord;
class SlideRecordsVisitor;

class Slide {
public:
    static Slide from_file(const std::string& filename);
    static Slide from_buf(const std::string& name, const uint8_t* buf, size_t size);

    Slide(const std::string& name,
          const SlideHeader& header,
          const std::vector<SlideRecord*>& records,
          size_t size);
    Slide(Slide&&);
    ~Slide();

    Slide(const Slide&) = delete;
    Slide& operator=(const Slide&) = delete;
    Slide& operator=(Slide&&) = delete;

    const std::string& name() const { return _name; }
    const SlideHeader& header() const { return _header; }
    void visit_records(SlideRecordsVisitor& visitor) const;
    size_t size() const { return _size; }

private:
    std::string _name;
    SlideHeader _header;
    std::vector<SlideRecord*> _records;
    size_t _size;
};

} // namespace libslide

#endif // __SLIDE_HPP__
