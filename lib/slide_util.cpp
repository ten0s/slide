#include <algorithm> // std::transform
#include <filesystem>
#include "slide_util.hpp"

namespace fs = std::filesystem;

namespace libslide {

std::string basename(const std::string& filename)
{
    return fs::path(filename).filename().u8string();
}

std::string get_ext(const std::string& filename)
{
    return fs::path(filename).extension().u8string();
}

std::string strip_ext(const std::string& filename)
{
    return fs::path(filename).stem().u8string();
}

std::string to_upper(const std::string& in)
{
    std::string out{in};
    std::transform(
        out.begin(), out.end(),
        out.begin(),
        [](auto c) { return std::toupper(c); }
    );
    return out;
}

} // namespace libslide
