#include <ostream>
#include "slide_library_header.hpp"

namespace libslide {

std::ostream& operator<<(std::ostream& os, const SlideLibraryHeader& header)
{
    os << "ID String: " << header.id_string() << "\n";
    return os;
}

} // namespace libslide
