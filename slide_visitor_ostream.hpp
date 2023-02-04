#ifndef __SLIDE_VISITOR_OSTREAM_HPP__
#define __SLIDE_VISITOR_OSTREAM_HPP__

#include <iostream>
#include "slide_visitor.hpp"

class SlideOStreamVisitor : public SlideDrawVisitor {
public:
    explicit SlideOStreamVisitor(std::ostream& os) : _os{os} {}

    void accept(SlideDrawVector& x) override {
        _os << "VECTOR: "
            << "(" << x.x0() << ", " << x.y0() << ")" << " "
            << "(" << x.x1() << ", " << x.y1() << ")" << "\n";
    }

    void accept(SlideDrawOffsetVector& x) override {
        _os << "OFFSET VECTOR: "
            << "(" << x.dx0() << ", " << x.dy0() << ")" << " "
            << "(" << x.dx1() << ", " << x.dy1() << ")" << "\n";
    }

    void accept(SlideDrawCommonEndpoint& x) override {
        _os << "COMMON ENDPOINT: "
            << "(" << x.dx0() << ", " << x.dy0() << ")" << "\n";
    }

    void accept(SlideDrawColor& x) override {
        _os << "COLOR: " << x.color() << "\n";
    }

private:
    std::ostream& _os;
};

#endif // __SLIDE_VISITOR_OSTREAM_HPP__
