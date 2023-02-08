#include "slide_colors.hpp"
#include "slide_record.hpp"
#include "slide_record_visitor_cairo.hpp"

static const unsigned PADDING_X = 1;
static const unsigned PADDING_Y = 1;

SlideRecordVisitorCairo::SlideRecordVisitorCairo(cairo_t* cr,
                                                 unsigned src_width,
                                                 unsigned src_height,
                                                 unsigned dst_x,
                                                 unsigned dst_y,
                                                 unsigned dst_width,
                                                 unsigned dst_height)
    : _cr{cr},
      _src_width{src_width},
      _src_height{src_height},
      _dst_x{dst_x},
      _dst_y{dst_y},
      _dst_width{dst_width},
      _dst_height{dst_height},
      _last_x{0},
      _last_y{0}
{
    _scale_x = 1.0 * _dst_width  / _src_width;
    _scale_y = 1.0 * _dst_height / _src_height;
}

inline
double SlideRecordVisitorCairo::adjust_x(unsigned x) const {
    // Scale & Move
    return x * _scale_x + _dst_x + PADDING_X;
}

inline
double SlideRecordVisitorCairo::adjust_y(unsigned y) const {
    // Point (0,0) is located at the lower-left corner.
    // Scale & Move
    return _dst_height - y * _scale_y + _dst_y - PADDING_Y;
}

void SlideRecordVisitorCairo::accept(SlideRecordVector& r)
{
    int x0 = r.x0(), y0 = r.y0();
    int x1 = r.x1(), y1 = r.y1();

    cairo_move_to(_cr, adjust_x(x0), adjust_y(y0));
    cairo_line_to(_cr, adjust_x(x1), adjust_y(y1));
    cairo_stroke(_cr);

    // The from point is saved as the last point.
    _last_x = x0;
    _last_y = y0;
}

void SlideRecordVisitorCairo::accept(SlideRecordOffsetVector& r)
{
    int x0 = _last_x + r.dx0(), y0 = _last_y + r.dy0();
    int x1 = _last_x + r.dx1(), y1 = _last_y + r.dy1();

    cairo_move_to(_cr, adjust_x(x0), adjust_y(y0));
    cairo_line_to(_cr, adjust_x(x1), adjust_y(y1));
    cairo_stroke(_cr);

    // The adjusted from point is saved as the last point.
    _last_x = x0;
    _last_y = y0;
}

void SlideRecordVisitorCairo::accept(SlideRecordCommonEndpoint& r)
{
    // This is a vector starting at the last point.
    int x0 = _last_x,           y0 = _last_y;
    int x1 = _last_x + r.dx0(), y1 = _last_y + r.dy0();

    cairo_move_to(_cr, adjust_x(x0), adjust_y(y0));
    cairo_line_to(_cr, adjust_x(x1), adjust_y(y1));
    cairo_stroke(_cr);

    // The adjusted to point is saved as the last point.
    _last_x = x1;
    _last_y = y1;
}

void SlideRecordVisitorCairo::accept(SlideRecordSolidFillPolygon& r)
{
    auto& vs = r.vertices();
    if (vs.size() > 0) {
        auto it = vs.cbegin();
        auto end = vs.cend();

        auto [x0, y0] = *it;
        cairo_move_to(_cr, adjust_x(x0), adjust_y(y0));
        ++it;

        for (; it != end; ++it) {
            auto [x, y] = *it;
            cairo_line_to(_cr, adjust_x(x), adjust_y(y));
        }

        cairo_close_path(_cr);
        cairo_fill(_cr);
    }
}

void SlideRecordVisitorCairo::accept(SlideRecordColor& r)
{
    RGB rgb = AutoCAD::colors[r.color()];
    cairo_set_source_rgb(_cr,
                         rgb.red   / 255.0,
                         rgb.green / 255.0,
                         rgb.blue  / 255.0);
}

void SlideRecordVisitorCairo::accept(SlideRecordEndOfFile&)
{

}
