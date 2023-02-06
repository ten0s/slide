#ifndef __SLIDE_LIBRARY_HEADER_HPP__
#define __SLIDE_LIBRARY_HEADER_HPP__

#include <string>
#include "slide_endian.hpp"

class SlideLibraryHeader {
public:
    explicit SlideLibraryHeader(const std::string& id_string)
        : _id_string{id_string}
        {}

    std::string id_string() const { return _id_string; }

private:
    std::string _id_string;
};

std::ostream& operator<<(std::ostream& os, const SlideLibraryHeader& header);

#endif // __SLIDE_LIBRARY_HEADER_HPP__
