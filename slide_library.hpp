#ifndef __SLIDE_LIBRARY_HPP__
#define __SLIDE_LIBRARY_HPP__

#include <string>
#include <vector>

#include "slide_library_header.hpp"

class SlideDirectory;
class SlideFile;

class SlideLibrary {
public:
    static SlideLibrary from_file(const std::string& filename);
    static SlideLibrary from_buf(const std::string& name, const uint8_t* buf, size_t size);

    SlideLibrary(const std::string& name,
                 const SlideLibraryHeader& header,
                 const std::vector<SlideDirectory*>& dirs,
                 const std::vector<SlideFile*>& files,
                 size_t size);
    SlideLibrary(SlideLibrary&&);
    ~SlideLibrary();

    SlideLibrary(const SlideLibrary&) = delete;
    SlideLibrary& operator=(const SlideLibrary&) = delete;
    SlideLibrary& operator=(SlideLibrary&&) = delete;

    const std::string& name() const { return _name; }
    const SlideLibraryHeader& header() const { return _header; }
    const std::vector<SlideDirectory*>& dirs() const { return _dirs; }
    const std::vector<SlideFile*>& files() const { return _files; }
    size_t size() const { return _size; }

private:
    std::string _name;
    SlideLibraryHeader _header;
    std::vector<SlideDirectory*> _dirs;
    std::vector<SlideFile*> _files;
    size_t _size;
};

std::ostream& operator<<(std::ostream& os, const SlideLibrary& file);

#endif // __SLIDE_LIBRARY_HPP__
