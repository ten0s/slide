#ifndef __SLIDE_RECORD_VISITOR_OSTREAM_HPP__
#define __SLIDE_RECORD_VISITOR_OSTREAM_HPP__

#include <iostream>
#include "slide_record_visitor.hpp"

class SlideRecordVisitorOStream : public SlideRecordVisitor {
public:
    explicit SlideRecordVisitorOStream(std::ostream& os) : _os{os} {}

    void accept(SlideRecordVector& x) override;
    void accept(SlideRecordOffsetVector& x) override;
    void accept(SlideRecordCommonEndpoint& x) override;
    void accept(SlideRecordColor& x) override;

private:
    std::ostream& _os;
};

#endif // __SLIDE_RECORD_VISITOR_OSTREAM_HPP__
