#ifndef __SLIDE_OSTREAM_VISITOR_H__
#define __SLIDE_OSTREAM_VISITOR_H__

#include <iostream>
#include "slide_visitor.h"

class SlideOStreamVisitor : public SlideDrawVisitor {
public:
    explicit SlideOStreamVisitor(std::ostream& os) : _os{os} {}

    void accept(SlideDraw& x) override {
        _os << "SlideDrawVisitor SlideDraw TODO\n";
    };

    void accept(SlideDrawVector& x) override {
        _os << "VECTOR "
            << "(" << x.x0() << ", " << x.y0() << ")" << " "
            << "(" << x.x1() << ", " << x.y1() << ")" << "\n";
    }

    void accept(SlideDrawOffsetVector& x) override {
        _os << "OFFSET VECTOR: "
            << "(" << int(x.dx0()) << ", " << int(x.dy0()) << ")" << " "
            << "(" << int(x.dx1()) << ", " << int(x.dy1()) << ")" << "\n";
    }

    void accept(SlideDrawCommonEndpoint& x) override {
        _os << "COMMON ENDPOINT: "
            << "(" << int(x.dx0()) << ", " << int(x.dy0()) << ")" << "\n";
    }

    void accept(SlideDrawColor& x) override {
        _os << "COLOR: " << int(x.color()) << "\n";
    }

private:
    std::ostream& _os;
};

#endif // __SLIDE_OSTREAM_VISITOR_H__
