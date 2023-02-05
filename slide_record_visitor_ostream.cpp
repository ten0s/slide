#include "slide_colors.hpp"
#include "slide_record.hpp"
#include "slide_record_visitor_ostream.hpp"

void SlideRecordVisitorOStream::accept(SlideRecordVector& r)
{
    _os << "VECTOR: "
        << "(" << r.x0() << ", " << r.y0() << ")" << " "
        << "(" << r.x1() << ", " << r.y1() << ")" << "\n";
}

void SlideRecordVisitorOStream::accept(SlideRecordOffsetVector& r)
{
    _os << "OFFSET VECTOR: "
        << "(" << r.dx0() << ", " << r.dy0() << ")" << " "
        << "(" << r.dx1() << ", " << r.dy1() << ")" << "\n";
}

void SlideRecordVisitorOStream::accept(SlideRecordCommonEndpoint& r)
{
    _os << "COMMON ENDPOINT: "
        << "(" << r.dx0() << ", " << r.dy0() << ")" << "\n";
}

void SlideRecordVisitorOStream::accept(SlideRecordColor& r)
{
    RGB rgb = AutoCAD::colors[r.color()];
    _os << "COLOR: " << r.color() << " "
        << "("
        << int(rgb.red) << ", " << int(rgb.green) << ", " << int(rgb.blue)
        << ")" << "\n";
}
