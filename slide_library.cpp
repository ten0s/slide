#include <algorithm> // std::for_each
#include <cstring>   // basename
#include <memory>    // std::unique_ptr
#include <fstream>
#include <sstream>
#include "slide_library.hpp"
#include "slide_parser.hpp"
#include "slide_directory.hpp"

SlideLibrary SlideLibrary::from_file(const std::string& filename)
{
    std::ifstream is{filename, std::ios::binary | std::ios::ate};
    if (is.is_open()) {
        // Determine file size.
        size_t size = is.tellg();
        std::unique_ptr<uint8_t[]> buf{new uint8_t[size]};
        is.seekg(0);
        // Read the whole file.
        if (is.read((char*)buf.get(), size)) {
            return from_buf(filename, buf.get(), size);
        } else {
            std::stringstream ss;
            ss << "Library read failed: " << filename << "\n";
            throw std::runtime_error{ss.str()};
        }
    } else {
        std::stringstream ss;
        ss << "Library library failed: " << filename << "\n";
        throw std::runtime_error{ss.str()};
    }
}

SlideLibrary SlideLibrary::from_buf(const std::string& filename,
                                    const uint8_t* buf, size_t size)
{
    auto [header, dirs, _] = parse_slide_library(buf, size);

    return SlideLibrary{
        filename,
        std::move(header),
        std::move(dirs)
    };
}

SlideLibrary::SlideLibrary(const std::string& filename,
                           const SlideLibraryHeader& header,
                           const std::vector<SlideDirectory*>& dirs)
        : _filename{filename},
          _header{header},
          _dirs{dirs}
{
    // Make name without path and extension
    std::string base = basename(_filename.c_str());
    _name = base.substr(0, base.rfind("."));
}

SlideLibrary::SlideLibrary(SlideLibrary&& old)
    : _filename{old._filename},
      _name{old._name},
      _header{old._header},
      _dirs{old._dirs}
{
    old._dirs = {};
}

SlideLibrary::~SlideLibrary()
{
    std::for_each(
        _dirs.begin(), _dirs.end(),
        [](SlideDirectory* dir) { delete dir; }
    );
}

std::ostream& operator<<(std::ostream& os, const SlideLibrary& lib)
{
    os << "Slide Library: " << lib.filename() << "\n";

    os << "Header:\n";
    os << lib.header();

    os << "Directories:\n";
    std::for_each(
        lib.dirs().cbegin(), lib.dirs().cend(),
        [&os](const SlideDirectory* dir) {
            os << *dir;
        }
    );

    return os;
}
