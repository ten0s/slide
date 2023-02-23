#ifndef __SLIDE_RECORD_VISITOR_OSTREAM_HPP__
#define __SLIDE_RECORD_VISITOR_OSTREAM_HPP__

#include <ostream>
#include <string>
#include "slide_record_visitor.hpp"

namespace libslide {

class SlideRecordVisitorOStream : public SlideRecordVisitor {
public:
    explicit SlideRecordVisitorOStream(std::ostream& os, size_t pad);
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

#endif // __SLIDE_RECORD_VISITOR_OSTREAM_HPP__
