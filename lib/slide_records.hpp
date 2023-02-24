#ifndef __SLIDE_RECORDS_HPP__
#define __SLIDE_RECORDS_HPP__

#include <cstdint>
#include <vector>
#include "slide_records_visitor.hpp"

namespace libslide {

class SlideRecord {
public:
    virtual ~SlideRecord() {}
    virtual void visit(SlideRecordsVisitor& v) = 0;
};

class SlideRecordVector : public SlideRecord {
public:
    explicit SlideRecordVector(int16_t x0, int16_t y0,
                               int16_t x1, int16_t y1)
        : _x0{x0}, _y0{y0},
          _x1{x1}, _y1{y1}
        {}

    int16_t x0() const { return _x0; }
    int16_t y0() const { return _y0; }
    int16_t x1() const { return _x1; }
    int16_t y1() const { return _y1; }

    void visit(SlideRecordsVisitor& visitor) override {
        visitor.accept(*this);
    }

private:
    int16_t _x0;
    int16_t _y0;
    int16_t _x1;
    int16_t _y1;
};

class SlideRecordOffsetVector : public SlideRecord {
public:
    explicit SlideRecordOffsetVector(int8_t dx0, int8_t dy0,
                                     int8_t dx1, int8_t dy1)
        : _dx0{dx0}, _dy0{dy0},
          _dx1{dx1}, _dy1{dy1}
        {}

    int8_t dx0() const { return _dx0; }
    int8_t dy0() const { return _dy0; }
    int8_t dx1() const { return _dx1; }
    int8_t dy1() const { return _dy1; }

    void visit(SlideRecordsVisitor& visitor) override {
        visitor.accept(*this);
    }

private:
    int8_t _dx0;
    int8_t _dy0;
    int8_t _dx1;
    int8_t _dy1;
};

class SlideRecordCommonEndpoint : public SlideRecord {
public:
    explicit SlideRecordCommonEndpoint(int8_t dx0, int8_t dy0)
        : _dx0{dx0}, _dy0{dy0}
        {}

    int8_t dx0() const { return _dx0; }
    int8_t dy0() const { return _dy0; }

    void visit(SlideRecordsVisitor& visitor) override {
        visitor.accept(*this);
    }

private:
    int8_t _dx0;
    int8_t _dy0;
};

class SlideRecordSolidFillPolygon : public SlideRecord {
public:
    using vertex_t   = std::pair<int16_t, int16_t>;
    using vertices_t = std::vector<vertex_t>;

    explicit SlideRecordSolidFillPolygon(const vertices_t& vertices)
        : _vertices{vertices}
        {}

    const vertices_t vertices() const { return _vertices; }

    void visit(SlideRecordsVisitor& visitor) override {
        visitor.accept(*this);
    }

private:
    vertices_t _vertices;
};

class SlideRecordColor : public SlideRecord {
public:
    explicit SlideRecordColor(uint8_t color)
        : _color{color}
        {}

    uint8_t color() const { return _color; }

    void visit(SlideRecordsVisitor& visitor) override {
        visitor.accept(*this);
    }

private:
    uint8_t _color;
};

class SlideRecordEndOfFile : public SlideRecord {
public:
    explicit SlideRecordEndOfFile() {}

    void visit(SlideRecordsVisitor& visitor) override {
        visitor.accept(*this);
    }
};

} // namespace libslide

#endif // __SLIDE_RECORDS_HPP__
