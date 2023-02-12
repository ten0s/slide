#include <filesystem>
#include <iostream>
#include <tuple>
#include <variant>
#include "slide.hpp"
#include "slide_library.hpp"
#include "slide_library_directory.hpp"
#include "slide_cache.hpp"
#include "slide_loader.hpp"
#include "slide_util.hpp"

namespace libslide {

static SlideCache cache;

using var_t = std::variant<std::tuple<std::string, std::string>, std::string>;

static std::string
normalize_slide_file(const std::string& file)
{
    return std::filesystem::absolute(file).u8string();
}

static std::string
normalize_slide_name(const std::string& name)
{
    return to_upper(name);
}

static std::string
make_slide_lib_name_uri(const std::string& file, const std::string& name)
{
    return file + "(" + name + ")";
}

static std::string
normalize_slide_lib_name(const std::string& file, const std::string& name)
{
    return make_slide_lib_name_uri(
        normalize_slide_file(file),
        normalize_slide_name(name)
    );
}

static var_t
parse_slide_uri(const std::string& slide_uri)
{
    // Check if it's a slide library or slide
    auto i = slide_uri.find("(");
    auto j = slide_uri.rfind(")");

    if (i >=0 && j == slide_uri.length()-1) {
        // It's a slide library
        return std::make_tuple(
            slide_uri.substr(0, i),
            slide_uri.substr(i+1, j-i-1)
        );
    } else if (i == std::string::npos && j == std::string::npos) {
        // It's a slide
        return slide_uri;
    } else {
        throw std::runtime_error("slide_uri: bad format");
    }
}

static std::string
join_slide_uri(const var_t& var)
{
   if (std::holds_alternative<std::string>(var)) {
       auto file = std::get<std::string>(var);
       return normalize_slide_file(file);
   } else {
       auto [file, name] = std::get<std::tuple<std::string, std::string>>(var);
       return normalize_slide_lib_name(file, name);
   }
}

static void
cache_slide_file(const std::string& file)
{
    // Read slide file.
    Slide slide = Slide::from_file(file);

    // Move slide.
    Slide* moved_slide = new Slide{std::move(slide)};

    // Store moved slide to cache.
    auto normal_uri = normalize_slide_file(file);
    cache.set(normal_uri, moved_slide);
}

static void
cache_slide_lib(const std::string& file)
{
    // Read slide library.
    SlideLibrary lib = SlideLibrary::from_file(file);

    // Store slides from library to cache.
    auto& dirs = lib.dirs();
    auto& slides = lib.slides();

    auto begin = dirs.begin();
    auto end = dirs.end();

    auto normal_file = normalize_slide_file(file);
    for (auto it = begin; it != end; ++it) {
        auto normal_name = normalize_slide_name((*it)->name());
        auto normal_uri = make_slide_lib_name_uri(normal_file, normal_name);

        // Move slide.
        size_t idx = std::distance(begin, it);
        Slide* moved_slide = new Slide(std::move(*slides.at(idx)));

        // Store moved slide to cache.
        cache.set(normal_uri, moved_slide);
    }
}

const Slide*
slide_from_uri(const std::string& slide_uri)
{
    auto parsed_uri = parse_slide_uri(slide_uri);
    auto normal_uri = join_slide_uri(parsed_uri);

    // Look slides cache.
    const Slide *slide = cache.get(normal_uri);
    if (slide) {
        return slide;
    }

    if (std::holds_alternative<std::string>(parsed_uri)) {
        auto file = std::get<std::string>(parsed_uri);
        cache_slide_file(file);
    } else {
        auto [file, _name] = std::get<std::tuple<std::string, std::string>>(parsed_uri);
        cache_slide_lib(file);
    }

    // Look slides cache again.
    return cache.get(normal_uri);
}

} // namespace libslide
