#include <ostream>
#include "slide_library_header.hpp"

std::ostream& operator<<(std::ostream& os, const SlideLibraryHeader& header)
{
    os << "ID string: " << header.id_string() << "\n";
    return os;
}
