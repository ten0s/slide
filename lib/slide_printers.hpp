#ifndef __SLIDE_PRINTERS_HPP__
#define __SLIDE_PRINTERS_HPP__

#include <ostream>
#include <string>

namespace libslide {

enum class slide_info_t {
    NONE = 0,
    INFO,
    RECS,
    ALL
};

class Slide;

template <slide_info_t>
class SlidePrinter {
public:
    explicit SlidePrinter(const Slide& slide, const std::string& pad = "")
        : _slide{slide}, _pad{pad} {}

    const Slide& slide() const { return _slide; }
    const std::string&  pad() const { return _pad; }

private:
    const Slide& _slide;
    std::string _pad;
};

template <slide_info_t What>
std::ostream& operator<<(std::ostream& os, const SlidePrinter<What>& p);

} // namespace libslide

#endif // __SLIDE_PRINTERS_HPP__
