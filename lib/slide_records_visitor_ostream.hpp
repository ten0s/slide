#ifndef __SLIDE_RECORDS_VISITOR_OSTREAM_HPP__
#define __SLIDE_RECORDS_VISITOR_OSTREAM_HPP__

#include <ostream>
#include <string>
#include "slide_records_visitor.hpp"

namespace libslide {

class SlideRecordsVisitorOStream : public SlideRecordsVisitor {
public:
    explicit SlideRecordsVisitorOStream(std::ostream& os, const std::string& pad);
    void accept(SlideRecordVector& r) override;
    void accept(SlideRecordOffsetVector& r) override;
    void accept(SlideRecordCommonEndpoint& r) override;
    void accept(SlideRecordSolidFillPolygon&) override;
    void accept(SlideRecordColor& r) override;
    void accept(SlideRecordEndOfFile& r) override;

private:
    std::ostream& _os;
    std::string _pad;
};

} // namespace libslide

#endif // __SLIDE_RECORDS_VISITOR_OSTREAM_HPP__
