#include <iostream>
#include <gtk/gtk.h>
#include <cairo.h>
#include "slide.hpp"
#include "slide_library.hpp"
#include "slide_record_visitor_cairo.hpp"
#include "slide_util.hpp"

void usage(const std::string& prog)
{
    std::cerr << "Usage: " << prog << " <SLIDE | SLIDELIB SLIDE>\n";
}

gboolean on_draw(GtkWidget* widget, cairo_t* cr, gpointer data)
{
    const Slide* slide = static_cast<const Slide*>(data);

    unsigned src_width = slide->header().high_x_dot();
    unsigned src_height = slide->header().high_y_dot();

    guint dst_width = gtk_widget_get_allocated_width(widget);
    guint dst_height = gtk_widget_get_allocated_height(widget);

    cairo_set_line_width(cr, 1);

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_rectangle(cr, 0, 0, dst_width, dst_height);
    cairo_fill(cr);

    SlideRecordVisitorCairo visitor{
        cr,
        src_width, src_height,
        0, 0,
        dst_width, dst_height,
    };
    slide->visit_records(visitor);

    return FALSE;
}

void show_window(const char* title, const Slide* slide) {
    GtkWindow* window;
    {
        window = (GtkWindow*)gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_default_size(window, 300, 200);
        gtk_window_set_position    (window, GTK_WIN_POS_CENTER);
        gtk_window_set_title       (window, title);

        g_signal_connect(window, "destroy", gtk_main_quit, NULL);
    }

    GtkDrawingArea* drawingArea;
    {
        drawingArea = (GtkDrawingArea*)gtk_drawing_area_new();
        gtk_container_add(GTK_CONTAINER(window), (GtkWidget*)drawingArea);

        g_signal_connect((GtkWidget*)drawingArea, "draw", G_CALLBACK(on_draw), (void*)slide);
    }

    gtk_widget_show_all((GtkWidget*)window);
    gtk_main();
}

int main (int argc, char* argv[]) {
    gtk_init(&argc, &argv);

    try {
        if (argc == 2) {

            std::string filename = argv[1];
            if (get_ext(filename) == ".sld") {

                Slide slide = Slide::from_file(filename);
                std::cout << slide;
                show_window(slide.name().c_str(), &slide);

            } else {
                throw std::runtime_error("Expected slide file");
            }

        } else if (argc == 3) {

            std::string filename = argv[1];
            std::string name = argv[2];
            if (get_ext(filename) == ".slb") {

                SlideLibrary library = SlideLibrary::from_file(filename);
                const Slide* slide = library.find(name);
                if (!slide) {
                    try {
                        size_t idx = std::stol(name);
                        slide = library.find(idx);
                    } catch (...) { }
                }

                if (slide) {
                    std::cout << *slide;
                    show_window(slide->name().c_str(), slide);
                } else {
                    throw std::runtime_error("Slide '" + name + "' not found");
                }

            } else {
                throw std::runtime_error("Expected slide library");
            }

        } else {
            usage(basename(argv[0]));
            return 1;
        }

    } catch (...) {
        // TODO:
        throw;
        //return 1;
    }

    return 0;
}
