#include <algorithm> // std::for_each
#include <memory>    // std::unique_ptr
#include <fstream>
#include <sstream>
#include "slide.hpp"
#include "slide_parser.hpp"
#include "slide_library.hpp"
#include "slide_library_parser.hpp"
#include "slide_library_directory.hpp"
#include "slide_util.hpp"

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
            auto name = to_upper(strip_ext(basename(filename)));
            return from_buf(name, buf.get(), size);
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

SlideLibrary SlideLibrary::from_buf(const std::string& name,
                                    const uint8_t* buf, size_t size)
{
    auto [header, dirs, files, offset] = parse_slide_library(buf, size);

    return SlideLibrary{
        name,
        header,
        dirs,
        files,
        offset,
    };
}

SlideLibrary::SlideLibrary(const std::string& name,
                           const SlideLibraryHeader& header,
                           const std::vector<SlideLibraryDirectory*>& dirs,
                           const std::vector<Slide*>& slides,
                           size_t size)
        : _name{name},
          _header{header},
          _dirs{dirs},
          _slides{slides},
          _size{size}
       {}

SlideLibrary::SlideLibrary(SlideLibrary&& old)
    : _name{old._name},
      _header{old._header},
      _dirs{old._dirs},
      _slides{old._slides},
      _size{old._size}
{
    old._dirs = {};
    old._slides = {};
}

SlideLibrary::~SlideLibrary()
{
    std::for_each(
        _dirs.begin(), _dirs.end(),
        [](auto* dir) { delete dir; }
    );
    _dirs = {};

    std::for_each(
        _slides.begin(), _slides.end(),
        [](auto* slide) { delete slide; }
    );
    _slides = {};
}

std::ostream& operator<<(std::ostream& os, const SlideLibrary& lib)
{
    os << "Slide Library Name: " << lib.name() << "\n";
    os << "Slide Library Size: " << lib.size() << "\n";

    os << "Slide Library Header:\n";
    os << lib.header();

    os << "Slide Library Dirs:\n";
    std::for_each(
        lib.dirs().cbegin(), lib.dirs().cend(),
        [&os](auto* dir) {
            os << *dir;
        }
    );

    os << "Slide Library Slides:\n";
    std::for_each(
        lib.slides().cbegin(), lib.slides().cend(),
        [&os](auto* slide) {
            os << *slide;
        }
    );

    return os;
}
