#ifndef __SLIDE_VISITOR_CAIRO_HPP__
#define __SLIDE_VISITOR_CAIRO_HPP__

#include <cairo.h>
#include "colors.hpp"
#include "slide_visitor.hpp"

class SlideCairoVisitor : public SlideDrawVisitor {
public:
    explicit SlideCairoVisitor(cairo_t* cr)
        : _cr{cr}, _x0{0}, _y0{0}
        {}

    ~SlideCairoVisitor() override {
        cairo_stroke(_cr);
    }

    void accept(SlideDrawVector& x) override {
        cairo_move_to(_cr, x.x0(), x.y1());
        cairo_line_to(_cr, x.x1(), x.y1());
        _x0 = x.x0();
        _y0 = x.y0();
    }

    void accept(SlideDrawOffsetVector& x) override {
        //cairo_rel_line_to
        //_os << "OFFSET VECTOR: "
        //    << "(" << int(x.dx0()) << ", " << int(x.dy0()) << ")" << " "
        //    << "(" << int(x.dx1()) << ", " << int(x.dy1()) << ")" << "\n";
    }

    void accept(SlideDrawCommonEndpoint& x) override {
        //cairo_rel_move_to
        //_os << "COMMON ENDPOINT: "
        //    << "(" << int(x.dx0()) << ", " << int(x.dy0()) << ")" << "\n";
    }

    void accept(SlideDrawColor& x) override {
        auto [red, green, blue] = AutoCAD::colors[x.color()];
        cairo_set_source_rgb(_cr, red / 255.0, green / 255.0, blue / 255.0);
    }

private:
    cairo_t* _cr;
    uint16_t _x0;
    uint16_t _y0;
};

#endif // __SLIDE_VISITOR_CAIRO_HPP__
