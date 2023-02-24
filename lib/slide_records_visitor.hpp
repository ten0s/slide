#ifndef __SLIDE_RECORDS_VISITOR_HPP__
#define __SLIDE_RECORDS_VISITOR_HPP__

namespace libslide {

class SlideRecordVector;
class SlideRecordOffsetVector;
class SlideRecordCommonEndpoint;
class SlideRecordSolidFillPolygon;
class SlideRecordColor;
class SlideRecordEndOfFile;

class SlideRecordsVisitor {
public:
    virtual ~SlideRecordsVisitor() {}
    virtual void accept(SlideRecordVector&) = 0;
    virtual void accept(SlideRecordOffsetVector&) = 0;
    virtual void accept(SlideRecordCommonEndpoint&) = 0;
    virtual void accept(SlideRecordSolidFillPolygon&) = 0;
    virtual void accept(SlideRecordColor&) = 0;
    virtual void accept(SlideRecordEndOfFile&) = 0;
};

} // namespace libslide

#endif // __SLIDE_RECORDS_VISITOR_HPP__
