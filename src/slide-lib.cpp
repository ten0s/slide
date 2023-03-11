/**
 *   AutoCAD slide command line utility
 *
 *   Copyright (C) 2023 Dmitry Klionsky aka ten0s <dm.klionsky@gmail.com>
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
#include <string>
#include <boost/program_options.hpp>

#include "../lib/slide.hpp"
#include "../lib/slide_binary_writer.hpp"
#include "../lib/slide_library.hpp"
#include "../lib/slide_library_binary_writer.hpp"
#include "../lib/slide_util.hpp"
#include "../lib/slide_version.hpp"

namespace po = boost::program_options;
using namespace libslide;

template<typename T>
static void
print_usage(std::ostream& os, const std::string& prog, const T& options)
{
    os << "Usage: " << prog << " [options] <FILE.slb>\n"
       << options << "\n";
}

static int
export_slide_to_file(const SlideLibrary& lib, const std::string& name)
{
    const Slide* slide = lib.find(name);
    if (!slide) {
        try {
            size_t idx = std::stol(name);
            slide = lib.find(idx);
        } catch (...) { }
    }

    if (slide) {
        std::string filename = slide->name() + ".sld";
        std::ofstream ofs {filename, std::ios::binary};
        write_slide_binary(ofs, *slide);
        return 0;
    } else {
        std::cerr << "Error: Library slide not found: " << name << "\n";
        return 1;
    }
}

static int
import_slide_from_file(SlideLibrary& lib, const std::string& libfile, const std::string& sldfile)
{
    auto ext = to_upper(get_ext(sldfile));
    if (ext == ".SLD") {
        try {
            //Slide slide = Slide::from_file(sldfile);
            //lib.append(slide);
            // TODO: make backup?
            std::ofstream ofs {"new-" + libfile, std::ios::binary};
            write_slide_library_binary(ofs, lib);
            return 0;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
            return 1;
        }
    } else {
        std::cerr << "Error: Invalid slide extension: " << ext << "\n";
        return 1;
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
        ("export,e",
         po::value<std::string>(),
         "export slide to file")
        ("import,i",
         po::value<std::string>(),
         "import slide from file")
        ;

    po::options_description hidden("Hidden options");
    hidden.add_options()
       ("names",
         po::value<std::vector<std::string>>(),
         "FILE.slb")
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
        if (names.size() >= 1) {
            auto libfile = names[0];
            auto ext = to_upper(get_ext(libfile));
            if (ext == ".SLB") {

                try {
                    SlideLibrary lib = SlideLibrary::from_file(libfile);

                    if (vm.count("export")) {
                        auto name = vm["export"].as<std::string>();
                        return export_slide_to_file(lib, name);
                    }

                    if (vm.count("import")) {
                        auto sldfile = vm["import"].as<std::string>();
                        return import_slide_from_file(lib, libfile, sldfile);
                    }

                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << "\n";
                    return 1;
                }

            } else {
                std::cerr << "Error: Invalid library extension: " << ext << "\n";
                return 1;
            }
        }
    }

    print_usage(std::cerr, prog, visible_options);
    return 1;
}
