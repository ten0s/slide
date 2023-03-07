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
#include <cairo.h>
#include <boost/program_options.hpp>

//#include "../lib/slide.hpp"
//#include "../lib/slide_library.hpp"
#include "../lib/slide_draw.h"
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

static void
write_to_png(const std::string& slide_uri, unsigned width, unsigned height, const std::string& filename)
{
    cairo_surface_t* cs = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t* cr = cairo_create(cs);

/*
    // Draw black background
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);
*/

    // Draw slide
    if (slide_draw(cr, 0, 0, width, height, slide_uri.c_str()) == 0) {
        cairo_surface_write_to_png(cs, filename.c_str());
    }

    cairo_surface_destroy(cs);
    cairo_destroy(cr);
}

int main (int argc, char* argv[])
{
    auto prog = basename(argv[0]);

    po::options_description generic("Generic options");
    generic.add_options()
        ("help", "print help")
        ("version", "print version")
        ;

/*
    po::options_description config("Configuration");
    config.add_options()
        ("to-png",
         po::value<std::string>(),
         "print slide info (info, records, all)\n"
         "or library info (info, names, all)")
        ;
*/

    po::options_description hidden("Hidden options");
    hidden.add_options()
       ("names",
         po::value<std::vector<std::string>>(),
         "FILE.sld or FILE.slb [NAME | NUM]")
        ;

    po::options_description all_options;
    all_options.add(generic).add(hidden);

    po::options_description visible_options("Allowed options");
    visible_options.add(generic);

    po::positional_options_description p;
    p.add("names", -1);

    po::variables_map vm;
    try {
        po::store(po::command_line_parser(argc, argv)
                  .options(all_options)
                  .positional(p)
                  .run(), vm);
        po::notify(vm);
    } catch (const boost::program_options::unknown_option& e) {
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

    const unsigned width  = 600;
    const unsigned height = 400;
    const std::string filename = "test.png";

    if (vm.count("names")) {
        auto names = vm["names"].as<std::vector<std::string>>();
        if (names.size() == 1) {
            auto file = names[0];
            auto ext = to_upper(get_ext(file));
            if (ext == ".SLD") {
                std::string uri = file;
                write_to_png(uri, width, height, filename);
                return 0;
            } else if (ext == ".SLB") {
                std::cerr << "Error: Expected slide name\n";
                return 1;
            } else {
                std::cerr << "Error: Invalid slide extension: " << ext << "\n";
                return 1;
            }
        }

        if (names.size() == 2) {
            auto file = names[0];
            auto ext = to_upper(get_ext(file));
            if (ext == ".SLB") {
                auto name = names[1];
                std::string uri = file + "(" + name + ")";
                write_to_png(uri, width, height, filename);
                return 0;
            } else {
                std::cerr << "Error: Invalid library extension: " << ext << "\n";
                return 1;
            }
        }
    }

    print_usage(std::cerr, prog, visible_options);
    return 1;
}
