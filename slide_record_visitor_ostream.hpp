#ifndef __SLIDE_RECORD_VISITOR_OSTREAM_HPP__
#define __SLIDE_RECORD_VISITOR_OSTREAM_HPP__

#include <ostream>
#include "slide_record_visitor.hpp"

class SlideRecordVisitorOStream : public SlideRecordVisitor {
public:
    explicit SlideRecordVisitorOStream(std::ostream& os) : _os{os} {}

    void accept(SlideRecordVector& r) override;
    void accept(SlideRecordOffsetVector& r) override;
    void accept(SlideRecordCommonEndpoint& r) override;
    void accept(SlideRecordSolidFillPolygon&) override;
    void accept(SlideRecordColor& r) override;
    void accept(SlideRecordEndOfFile& r) override;

private:
    std::ostream& _os;
};

#endif // __SLIDE_RECORD_VISITOR_OSTREAM_HPP__
