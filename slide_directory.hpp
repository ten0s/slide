#ifndef __SLIDE_DIRECTORY_HPP__
#define __SLIDE_DIRECTORY_HPP__

#include <ostream>
#include <string>

class SlideDirectory {
public:
    explicit SlideDirectory(const std::string& name,
                            uint32_t addr)
        : _name{name},
          _addr{addr}
        {}

    const std::string& name() const { return _name; }
    size_t addr() const { return _addr; }

private:
    std::string _name;
    uint32_t _addr;
};

std::ostream& operator<<(std::ostream& os, const SlideDirectory& dir);

#endif // __SLIDE_DIRECTORY_HPP__
