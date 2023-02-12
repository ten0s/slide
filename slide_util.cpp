#include <algorithm> // std::transform
#include <filesystem>
#include "slide_util.hpp"

namespace libslide {

std::string basename(const std::string& filename)
{
    namespace fs = std::filesystem;
    return fs::path(filename).filename();
}

std::string get_ext(const std::string& filename)
{
    return filename.substr(filename.rfind("."));
}

std::string strip_ext(const std::string& filename)
{
    return filename.substr(0, filename.rfind("."));
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
