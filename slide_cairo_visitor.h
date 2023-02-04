#ifndef __SLIDE_CAIRO_VISITOR_H__
#define __SLIDE_CAIRO_VISITOR_H__

#include <cairo.h>
#include "slide_visitor.h"
#include "colors.h"

class SlideCairoVisitor : public SlideDrawVisitor {
public:
    explicit SlideCairoVisitor(cairo_t* cr) : _cr{cr} {}

    void accept(SlideDraw& x) override {
        // TODO: what here?
    };

    void accept(SlideDrawVector& x) override {
        //_os << "VECTOR "
        //    << "(" << x.x0() << ", " << x.y0() << ")" << " "
        //    << "(" << x.x1() << ", " << x.y1() << ")" << "\n";
    }

    void accept(SlideDrawOffsetVector& x) override {
        //_os << "OFFSET VECTOR: "
        //    << "(" << int(x.dx0()) << ", " << int(x.dy0()) << ")" << " "
        //    << "(" << int(x.dx1()) << ", " << int(x.dy1()) << ")" << "\n";
    }

    void accept(SlideDrawCommonEndpoint& x) override {
        //_os << "COMMON ENDPOINT: "
        //    << "(" << int(x.dx0()) << ", " << int(x.dy0()) << ")" << "\n";
    }

    void accept(SlideDrawColor& x) override {
        RGB rgb = AutoCAD::colors[x.color()];
        GdkRGBA color = {rgb.red, rgb.green, rgb.blue, 1};
        gdk_cairo_set_source_rgba(_cr, &color);
    }

private:
    cairo_t* _cr;
};

#endif // __SLIDE_CAIRO_VISITOR_H__
