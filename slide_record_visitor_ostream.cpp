#include "slide_colors.hpp"
#include "slide_record.hpp"
#include "slide_record_visitor_ostream.hpp"

void SlideRecordVisitorOStream::accept(SlideRecordVector& r)
{
    _os << "(VECTOR"
        << " " << (int)r.x0()
        << " " << (int)r.y0()
        << " " << (int)r.x1()
        << " " << (int)r.y1()
        << ")\n";
}

void SlideRecordVisitorOStream::accept(SlideRecordOffsetVector& r)
{
    _os << "(OFFSET_VECTOR"
        << " " << (int)r.dx0()
        << " " << (int)r.dx1()
        << " " << (int)r.dy1()
        << ")\n";
}

void SlideRecordVisitorOStream::accept(SlideRecordCommonEndpoint& r)
{
    _os << "(COMMON_ENDPOINT"
        << " " << (int)r.dx0()
        << " " << (int)r.dy0()
        << ")\n";
}

void SlideRecordVisitorOStream::accept(SlideRecordSolidFillPolygon& r)
{
    _os << "(SOLID_FILL_POLYGON";
    for (auto& [x, y] : r.vertices()) {
        _os << " " << x << " " << y;
    }
    _os << ")\n";
}

void SlideRecordVisitorOStream::accept(SlideRecordColor& r)
{
    RGB rgb = AutoCAD::colors[r.color()];
    _os << "(COLOR " << (int)r.color() << ")"
        << " # ("
        << (int)rgb.red   << " "
        << (int)rgb.green << " "
        << (int)rgb.blue
        << ")\n";
}

void SlideRecordVisitorOStream::accept(SlideRecordEndOfFile&)
{
    _os << "(END_OF_FILE)\n";
}
