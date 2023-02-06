#ifndef __SLIDE_LIBRARY_HPP__
#define __SLIDE_LIBRARY_HPP__

#include <string>
#include <vector>

#include "slide_library_header.hpp"

class Slide;
class SlideLibraryDirectory;

class SlideLibrary {
public:
    static SlideLibrary from_file(const std::string& filename);
    static SlideLibrary from_buf(const std::string& name, const uint8_t* buf, size_t size);

    SlideLibrary(const std::string& name,
                 const SlideLibraryHeader& header,
                 const std::vector<SlideLibraryDirectory*>& dirs,
                 const std::vector<Slide*>& slides,
                 size_t size);
    SlideLibrary(SlideLibrary&&);
    ~SlideLibrary();

    SlideLibrary(const SlideLibrary&) = delete;
    SlideLibrary& operator=(const SlideLibrary&) = delete;
    SlideLibrary& operator=(SlideLibrary&&) = delete;

    const std::string& name() const { return _name; }
    const SlideLibraryHeader& header() const { return _header; }
    const std::vector<SlideLibraryDirectory*>& dirs() const { return _dirs; }
    const std::vector<Slide*>& slides() const { return _slides; }
    size_t size() const { return _size; }

    const Slide* find(const std::string& name) const;
    const Slide* find(size_t idx) const;

private:
    std::string _name;
    SlideLibraryHeader _header;
    std::vector<SlideLibraryDirectory*> _dirs;
    std::vector<Slide*> _slides;
    size_t _size;
};

std::ostream& operator<<(std::ostream& os, const SlideLibrary& lib);

#endif // __SLIDE_LIBRARY_HPP__
