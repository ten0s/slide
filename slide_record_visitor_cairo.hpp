#ifndef __SLIDE_RECORD_VISITOR_CAIRO_HPP__
#define __SLIDE_RECORD_VISITOR_CAIRO_HPP__

#include <cairo.h>
#include "slide_record_visitor.hpp"

class SlideRecordVisitorCairo : public SlideRecordVisitor {
public:
    explicit SlideRecordVisitorCairo(cairo_t* cr, unsigned width, unsigned height)
        : _cr{cr}, _width{width}, _height{height}, _x0{0}, _y0{0} {}

    void accept(SlideRecordVector& r) override;
    void accept(SlideRecordOffsetVector& r) override;
    void accept(SlideRecordCommonEndpoint& r) override;
    void accept(SlideRecordColor& r) override;

private:
    cairo_t* _cr;
    unsigned _width;
    unsigned _height;
    unsigned _x0;
    unsigned _y0;
};

#endif // __SLIDE_VISITOR_CAIRO_HPP__
