#include <string.h> // basename

#include <algorithm>
#include <iostream>
#include <fstream>

#include <exception>
#include <memory>

#include <vector>

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

    ifstream is{argv[1], std::ios::binary | std::ios::ate};
    if (is.is_open()) {
        // Read the whole file.
        size_t size = is.tellg();
        std::unique_ptr<uint8_t[]> buf{new uint8_t[size]};
        is.seekg(0);
        if (is.read((char*)buf.get(), size)) {

            auto [header, offset] = parse_slide_file_header(buf.get(), size);
            Endian endian = header.endian();

            std::vector<std::unique_ptr<SlideDraw>> draws;

            while (offset < size) {
                auto [draw, delta] = parse_slide_draw(&buf[offset], size-offset, endian);
                if (draw) {
                    draws.push_back(std::unique_ptr<SlideDraw>(draw));
                }
                offset += delta;
                //break;
            }

            cout << header;
            cout << "Operations\n";
            std::for_each(draws.cbegin(), draws.cend(), [](auto &d) {
                d->draw();
            });

        } else {
            cerr << "Read failed\n";
        }

    } else {
        // TODO: use `perror` or analog
        cerr << "File not found\n";
    }

    return 0;
}
