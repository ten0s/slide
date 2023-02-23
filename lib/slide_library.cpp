#include <algorithm> // std::find_if
#include <memory>    // std::unique_ptr
#include <fstream>
#include <sstream>
#include "slide.hpp"
#include "slide_parser.hpp"
#include "slide_library.hpp"
#include "slide_library_parser.hpp"
#include "slide_library_directory.hpp"
#include "slide_util.hpp"

namespace libslide {

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
    for (auto& dir : _dirs) {
        delete dir;
    }
    _dirs = {};

    for (auto& slide : _slides) {
        delete slide;
    };
    _slides = {};
}

const Slide* SlideLibrary::find(const std::string& name) const
{
    std::string upper = to_upper(name);
    auto pos = std::find_if(
        _dirs.cbegin(), _dirs.cend(),
        [&](auto* dir) { return dir->name() == upper; }
    );

    if (pos != _dirs.cend()) {
        size_t idx = std::distance(_dirs.cbegin(), pos);
        return find(idx);
    } else {
        return nullptr;
    }
}

const Slide* SlideLibrary::find(size_t idx) const
{
    if (idx >= 0 && idx < _slides.size()) {
        return _slides.at(idx);
    }
    return nullptr;
}

std::ostream& operator<<(std::ostream& os, const SlideLibrary& lib)
{
    auto& header = lib.header();

    os << "Info:\n";

    os << "    Type : " << header.id_string() << "\n";
    os << "    Name : " << lib.name() << "\n";
    os << "    Size : " << lib.size() << "\n";

    os << "Slides:\n";
    for (auto& dir : lib.dirs()) {
        os << "    " << dir->name();
        os << "\n";
    };

    //os << "Slide Infos:\n";
    //for (auto& slide : lib.slides()) {
    //    os << *slide;
    //};

    return os;
}

} // namespace libslide
