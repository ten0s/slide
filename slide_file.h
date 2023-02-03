#ifndef __SLIDE_FILE_H__
#define __SLIDE_FILE_H__

#include <ostream>
#include <string>
//#include <vector>

enum class Endian { UNK, LE, BE };

class SlideFileHeader {
public:
    explicit SlideFileHeader(
        const std::string& id_string,
        char type_indicator,
        char level_indicator,
        short high_x_dot,
        short high_y_dot,
        float aspect_ration,
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
    float aspect_ratio() const { return _aspect_ratio; }
    short hardware_fill() const { return _hardware_fill; }
    Endian endian() const { return _endian; }

private:
    std::string _id_string;
    char _type_indicator;
    char _level_indicator;
    short _high_x_dot;
    short _high_y_dot;
    float _aspect_ratio;
    short _hardware_fill;
    Endian _endian;
};

std::pair<SlideFileHeader, size_t>
parse_slide_file_header(const char *buf);

std::ostream& operator<<(std::ostream& os, const SlideFileHeader& header);

struct SlideFileData {

};

/*
struct SlideFile {
    SlideFileHeader header;
    std::vector<SlideFileData> data;
};
*/

#endif // __SLIDE_FILE_H__
