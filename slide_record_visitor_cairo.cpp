#include "slide_record.hpp"
#include "slide_record_visitor_cairo.hpp"
#include "colors.hpp"

SlideRecordVisitorCairo::~SlideRecordVisitorCairo()
{
    cairo_stroke(_cr);
}

void SlideRecordVisitorCairo::accept(SlideRecordVector& x)
{
    cairo_move_to(_cr, x.x0(), x.y1());
    cairo_line_to(_cr, x.x1(), x.y1());
    _x0 = x.x0();
    _y0 = x.y0();
}

void SlideRecordVisitorCairo::accept(SlideRecordOffsetVector& x)
{
    //cairo_rel_line_to
    //_os << "OFFSET VECTOR: "
    //    << "(" << int(x.dx0()) << ", " << int(x.dy0()) << ")" << " "
    //    << "(" << int(x.dx1()) << ", " << int(x.dy1()) << ")" << "\n";
}

void SlideRecordVisitorCairo::accept(SlideRecordCommonEndpoint& x)
{
    //cairo_rel_move_to
    //_os << "COMMON ENDPOINT: "
    //    << "(" << int(x.dx0()) << ", " << int(x.dy0()) << ")" << "\n";
}

void SlideRecordVisitorCairo::accept(SlideRecordColor& x)
{
    auto [red, green, blue] = AutoCAD::colors[x.color()];
    cairo_set_source_rgb(_cr, red / 255.0, green / 255.0, blue / 255.0);
}
