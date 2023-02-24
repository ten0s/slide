#ifndef __SLIDE_LIBRARY_DIRECTORY_HPP__
#define __SLIDE_LIBRARY_DIRECTORY_HPP__

#include <ostream>
#include <string>

namespace libslide {

class SlideLibraryDirectory {
public:
    explicit SlideLibraryDirectory(const std::string& name,
                                   uint32_t addr)
        : _name{name},
          _addr{addr}
        {}

    const std::string& name() const { return _name; }
    uint32_t addr() const { return _addr; }

private:
    std::string _name;
    uint32_t _addr;
};

} // namespace libslide

#endif // __SLIDE_LIBRARY_DIRECTORY_HPP__
