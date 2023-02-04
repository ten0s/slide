#ifndef __SLIDE_VISITOR_HPP__
#define __SLIDE_VISITOR_HPP__

class SlideRecord;
class SlideRecordVector;
class SlideRecordOffsetVector;
class SlideRecordCommonEndpoint;
class SlideRecordColor;

class SlideRecordVisitor {
public:
    virtual ~SlideRecordVisitor() {}

    virtual void accept(SlideRecordVector&) = 0;
    virtual void accept(SlideRecordOffsetVector&) = 0;
    virtual void accept(SlideRecordCommonEndpoint&) = 0;
    virtual void accept(SlideRecordColor&) = 0;
};

#endif // __SLIDE_VISITOR_HPP__
