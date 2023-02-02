//#include <libgen.h>
#include <string.h> // basename

#include <iostream>
#include <fstream>

#include <exception>
#include <memory>

#include <stddef.h> // offsetof

#include "slide_file.h"

using namespace std;

void usage(const char* prog)
{
    cerr << "Usage: " << basename(prog) << " SLIDE" << endl;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        usage(argv[0]);
        return 1;
    }

    SlideFileHeader header;

    ifstream is{argv[1], std::ios::binary | std::ios::ate};
    if (is.is_open()) {
        // Read the whole file.
        auto size = is.tellg();
        std::unique_ptr<char[]> buf{new char[size]};
        is.seekg(0);
        if (is.read(buf.get(), size)) {

            // TODO: Move to SlideFileHeaderParser -> SlideFileHeader
            // TODO: Implement SlideFileDataParser -> ...

            // ID String
            std::string id_str{"AutoCAD Slide"};
            if (strncmp(buf.get(), id_str.c_str(), 13) != 0 ||
                buf[13] != 0x0d || buf[14] != 0x0a ||
                buf[15] != 0x1a || buf[16] != 0x00) {
                throw std::runtime_error{"Invalid " + id_str + " File"};
            }
            header._id_string = id_str;

            // Type indicator
            header._type_indicator = buf[17];

            // Level indicator
            header._level_indicator = buf[18];
            switch (header._level_indicator) {
            case 1: // Old version
                // No endian inside the file
                // Need to guess or better receive from command line arg
                throw std::runtime_error("Version 1 not implemented");
            case 2: // New version
                // Test number (determine endianess)
                union { char in[2]; short out; } x;
                x.in[0] = buf[offsetof(SlideFileHeaderV2, test_number) + 0];
                x.in[1] = buf[offsetof(SlideFileHeaderV2, test_number) + 1];
                header._endian = (x.out == 0x1234 ? Endian::LE : Endian::BE);
                break;
            default:
                throw std::runtime_error("Unknown version");
            }

            header._high_x_dot = header.read_short(&buf[offsetof(SlideFileHeaderV2, high_x_dot)], header._endian);
            header._high_y_dot = header.read_short(&buf[offsetof(SlideFileHeaderV2, high_y_dot)], header._endian);

            int aspect_ratio = header.read_int(&buf[offsetof(SlideFileHeaderV2, aspect_ratio)], Endian::LE);
            header._aspect_ratio = aspect_ratio / 10'000'000.0;

            header._hardware_fill = header.read_short(&buf[offsetof(SlideFileHeaderV2, hardware_fill)], header._endian);
        }

        cout << header;
    } else {
        // TODO: use `perror` or analog
        cerr << "File not found\n";
    }

    return 0;
}
