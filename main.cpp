#include <iostream>
#include <string>
#include "slide_file.hpp"
#include "slide_library.hpp"
#include "slide_util.hpp"

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
        std::string filename = argv[1];
        std::string ext = get_ext(filename);

        if (ext == ".sld") {
            SlideFile file = SlideFile::from_file(filename);
            std::cout << file;
        } else if (ext == ".slb") {
            SlideLibrary library = SlideLibrary::from_file(filename);
            std::cout << library;
        } else {
            throw std::runtime_error("Unknown extention: " + ext);
        }
    } catch (...) {
        // TODO:
        throw;
        //return 1;
    }

    return 0;
}
