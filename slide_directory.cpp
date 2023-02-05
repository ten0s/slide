#include "slide_directory.hpp"

std::ostream& operator<<(std::ostream& os, const SlideDirectory& dir)
{
    os << "Slide Name: " << dir.name() << "\n";
    os << "Slide Addr: " << dir.addr() << "\n";

    return os;
}
