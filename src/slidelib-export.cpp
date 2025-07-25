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
#include <fstream>
#include <memory>
#include <string>
#include <boost/program_options.hpp>

#include "../lib/slide.hpp"
#include "../lib/slide_binary_writer.hpp"
#include "../lib/slide_library.hpp"
#include "../lib/slide_library_directory.hpp"
#include "../lib/slide_library_binary_writer.hpp"
#include "../lib/slide_util.hpp"
#include "../lib/slide_version.hpp"

namespace po = boost::program_options;
using namespace libslide;

static bool MAKE_BACKUP = true;

template<typename T>
static void
print_usage(std::ostream& os, const std::string& prog, const T& options)
{
    os << "Usage: " << prog << " [options] <FILE.slb> [NAME ...]\n"
       << options << "\n";
}

static int
export_slides(const SlideLibrary& lib,
              const std::vector<std::string>& names)
{
    for (const auto& name : names) {
        auto slide = lib.find(name);
        if (!slide) {
            try {
                size_t idx = std::stol(name);
                slide = lib.find(idx);
            } catch (...) { }
        }

        if (slide) {
            auto slidefile = slide.value()->name() + ".sld";
            if (MAKE_BACKUP) { make_backup(slidefile); }
            std::ofstream ofs{slidefile, std::ios::binary};
            write_slide_binary(ofs, *slide.value());
        } else {
            std::cerr << "Error: Library slide not found: " << name << "\n";
            return 1;
        }
    }

    return 0;
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
        ("all",
         "export all")
        ("no-bak",
         "don't create backup files")
        ;

    po::options_description hidden("Hidden options");
    hidden.add_options()
       ("names",
         po::value<std::vector<std::string>>(),
         "FILE.slb [NAME ...]")
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

    if (vm.count("no-bak")) {
        MAKE_BACKUP = false;
    }

    if (vm.count("names")) {
        auto names = vm["names"].as<std::vector<std::string>>();
        if (names.size() >= 1) {
            auto file = names[0];
            auto ext = to_upper(get_ext(file));
            if (ext == ".SLB") {
                try {
                    auto lib = SlideLibrary::from_file(file);
                    auto slides = tail(names);
                    if (vm.count("all")) {
                        auto& dirs = lib->dirs();
                        std::transform(
                            dirs.cbegin(), dirs.cend(),
                            std::back_inserter(slides),
                            [](const auto& dir) { return dir->name(); }
                        );
                    }
                    return export_slides(*lib, slides);
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
