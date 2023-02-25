#include <algorithm> // std::max
#include "slide.hpp"
#include "slide_library.hpp"
#include "slide_library_directory.hpp"
#include "slide_library_printers.hpp"

namespace libslide {

template <>
std::ostream&
operator<<(std::ostream& os, const SlideLibraryPrinter<slide_library_info_t::INFO>& p)
{
    auto& lib = p.lib();
    auto& pad = p.pad();

    auto& header = lib.header();

    os << pad << "Type   : " << header.id_string() << "\n";
    os << pad << "Name   : " << lib.name() << "\n";
    os << pad << "Size   : " << lib.size() << "\n";
    os << pad << "Slides : " << lib.dirs().size() << "\n";

    return os;
}

template <>
std::ostream&
operator<<(std::ostream& os, const SlideLibraryPrinter<slide_library_info_t::NAMES>& p)
{
    auto& lib = p.lib();
    auto& pad = p.pad();

    for (auto& dir : lib.dirs()) {
        os << pad << dir->name() << "\n";
    };

    return os;
}

template <>
std::ostream&
operator<<(std::ostream& os, const SlideLibraryPrinter<slide_library_info_t::DIRS>& p)
{
    auto& lib = p.lib();
    auto& pad = p.pad();

    size_t max = 0;
    for (auto& dir : lib.dirs()) {
        max = std::max(max, dir->name().size());
    };

    for (auto& dir : lib.dirs()) {
        size_t size = dir->name().size();
        os << pad << dir->name() << std::string(max - size + 1, ' ') << ": " << dir->addr() << "\n";
    };

    return os;
}

} // namespace libslide
