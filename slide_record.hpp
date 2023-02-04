#ifndef __SLIDE_RECORD_HPP__
#define __SLIDE_RECORD_HPP__

class SlideRecord {
public:
    virtual ~SlideRecord() {}
    virtual void visit(SlideRecordVisitor& v) = 0;
};

class SlideRecordVector : public SlideRecord {
public:
    explicit SlideRecordVector(uint16_t x0, uint16_t y0,
                               uint16_t x1, uint16_t y1)
        : _x0{x0}, _y0{y0}, _x1{x1}, _y1{y1} {}

    unsigned x0() const { return _x0; }
    unsigned y0() const { return _y0; }
    unsigned x1() const { return _x1; }
    unsigned y1() const { return _y1; }

    void visit(SlideRecordVisitor& visitor) override {
        visitor.accept(*this);
    }

private:
    uint16_t _x0;
    uint16_t _y0;
    uint16_t _x1;
    uint16_t _y1;
};

class SlideRecordOffsetVector : public SlideRecord {
public:
    explicit SlideRecordOffsetVector(int8_t dx0, int8_t dy0,
                                     int8_t dx1, int8_t dy1)
        : _dx0{dx0}, _dy0{dy0}, _dx1{dx1}, _dy1{dy1} {}

    int dx0() const { return _dx0; }
    int dy0() const { return _dy0; }
    int dx1() const { return _dx1; }
    int dy1() const { return _dy1; }

    void visit(SlideRecordVisitor& visitor) override {
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
        : _dx0{dx0}, _dy0{dy0} {}

    int dx0() const { return _dx0; }
    int dy0() const { return _dy0; }

    void visit(SlideRecordVisitor& visitor) override {
        visitor.accept(*this);
    }

private:
    int8_t _dx0;
    int8_t _dy0;
};

class SlideRecordColor : public SlideRecord {
public:
    explicit SlideRecordColor(uint8_t color) :
        _color{color} {}

    unsigned color() const { return _color; }

    void visit(SlideRecordVisitor& visitor) override {
        visitor.accept(*this);
    }

private:
    uint8_t _color;
};

#endif // __SLIDE_RECORD_HPP__
