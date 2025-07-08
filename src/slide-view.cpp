/**
 *   AutoCAD slide viewer
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
#include <gtk/gtk.h>
#include <cairo.h>
#include <boost/program_options.hpp>

#include "../lib/slide_colors.hpp"
#include "../lib/slide_draw_cairo.h"
#include "../lib/slide_util.hpp"
#include "../lib/slide_version.hpp"

namespace po = boost::program_options;
using namespace libslide;

static constexpr int WINDOW_WIDTH  = 800;
static constexpr int WINDOW_HEIGHT = 600;

template<typename T>
static void
print_usage(std::ostream& os, const std::string& prog, const T& options)
{
    os << "Usage: " << prog << " [options] <FILE.sld | FILE.slb [NAME | NUM]>\n"
       << options << "\n";
}

gboolean on_draw(GtkWidget* widget, cairo_t* cr, gpointer data)
{
    auto tuple = *static_cast<std::tuple<int, const char*>*>(data);
    auto [background, slide_uri] = tuple;

    guint width = gtk_widget_get_allocated_width(widget);
    guint height = gtk_widget_get_allocated_height(widget);

    // Draw background
    if (background >= 0) {
        RGB rgb = AutoCAD::colors[background];
        cairo_set_source_rgb(cr,
                             1.0 * rgb.red   / MAX_COLOR,
                             1.0 * rgb.green / MAX_COLOR,
                             1.0 * rgb.blue  / MAX_COLOR);
        cairo_rectangle(cr, 0, 0, width, height);
        cairo_fill(cr);
    }

    // Draw slide
    if (slide_draw_cairo(cr, 0, 0, width, height, slide_uri) != 0) {
        gtk_main_quit();
    }

    return FALSE;
}

void show_window(const char* title, int width, int height, int background, const char* slide_uri)
{
    GtkWindow* window = (GtkWindow*)gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(window, width, height);
    gtk_window_set_position    (window, GTK_WIN_POS_CENTER);
    gtk_window_set_title       (window, title);
    g_signal_connect(window, "destroy", gtk_main_quit, NULL);

    GtkDrawingArea* drawingArea = (GtkDrawingArea*)gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), (GtkWidget*)drawingArea);

    auto tuple = std::make_tuple(background, slide_uri);
    g_signal_connect((GtkWidget*)drawingArea, "draw", G_CALLBACK(on_draw), (void*)&tuple);

    gtk_widget_show_all((GtkWidget*)window);
    gtk_main();
}

int main (int argc, char* argv[])
{
    gtk_init(&argc, &argv);

    auto prog = basename(argv[0]);

    po::options_description generic("Generic options");
    generic.add_options()
        ("help", "print help")
        ("version", "print version")
        ;

    po::options_description config("Configuration");
    config.add_options()
        ("width,w",
         po::value<unsigned>(),
         ("window width, " + std::to_string(WINDOW_WIDTH) + " by default").c_str())
        ("height,h",
         po::value<unsigned>(),
         ("window height, " + std::to_string(WINDOW_HEIGHT) + " by default").c_str())
        ("background,b",
         po::value<int>(),
         ("background AutoCAD color [-1, " + std::to_string(MAX_COLOR) + "]\n"
          "(https://gohtx.com/acadcolors.php),\n"
          "-1 for transparent, 0 (black) by default").c_str())
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

    int width = WINDOW_WIDTH;
    if (vm.count("width")) {
        width = vm["width"].as<unsigned>();
        if (width < 0) {
            std::cerr << "Error: Invalid 'width': " << width << "\n";
            return 1;
        }
    }

    int height = WINDOW_HEIGHT;
    if (vm.count("height")) {
        height = vm["height"].as<unsigned>();
        if (height < 0) {
            std::cerr << "Error: Invalid 'height': " << height << "\n";
        }
    }

    int background = 0;
    if (vm.count("background")) {
        background = vm["background"].as<int>();
        if (background < -1) {
            background = -1;
        }
        if (background > MAX_COLOR) {
            std::cerr << "Error: Invalid 'background'\n";
            return 1;
        }
    }

    if (vm.count("names")) {
        std::string title;
        std::string slide_uri;
        auto names = vm["names"].as<std::vector<std::string>>();
        if (names.size() == 1) {
            auto file = names[0];
            auto ext = to_upper(get_ext(file));
            if (ext == ".SLD") {
                title = strip_ext(basename(file));
                slide_uri = file;
            } else if (ext == ".SLB") {
                std::cerr << "Error: Expected slide name\n";
                return 1;
            } else {
                std::cerr << "Error: Invalid slide extension: " << file << "\n";
                return 1;
            }
        }

        if (names.size() >= 2) {
            auto file = names[0];
            auto ext = to_upper(get_ext(file));
            if (ext == ".SLB") {
                auto name = names[1];
                title = strip_ext(basename(file)) + "(" + name + ")";
                slide_uri = file + "(" + name + ")";
            } else {
                std::cerr << "Error: Invalid library extension: " << file << "\n";
                return 1;
            }
        }

        show_window(title.c_str(), width, height, background, slide_uri.c_str());
        return 0;
    }

    print_usage(std::cerr, prog, visible_options);
    return 1;
}
