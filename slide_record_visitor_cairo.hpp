#ifndef __SLIDE_RECORD_VISITOR_CAIRO_HPP__
#define __SLIDE_RECORD_VISITOR_CAIRO_HPP__

#include <cairo.h>
#include "slide_record_visitor.hpp"

class SlideRecordVisitorCairo : public SlideRecordVisitor {
public:
    explicit SlideRecordVisitorCairo(cairo_t* cr)
        : _cr{cr}, _x0{0}, _y0{0} {}

    ~SlideRecordVisitorCairo() override;
    void accept(SlideRecordVector& x) override;
    void accept(SlideRecordOffsetVector& x) override;
    void accept(SlideRecordCommonEndpoint& x) override;
    void accept(SlideRecordColor& x) override;

private:
    cairo_t* _cr;
    uint16_t _x0;
    uint16_t _y0;
};

#endif // __SLIDE_VISITOR_CAIRO_HPP__
