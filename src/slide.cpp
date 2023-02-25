#include <iostream>
#include <optional>
#include <string>
#include <boost/program_options.hpp>

#include "../lib/slide.hpp"
#include "../lib/slide_printers.hpp"
#include "../lib/slide_library.hpp"
#include "../lib/slide_library_printers.hpp"
#include "../lib/slide_util.hpp"

namespace po = boost::program_options;
using namespace libslide;

template<typename T>
static void
print_usage(std::ostream& os, const std::string& prog, const T& desc)
{
    os << "Usage: " << prog << " [options] <FILE.sld | FILE.slb [NAME | NUM]>\n"
       << desc << "\n";
}

static
std::optional<slide_info_t>
parse_slide_info(const std::string& raw)
{
    auto info = slide_info_t::NONE;

    auto what = to_upper(raw);
    if (what == "INFO") {
        info = slide_info_t::INFO;
    } else if (what == "RECS") {
        info = slide_info_t::RECS;
    }else if (what == "ALL") {
        info = slide_info_t::ALL;
    } else {
        return {};
    }

    return info;
}

static
std::optional<slide_library_info_t>
parse_library_info(const std::string& raw)
{
    auto info = slide_library_info_t::NONE;

    auto what = to_upper(raw);
    if (what == "INFO") {
        info = slide_library_info_t::INFO;
    } else if (what == "NAMES") {
        info = slide_library_info_t::NAMES;
    } else if (what == "DIRS") {
        info = slide_library_info_t::DIRS;
    }else if (what == "ALL") {
        info = slide_library_info_t::ALL;
    } else {
        return {};
    }

    return info;
}

static void
print_slide_info(std::ostream& os, const Slide& slide, slide_info_t info)
{
    //using enum slide_info_t; since C++20

    switch (info) {
    case slide_info_t::ALL:
        os << "Info:\n";
        os << SlidePrinter<slide_info_t::INFO>{slide, "  "};
        os << "Records:\n";
        os << SlidePrinter<slide_info_t::RECS>{slide, "  "};
        break;
    case slide_info_t::INFO:
        os << SlidePrinter<slide_info_t::INFO>{slide};
        break;
    case slide_info_t::RECS:
        os << SlidePrinter<slide_info_t::RECS>{slide};
        break;
    default:
        break;
    }
}

static void
print_library_info(std::ostream& os, const SlideLibrary& lib, slide_library_info_t info)
{
    //using enum slide_library_info_t; since C++20

    switch (info) {
    case slide_library_info_t::ALL:
        os << "Info:\n";
        os << SlideLibraryPrinter<slide_library_info_t::INFO>{lib, "  "};
        os << "Names:\n";
        os << SlideLibraryPrinter<slide_library_info_t::NAMES>{lib, "  "};
        break;
    case slide_library_info_t::INFO:
        os << SlideLibraryPrinter<slide_library_info_t::INFO>{lib};
        break;
    case slide_library_info_t::NAMES:
        os << SlideLibraryPrinter<slide_library_info_t::NAMES>{lib};
        break;
    case slide_library_info_t::DIRS:
        os << SlideLibraryPrinter<slide_library_info_t::DIRS>{lib};
        break;
    default:
        break;
    }
}

int main(int argc, char* argv[])
{
    auto prog = basename(argv[0]);

    /*
    options_description general("General options");
    general.add_options()
        ("help", "produce a help message")
        ("help-module", po::value<string>(),
         "produce a help for a given module")
        ("version", "output the version number")
    ;
    */

    po::options_description desc("Allowed options");
    desc.add_options()
        ("info,i", po::value<std::string>()->default_value("info"), "print slide or library info")
        ("names", po::value<std::vector<std::string>>(), "FILE.sld or FILE.slb [NAME | NUM]")
        ("help,h", "print help")
        ("version", "print version number")
        ;

    po::positional_options_description p;
    p.add("names", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv)
              .options(desc)
              .positional(p)
              .run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        print_usage(std::cout, prog, desc);
        return 0;
    }

    if (vm.count("version")) {
        // TODO
        std::cout << "0.0.0" << "\n";
        return 0;
    }

    if (vm.count("names")) {
        auto names = vm["names"].as<std::vector<std::string>>();
        if (names.size() == 1) {
            auto file = names[0];
            auto ext = get_ext(file);
            if (ext == ".sld") {
                Slide slide = Slide::from_file(file);

                if (vm.count("info")) {
                    auto raw = vm["info"].as<std::string>();
                    if (auto info = parse_slide_info(raw)) {
                        print_slide_info(std::cout, slide, info.value());
                        return 0;
                    } else {
                        std::cerr << "Invalid slide info value: " << raw << "\n";
                        goto usage;
                    }
                }

                // TODO: Add other commands here
                return 0;
            } else if (ext == ".slb") {
                SlideLibrary library = SlideLibrary::from_file(file);

                if (vm.count("info")) {
                    auto raw = vm["info"].as<std::string>();
                    if (auto info = parse_library_info(raw)) {
                        print_library_info(std::cout, library, info.value());
                        return 0;
                    } else {
                        std::cerr << "Invalid slide library info value: " << raw << "\n";
                        goto usage;
                    }
                }

                // TODO: Add other commands here
                return 0;
            } else {
                std::cerr << "Invalid slide extension: " << ext << "\n";
                goto usage;
            }
        }

        if (names.size() == 2) {
            auto file = names[0];
            auto ext = get_ext(file);
            if (ext == ".slb") {
                auto name = names[1];
                SlideLibrary library = SlideLibrary::from_file(file);
                const Slide* slide = library.find(name);
                if (!slide) {
                    try {
                        size_t idx = std::stol(name);
                        slide = library.find(idx);
                    } catch (...) { }
                }

                if (slide) {
                    if (vm.count("info")) {
                        auto raw = vm["info"].as<std::string>();
                        if (auto info = parse_slide_info(raw)) {
                            print_slide_info(std::cout, *slide, info.value());
                            return 0;
                        } else {
                            std::cerr << "Invalid slide info value: " << raw << "\n";
                            goto usage;
                        }
                    }

                    // TODO: Add other commands here
                    return 0;
                } else {
                    std::cerr << "Library slide not found: " << name << "\n";
                    return 1;
                }
            } else {
                std::cerr << "Invalid library extension: " << ext << "\n";
                goto usage;
            }
        }

    } else {
        std::cerr << "Invalid names\n";
        goto usage;
    }

usage:
    print_usage(std::cerr, prog, desc);
    return 1;
}
