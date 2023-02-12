#include "slide_library_directory.hpp"

namespace libslide {

std::ostream& operator<<(std::ostream& os, const SlideLibraryDirectory& dir)
{
    os << "Slide Name: " << dir.name() << "\n";
    os << "Slide Addr: " << dir.addr() << "\n";

    return os;
}

} // namespace libslide
