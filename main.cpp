#include <string.h> // basename

#include <iostream>
#include <fstream>

#include <memory>

#include "slide_file.hpp"

using namespace std;

void usage(const std::string& prog)
{
    cerr << "Usage: " << prog << " SLIDE" << endl;
}

int main(int argc, char *argv[])
{
    const std::string prog = basename(argv[0]);
    if (argc < 2) {
        usage(prog);
        return 1;
    }

    const std::string name = argv[1];
    ifstream is{name, std::ios::binary | std::ios::ate};
    if (is.is_open()) {
        // Read the whole file.
        size_t size = is.tellg();
        std::unique_ptr<uint8_t[]> buf{new uint8_t[size]};
        is.seekg(0);
        if (is.read((char*)buf.get(), size)) {

            auto [file, offset] = parse_slide_file(name, buf.get(), size);
            cout << file;

        } else {
            cerr << "Read failed\n";
        }

    } else {
        // TODO: use `perror` or analog
        cerr << "File not found\n";
    }

    return 0;
}
