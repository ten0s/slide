#include <filesystem>
#include <iostream>
#include <sstream>
#include <tuple>
#include <variant>
#include <unordered_map>
#include "slide_draw.h"
#include "slide.hpp"
#include "slide_cache.hpp"
#include "slide_library.hpp"
#include "slide_library_directory.hpp"
#include "slide_record_visitor_cairo.hpp"
#include "slide_util.hpp"

static SlideCache cache;

using variant_t = std::variant<std::tuple<std::string, std::string>, std::string>;

static variant_t
parse_slide_uri(const std::string& slide_uri);

static std::string
join_slide_uri(const variant_t& v);

static std::string
normalize_slide_file(const std::string& file);

static std::string
normalize_slide_name(const std::string& name);

static std::string
make_slide_lib_name_uri(const std::string& file, const std::string& name);

static std::string
normalize_slide_lib_name(const std::string& file, const std::string& name);

int slide_draw(cairo_t *cr,
               unsigned x,
               unsigned y,
               unsigned width,
               unsigned height,
               const char *slide_uri)
{
    std::cout << "+slide_draw "
              << x << " " << y << " "
              << width << " " << height << " "
              << slide_uri << "\n";

    try {
        auto parsed_uri = parse_slide_uri(slide_uri);
        auto normal_uri = join_slide_uri(parsed_uri);

        // Try slides cache first.
        const Slide *slide_ptr = cache.find(normal_uri);

        if (!slide_ptr) {
            std::visit([](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;

                if constexpr (std::is_same_v<T, std::string>) {
                    std::cout << "Slide file: " << arg << "\n";

                    // Read slide file.
                    Slide slide = Slide::from_file(arg);

                    // Move slide.
                    Slide* moved_slide = new Slide{std::move(slide)};

                    // Store moved slide to cache.
                    auto normal_uri = normalize_slide_file(arg);
                    cache.store(normal_uri, moved_slide);

                } else {
                    auto [file, name] = arg;
                    std::cout << "Slide library: " << file << ", "
                              << "name: "    << name << "\n";

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
                        cache.store(normal_uri, moved_slide);
                    }
                }

            }, parsed_uri);
        }

        // Try slides cache again
        slide_ptr = cache.find(normal_uri);

        if (!slide_ptr) {
            std::ostringstream ss;
            ss << "Slide " << slide_uri << " not found";
            throw std::runtime_error(ss.str());
        }

        unsigned sld_width = slide_ptr->header().high_x_dot();
        unsigned sld_height = slide_ptr->header().high_y_dot();
        double   sld_aspect_ratio = slide_ptr->header().aspect_ratio();

        // Draw slide
        SlideRecordVisitorCairo visitor{
            cr,
            sld_width, sld_height,
            sld_aspect_ratio,
            x, y,
            width, height
        };
        slide_ptr->visit_records(visitor);
    } catch (const std::exception& e) {
        std::ostringstream ss;
        ss << "Exception: " << e.what() << "\n";
        std::cerr << ss.str();
        return -1;
    }

    std::cout << "-slide_draw\n";
    return 0;
}

static variant_t
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
join_slide_uri(const variant_t& var)
{
    return std::visit([](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, std::string>) {
            return normalize_slide_file(arg);
        } else {
            auto [file, name] = arg;
            return normalize_slide_lib_name(file, name);
        }
    }, var);
}

static std::string
normalize_slide_file(const std::string& file)
{
    return std::filesystem::absolute(file);
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
