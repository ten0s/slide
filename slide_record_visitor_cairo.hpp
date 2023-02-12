#ifndef __SLIDE_RECORD_VISITOR_CAIRO_HPP__
#define __SLIDE_RECORD_VISITOR_CAIRO_HPP__

#include <cairo.h>
#include "slide_record_visitor.hpp"

namespace libslide {

class SlideRecordVisitorCairo : public SlideRecordVisitor {
public:
    explicit SlideRecordVisitorCairo(cairo_t* cr,
                                     unsigned src_width,   // slide width
                                     unsigned src_height,  // slide height
                                     double   src_ratio,   // slide aspect ratio
                                     unsigned dst_x,       // draw offset x
                                     unsigned dst_y,       // draw offset y
                                     unsigned dst_width,   // draw width
                                     unsigned dst_height); // draw height

    void accept(SlideRecordVector& r) override;
    void accept(SlideRecordOffsetVector& r) override;
    void accept(SlideRecordCommonEndpoint& r) override;
    void accept(SlideRecordSolidFillPolygon&) override;
    void accept(SlideRecordColor& r) override;
    void accept(SlideRecordEndOfFile&) override;

private:
    double adjust_x(unsigned x) const;
    double adjust_y(unsigned y) const;

private:
    cairo_t* _cr;
    unsigned _dst_x;
    unsigned _dst_y;
    unsigned _dst_height;
    unsigned _last_x;
    unsigned _last_y;
    double _scale_x;
    double _scale_y;
};

} // namespace libslide

#endif // __SLIDE_VISITOR_CAIRO_HPP__
