//#include <libgen.h>
#include <string.h> // basename

#include <iostream>
#include <fstream>

#include <exception>
#include <memory>

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
        auto size = is.tellg();
        std::unique_ptr<char[]> buf{new char[size]};
        is.seekg(0);
        if (is.read(buf.get(), size)) {

            auto [header, offset] = parse_slide_file_header(buf.get());
            cout << header;
        } else {
            cerr << "Read failed\n";
        }

    } else {
        // TODO: use `perror` or analog
        cerr << "File not found\n";
    }

    return 0;
}
