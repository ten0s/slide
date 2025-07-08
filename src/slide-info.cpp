/**
 *   AutoCAD slide command line utility
 *
 *   Copyright (C) 2023-2025 Dmitry Klionsky aka ten0s <dm.klionsky@gmail.com>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* SPDX-License-Identifier: GPL-3.0-or-later */

#include <iostream>
#include <optional>
#include <string>
#include <boost/program_options.hpp>

#include "../lib/slide.hpp"
#include "../lib/slide_info_text_writer.hpp"
#include "../lib/slide_library.hpp"
#include "../lib/slide_library_info_text_writer.hpp"
#include "../lib/slide_util.hpp"
#include "../lib/slide_version.hpp"

namespace po = boost::program_options;
using namespace libslide;

template<typename T>
static void
print_usage(std::ostream& os, const std::string& prog, const T& options)
{
    os << "Usage: " << prog << " [options] <FILE.sld | FILE.slb [NAME | NUM]>\n"
       << options << "\n";
}

static
std::optional<slide_info_t>
parse_slide_info(const std::string& raw)
{
    auto info = slide_info_t::NONE;

    auto what = to_upper(raw);
    if (what == "INFO") {
        info = slide_info_t::INFO;
    } else if (what == "RECORDS") {
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
    } else if (what == "ALL") {
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
        write_slide_info_text(os, slide, slide_info_t::INFO, "  ");
        os << "Records:\n";
        write_slide_info_text(os, slide, slide_info_t::RECS, "  ");
        break;
    case slide_info_t::INFO:
    case slide_info_t::RECS:
        write_slide_info_text(os, slide, info);
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
        write_slide_library_info_text(os, lib, slide_library_info_t::INFO, "  ");
        os << "Names:\n";
        write_slide_library_info_text(os, lib, slide_library_info_t::NAMES, "  ");
        break;
    case slide_library_info_t::INFO:
    case slide_library_info_t::NAMES:
    case slide_library_info_t::DIRS:
        write_slide_library_info_text(os, lib, info);
    default:
        break;
    }
}

int main(int argc, char* argv[])
{
    auto prog = basename(argv[0]);

    po::options_description generic("Generic options");
    generic.add_options()
        ("help", "print help")
        ("version", "print version")
        ;

    po::options_description config("Configuration");
    config.add_options()
        ("what,w",
         po::value<std::string>()->default_value("info"),
         "print slide info (info, records, all)\n"
         "or library info (info, names, all)")
        ;

    po::options_description hidden("Hidden options");
    hidden.add_options()
       ("names",
         po::value<std::vector<std::string>>(),
         "FILE.sld or FILE.slb [NAME | NUM]")
        ;

    po::options_description all_options;
    all_options.add(generic).add(config).add(hidden);

    po::options_description visible_options("Allowed options");
    visible_options.add(generic).add(config);

    po::positional_options_description p;
    p.add("names", -1);

    po::variables_map vm;
    try {
        po::store(po::command_line_parser(argc, argv)
                  .options(all_options)
                  .positional(p)
                  .run(), vm);
        po::notify(vm);
    } catch (const po::unknown_option& e) {
        std::cerr << "Error: Unknown option: " << e.get_option_name() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    if (vm.count("help")) {
        print_usage(std::cout, prog, visible_options);
        return 0;
    }

    if (vm.count("version")) {
        std::cout << VERSION << "\n";
        return 0;
    }

    if (vm.count("names")) {
        auto names = vm["names"].as<std::vector<std::string>>();
        if (names.size() == 1) {
            auto file = names[0];
            auto ext = to_upper(get_ext(file));
            if (ext == ".SLD") {
                try {
                    auto slide = Slide::from_file(file);

                    if (vm.count("what")) {
                        auto raw = vm["what"].as<std::string>();
                        if (auto info = parse_slide_info(raw)) {
                            print_slide_info(std::cout, *slide, info.value());
                            return 0;
                        } else {
                            std::cerr << "Error: Invalid slide info: " << raw << "\n";
                            return 1;
                        }
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << "\n";
                    return 1;
                }
            } else if (ext == ".SLB") {
                try {
                    auto library = SlideLibrary::from_file(file);

                    if (vm.count("what")) {
                        auto raw = vm["what"].as<std::string>();
                        if (auto info = parse_library_info(raw)) {
                            print_library_info(std::cout, *library, info.value());
                            return 0;
                        } else {
                            std::cerr << "Error: Invalid slide library info: " << raw << "\n";
                            return 1;
                        }
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << "\n";
                    return 1;
                }
            } else {
                std::cerr << "Error: Invalid slide extension: " << file << "\n";
                return 1;
            }
        }

        if (names.size() == 2) {
            auto file = names[0];
            auto ext = to_upper(get_ext(file));
            if (ext == ".SLB") {
                auto name = names[1];

                try {
                    auto lib = SlideLibrary::from_file(file);
                    auto slide = lib->find(name);
                    if (!slide) {
                        try {
                            size_t idx = std::stol(name);
                            slide = lib->find(idx);
                        } catch (...) { }
                    }

                    if (slide) {
                        if (vm.count("what")) {
                            auto raw = vm["what"].as<std::string>();
                            if (auto info = parse_slide_info(raw)) {
                                print_slide_info(std::cout, *slide.value(), info.value());
                                return 0;
                            } else {
                                std::cerr << "Error: Invalid slide info: " << raw << "\n";
                                return 1;
                            }
                        }
                    } else {
                        std::cerr << "Error: Library slide not found: " << name << "\n";
                        return 1;
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << "\n";
                    return 1;
                }

            } else {
                std::cerr << "Error: Invalid library extension: " << file << "\n";
                return 1;
            }
        }
    }

    print_usage(std::cerr, prog, visible_options);
    return 1;
}
