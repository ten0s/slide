#include "slide_record.hpp"
#include "slide_record_visitor_cairo.hpp"
#include "colors.hpp"

SlideRecordVisitorCairo::~SlideRecordVisitorCairo()
{
    cairo_stroke(_cr);
}

void SlideRecordVisitorCairo::accept(SlideRecordVector& r)
{
    int x0 = r.x0(), y0 = r.y0();
    int x1 = r.x1(), y1 = r.y1();

    cairo_move_to(_cr, x0, y0);
    cairo_line_to(_cr, x1, y1);

    // The from point is saved as the last point.
    _x0 = x0;
    _y0 = y0;
}

void SlideRecordVisitorCairo::accept(SlideRecordOffsetVector& r)
{
    int x0 = _x0 + r.dx0(), y0 = _y0 + r.dy0();
    int x1 = _x0 + r.dx1(), y1 = _y0 + r.dy1();

    cairo_move_to(_cr, x0, y0);
    cairo_line_to(_cr, x1, y1);

    // The adjusted from point is saved as the last point.
    _x0 = x0;
    _y0 = y0;
}

void SlideRecordVisitorCairo::accept(SlideRecordCommonEndpoint& r)
{
    // This is a vector starting at the last point.
    int x0 = _x0,           y0 = _y0;
    int x1 = _x0 + r.dx0(), y1 = _y0 + r.dy0();

    cairo_move_to(_cr, x0, y0);
    cairo_line_to(_cr, x1, y1);

    // The adjusted to point is saved as the last point.
    _x0 = x1;
    _y0 = y1;
}

void SlideRecordVisitorCairo::accept(SlideRecordColor& r)
{
    auto [red, green, blue] = AutoCAD::colors[r.color()];
    cairo_set_source_rgb(_cr, red / 255.0, green / 255.0, blue / 255.0);
}
