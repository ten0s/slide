#ifndef __SLIDE_RECORD_VISITOR_HPP__
#define __SLIDE_RECORD_VISITOR_HPP__

class SlideRecordVector;
class SlideRecordOffsetVector;
class SlideRecordCommonEndpoint;
class SlideRecordSolidFillPolygon;
class SlideRecordColor;
class SlideRecordEndOfFile;

class SlideRecordVisitor {
public:
    virtual ~SlideRecordVisitor() {}
    virtual void accept(SlideRecordVector&) = 0;
    virtual void accept(SlideRecordOffsetVector&) = 0;
    virtual void accept(SlideRecordCommonEndpoint&) = 0;
    virtual void accept(SlideRecordSolidFillPolygon&) = 0;
    virtual void accept(SlideRecordColor&) = 0;
    virtual void accept(SlideRecordEndOfFile&) = 0;
};

#endif // __SLIDE_RECORD_VISITOR_HPP__
