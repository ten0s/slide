#ifndef __SLIDE_VISITOR_H__
#define __SLIDE_VISITOR_H__

class SlideDraw;
class SlideDrawVector;
class SlideDrawOffsetVector;
class SlideDrawCommonEndpoint;
class SlideDrawColor;

class SlideDrawVisitor {
public:
    virtual void accept(SlideDraw&) = 0;
    virtual void accept(SlideDrawVector&) = 0;
    virtual void accept(SlideDrawOffsetVector&) = 0;
    virtual void accept(SlideDrawCommonEndpoint&) = 0;
    virtual void accept(SlideDrawColor&) = 0;
};

#endif // __SLIDE_VISITOR_H__
