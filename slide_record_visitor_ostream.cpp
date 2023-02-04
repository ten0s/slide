#include "slide_record.hpp"
#include "slide_record_visitor_ostream.hpp"

void SlideRecordVisitorOStream::accept(SlideRecordVector& x)
{
    _os << "VECTOR: "
        << "(" << x.x0() << ", " << x.y0() << ")" << " "
        << "(" << x.x1() << ", " << x.y1() << ")" << "\n";
}

void SlideRecordVisitorOStream::accept(SlideRecordOffsetVector& x)
{
    _os << "OFFSET VECTOR: "
        << "(" << x.dx0() << ", " << x.dy0() << ")" << " "
        << "(" << x.dx1() << ", " << x.dy1() << ")" << "\n";
}

void SlideRecordVisitorOStream::accept(SlideRecordCommonEndpoint& x)
{
    _os << "COMMON ENDPOINT: "
        << "(" << x.dx0() << ", " << x.dy0() << ")" << "\n";
}

void SlideRecordVisitorOStream::accept(SlideRecordColor& x)
{
    _os << "COLOR: " << x.color() << "\n";
}
