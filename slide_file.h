#ifndef __SLIDE_FILE_H__
#define __SLIDE_FILE_H__

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

enum class Endian { UNK, LE, BE };

class SlideFileHeader {
public:
    explicit SlideFileHeader(const std::string& id_string,
                             char type_indicator,
                             char level_indicator,
                             short high_x_dot,
                             short high_y_dot,
                             double aspect_ration,
                             short hardware_fill,
                             Endian endian) :
        _id_string{id_string},
        _type_indicator{type_indicator},
        _level_indicator{level_indicator},
        _high_x_dot{high_x_dot},
        _high_y_dot{high_y_dot},
        _aspect_ratio{aspect_ration},
        _hardware_fill{hardware_fill},
        _endian{endian} {}

    std::string id_string() const { return _id_string; }
    int type_indicator() const { return _type_indicator; }
    int level_indicator() const { return _level_indicator; }
    short high_x_dot() const { return _high_x_dot; }
    short high_y_dot() const { return _high_y_dot; }
    double aspect_ratio() const { return _aspect_ratio; }
    short hardware_fill() const { return _hardware_fill; }
    Endian endian() const { return _endian; }

private:
    std::string _id_string;
    char _type_indicator;
    char _level_indicator;
    short _high_x_dot;
    short _high_y_dot;
    double _aspect_ratio;
    short _hardware_fill;
    Endian _endian;
};

class SlideDraw {
public:
    virtual ~SlideDraw() {}
    virtual void draw() = 0;
};

class SlideDrawVector : public SlideDraw {
public:
    explicit SlideDrawVector(uint16_t x0, uint16_t y0,
                             uint16_t x1, uint16_t y1)
        : _x0{x0}, _y0{y0}, _x1{x1}, _y1{y1} {}

    void draw() override {
        std::cout << "VECTOR "
                  << "(" <<_x0 << ", " << _y0 << ")" << " "
                  << "(" <<_x1 << ", " << _y1 << ")" << "\n";
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

    void draw() override {
        std::cout << "OFFSET VECTOR "
                  << "(" << int(_dx0) << ", " << int(_dy0) << ")" << " "
                  << "(" << int(_dx1) << ", " << int(_dy1) << ")" << "\n";
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

    void draw() override {
        std::cout << "COMMON ENDPOINT "
                  << "(" << int(_dx0) << ", " << int(_dy0) << ")" << "\n";
    }

private:
    int8_t _dx0;
    int8_t _dy0;
};

class SlideDrawColor : public SlideDraw {
public:
    explicit SlideDrawColor(uint8_t color) :
        _color{color} {}

    void draw() override {
        std::cout << "COLOR " << int(_color) << "\n";
    }

    uint8_t color() const { return _color; }

private:
    uint8_t _color;
};

class SlideFile {
public:
    explicit SlideFile(const std::string& name,
                       const SlideFileHeader& header,
                       const std::vector<SlideDraw*>& draws)
        : _name{name}, _header{header}, _draws{draws} {}

    SlideFile(SlideFile&& old)
        : _name{old._name}, _header{old._header}, _draws{old._draws}
        {
            old._draws = {};
        }

    SlideFile(const SlideFile&) = delete;
    SlideFile& operator=(const SlideFile&) = delete;
    SlideFile& operator=(SlideFile&&) = delete;

    ~SlideFile() {
        std::for_each(_draws.begin(), _draws.end(), [](SlideDraw* draw) {
            delete draw;
        });
    }

    const std::string& name() const { return _name; }
    const SlideFileHeader& header() const { return _header; }
    const std::vector<SlideDraw*>& records() const { return _draws; }

private:
    std::string _name;
    SlideFileHeader _header;
    std::vector<SlideDraw*> _draws;
};

std::pair<SlideFile, size_t>
parse_slide_file(const std::string& name, const uint8_t* buf, size_t size);

std::pair<SlideFileHeader, size_t>
parse_slide_file_header(const uint8_t* buf, size_t size);

std::pair<SlideDraw*, size_t>
parse_slide_draw(const uint8_t* buf, size_t size, Endian endian);

std::ostream& operator<<(std::ostream& os, const SlideFile& header);
std::ostream& operator<<(std::ostream& os, const SlideFileHeader& hdr);

#endif // __SLIDE_FILE_H__
