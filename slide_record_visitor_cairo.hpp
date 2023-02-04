#ifndef __SLIDE_RECORD_VISITOR_CAIRO_HPP__
#define __SLIDE_RECORD_VISITOR_CAIRO_HPP__

#include <cairo.h>
#include "slide_record_visitor.hpp"

class SlideRecordVisitorCairo : public SlideRecordVisitor {
public:
    explicit SlideRecordVisitorCairo(cairo_t* cr)
        : _cr{cr}, _x0{0}, _y0{0} {}

    ~SlideRecordVisitorCairo() override;
    void accept(SlideRecordVector& r) override;
    void accept(SlideRecordOffsetVector& r) override;
    void accept(SlideRecordCommonEndpoint& r) override;
    void accept(SlideRecordColor& r) override;

private:
    cairo_t* _cr;
    uint16_t _x0;
    uint16_t _y0;
};

#endif // __SLIDE_VISITOR_CAIRO_HPP__
