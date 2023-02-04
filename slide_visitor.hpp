#ifndef __SLIDE_VISITOR_HPP__
#define __SLIDE_VISITOR_HPP__

class SlideDraw;
class SlideDrawVector;
class SlideDrawOffsetVector;
class SlideDrawCommonEndpoint;
class SlideDrawColor;

class SlideDrawVisitor {
public:
    virtual ~SlideDrawVisitor() {}

    virtual void accept(SlideDrawVector&) = 0;
    virtual void accept(SlideDrawOffsetVector&) = 0;
    virtual void accept(SlideDrawCommonEndpoint&) = 0;
    virtual void accept(SlideDrawColor&) = 0;
};

#endif // __SLIDE_VISITOR_HPP__
