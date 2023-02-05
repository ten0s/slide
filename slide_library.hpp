#ifndef __SLIDE_LIBRARY_HPP__
#define __SLIDE_LIBRARY_HPP__

#include <string>
#include <vector>

#include "slide_library_header.hpp"

class SlideDirectory;

class SlideLibrary {
public:
    static SlideLibrary from_file(const std::string& filename);
    static SlideLibrary from_buf(const std::string& name, const uint8_t* buf, size_t size);

    SlideLibrary(SlideLibrary&&);
    ~SlideLibrary();

    const std::string& filename() const { return _filename; }
    const std::string& name() const { return _name; }
    const SlideLibraryHeader& header() const { return _header; }
    const std::vector<SlideDirectory*> dirs() const { return _dirs; }

private:
    SlideLibrary(const std::string& filename,
                 const SlideLibraryHeader& header,
                 const std::vector<SlideDirectory*>& dirs);
    SlideLibrary(const SlideLibrary&) = delete;
    SlideLibrary& operator=(const SlideLibrary&) = delete;
    SlideLibrary& operator=(SlideLibrary&&) = delete;

private:
    std::string _filename;
    std::string _name;
    SlideLibraryHeader _header;
    std::vector<SlideDirectory*> _dirs;
};

std::ostream& operator<<(std::ostream& os, const SlideLibrary& file);

#endif // __SLIDE_LIBRARY_HPP__
