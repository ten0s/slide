#include <iostream>
#include <cstring> // basename
#include "slide_file.hpp"

void usage(const std::string& prog)
{
    std::cerr << "Usage: " << prog << " SLIDE\n";
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        usage(basename(argv[0]));
        return 1;
    }

    try {
        SlideFile file = SlideFile::from_file(argv[1]);
        std::cout << file;
    } catch (...) {
        // TODO:
        throw;
        //return 1;
    }

    return 0;
}
