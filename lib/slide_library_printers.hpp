#ifndef __SLIDE_LIBRARY_PRINTERS_HPP__
#define __SLIDE_LIBRARY_PRINTERS_HPP__

#include <ostream>
#include <string>

namespace libslide {

enum class slide_library_info_t {
    NONE = 0,
    INFO,
    NAMES,
    DIRS,
    ALL
};

class SlideLibrary;

template <slide_library_info_t>
class SlideLibraryPrinter {
public:
    explicit SlideLibraryPrinter(const SlideLibrary& lib, const std::string& pad = "")
        : _lib{lib}, _pad{pad} {}

    const SlideLibrary& lib() const { return _lib; }
    const std::string& pad() const { return _pad; }

private:
    const SlideLibrary& _lib;
    std::string _pad;
};

template <slide_library_info_t What>
std::ostream& operator<<(std::ostream& os, const SlideLibraryPrinter<What>& p);

} // namespace libslide

#endif // __SLIDE_LIBRARY_PRINTERS_HPP__
