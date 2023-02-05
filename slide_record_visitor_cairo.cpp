#include "slide_colors.hpp"
#include "slide_record.hpp"
#include "slide_record_visitor_cairo.hpp"

const int HORIZ_PADDING = 1;
const int VERT_PADDING = 0;

double SlideRecordVisitorCairo::scalex(unsigned x) const {
    // Scale
    x = 1.0 * x * _dst_width / _src_width;
    // Add left padding
    return x + HORIZ_PADDING;
}

double SlideRecordVisitorCairo::scaley(unsigned y) const {
    // Scale
    y = 1.0 * y * _dst_height / _src_height;
    // (0, 0) is bottom-left corner
    return _dst_height - y;
}

void SlideRecordVisitorCairo::accept(SlideRecordVector& r)
{
    int x0 = r.x0(), y0 = r.y0();
    int x1 = r.x1(), y1 = r.y1();

    cairo_move_to(_cr, scalex(x0), scaley(y0));
    cairo_line_to(_cr, scalex(x1), scaley(y1));
    cairo_stroke(_cr);

    // The from point is saved as the last point.
    _x0 = x0;
    _y0 = y0;
}

void SlideRecordVisitorCairo::accept(SlideRecordOffsetVector& r)
{
    int x0 = _x0 + r.dx0(), y0 = _y0 + r.dy0();
    int x1 = _x0 + r.dx1(), y1 = _y0 + r.dy1();

    cairo_move_to(_cr, scalex(x0), scaley(y0));
    cairo_line_to(_cr, scalex(x1), scaley(y1));
    cairo_stroke(_cr);

    // The adjusted from point is saved as the last point.
    _x0 = x0;
    _y0 = y0;
}

void SlideRecordVisitorCairo::accept(SlideRecordCommonEndpoint& r)
{
    // This is a vector starting at the last point.
    int x0 = _x0,           y0 = _y0;
    int x1 = _x0 + r.dx0(), y1 = _y0 + r.dy0();

    cairo_move_to(_cr, scalex(x0), scaley(y0));
    cairo_line_to(_cr, scalex(x1), scaley(y1));
    cairo_stroke(_cr);

    // The adjusted to point is saved as the last point.
    _x0 = x1;
    _y0 = y1;
}

void SlideRecordVisitorCairo::accept(SlideRecordColor& r)
{
    RGB rgb = AutoCAD::colors[r.color()];
    cairo_set_source_rgb(_cr,
                         rgb.red   / 255,
                         rgb.green / 255,
                         rgb.blue  / 255);
}
