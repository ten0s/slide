#ifndef __SLIDE_RECORD_VISITOR_CAIRO_HPP__
#define __SLIDE_RECORD_VISITOR_CAIRO_HPP__

#include <cairo.h>
#include "slide_record_visitor.hpp"

class SlideRecordVisitorCairo : public SlideRecordVisitor {
public:
    explicit SlideRecordVisitorCairo(cairo_t* cr,
                                     unsigned src_width,
                                     unsigned src_height,
                                     unsigned dst_width,
                                     unsigned dst_height,
                                     double aspect)
        : _cr{cr},
          _src_width{src_width},
          _src_height{src_height},
          _dst_width{dst_width},
          _dst_height{dst_height},
          _aspect{aspect},
          _x0{0},
          _y0{0}
        {}

    void accept(SlideRecordVector& r) override;
    void accept(SlideRecordOffsetVector& r) override;
    void accept(SlideRecordCommonEndpoint& r) override;
    void accept(SlideRecordColor& r) override;

private:
    double scalex(unsigned x) const;
    double scaley(unsigned y) const;

private:
    cairo_t* _cr;
    unsigned _src_width;
    unsigned _src_height;
    unsigned _dst_width;
    unsigned _dst_height;
    double _aspect;
    unsigned _x0;
    unsigned _y0;
};

#endif // __SLIDE_VISITOR_CAIRO_HPP__
