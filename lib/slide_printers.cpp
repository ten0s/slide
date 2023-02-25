#include "slide.hpp"
#include "slide_header.hpp"
#include "slide_printers.hpp"
#include "slide_records_visitor_ostream.hpp"

namespace libslide {

template <>
std::ostream&
operator<<(std::ostream& os, const SlidePrinter<slide_info_t::INFO>& p)
{
    auto& slide = p.slide();
    auto& pad = p.pad();

    auto& header = slide.header();
    auto version = (int)header.level_indicator();

    os << pad << "Type   : " << header.id_string() << " " << version << ".0" << "\n";
    os << pad << "Name   : " << slide.name() << "\n";
    os << pad << "Size   : " << slide.size() << "\n";
    os << pad << "Width  : " << header.high_x_dot() << "\n";
    os << pad << "Height : " << header.high_y_dot() << "\n";
    os << pad << "Ratio  : " << header.aspect_ratio() << "\n";
    os << pad << "Endian : " << (header.endian() == Endian::LE ?
                                       "Little" : "Big") << "\n";
    return os;
}

template <>
std::ostream&
operator<<(std::ostream& os, const SlidePrinter<slide_info_t::RECS>& p)
{
    auto& slide = p.slide();
    auto& pad = p.pad();

    SlideRecordsVisitorOStream visitor{os, pad};
    slide.visit_records(visitor);

    return os;
}

} // namespace libslide
