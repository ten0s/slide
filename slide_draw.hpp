#ifndef __SLIDE_DRAW_HPP__
#define __SLIDE_DRAW_HPP__

class SlideDraw {
public:
    virtual ~SlideDraw() {}
    virtual void visit(SlideDrawVisitor& v) = 0;
};

class SlideDrawVector : public SlideDraw {
public:
    explicit SlideDrawVector(uint16_t x0, uint16_t y0,
                             uint16_t x1, uint16_t y1)
        : _x0{x0}, _y0{y0}, _x1{x1}, _y1{y1} {}

    unsigned x0() const { return _x0; }
    unsigned y0() const { return _y0; }
    unsigned x1() const { return _x1; }
    unsigned y1() const { return _y1; }

    void visit(SlideDrawVisitor& visitor) override {
        visitor.accept(*this);
    }

private:
    uint16_t _x0;
    uint16_t _y0;
    uint16_t _x1;
    uint16_t _y1;
};

class SlideDrawOffsetVector : public SlideDraw {
public:
    explicit SlideDrawOffsetVector(int8_t dx0, int8_t dy0,
                                   int8_t dx1, int8_t dy1)
        : _dx0{dx0}, _dy0{dy0}, _dx1{dx1}, _dy1{dy1} {}

    int dx0() const { return _dx0; }
    int dy0() const { return _dy0; }
    int dx1() const { return _dx1; }
    int dy1() const { return _dy1; }

    void visit(SlideDrawVisitor& visitor) override {
        visitor.accept(*this);
    }

private:
    int8_t _dx0;
    int8_t _dy0;
    int8_t _dx1;
    int8_t _dy1;
};

class SlideDrawCommonEndpoint : public SlideDraw {
public:
    explicit SlideDrawCommonEndpoint(int8_t dx0, int8_t dy0)
        : _dx0{dx0}, _dy0{dy0} {}

    int dx0() const { return _dx0; }
    int dy0() const { return _dy0; }

    void visit(SlideDrawVisitor& visitor) override {
        visitor.accept(*this);
    }

private:
    int8_t _dx0;
    int8_t _dy0;
};

class SlideDrawColor : public SlideDraw {
public:
    explicit SlideDrawColor(uint8_t color) :
        _color{color} {}

    unsigned char color() const { return _color; }

    void visit(SlideDrawVisitor& visitor) override {
        visitor.accept(*this);
    }

private:
    uint8_t _color;
};

#endif // __SLIDE_DRAW_HPP__
