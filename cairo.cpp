#include <iostream>
#include <gtk/gtk.h>
#include <cairo.h>
#include "slide_draw.h"
#include "slide_util.hpp"

using namespace libslide;

void usage(const std::string& prog)
{
    std::cerr << "Usage: " << prog << " <SLIDE.sld | SLIDELIB.slb NAME>\n";
}

gboolean on_draw(GtkWidget* widget, cairo_t* cr, gpointer data)
{
    const char* slide_uri = static_cast<const char*>(data);

    guint width = gtk_widget_get_allocated_width(widget);
    guint height = gtk_widget_get_allocated_height(widget);

    // Draw black background
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);

    // Draw slide
    slide_draw(
        cr,
        0, 0,
        width, height,
        slide_uri
    );

    return FALSE;
}

void show_window(const char* title, const char* slide_uri)
{
    GtkWindow* window = (GtkWindow*)gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(window, 300, 200);
    gtk_window_set_position    (window, GTK_WIN_POS_CENTER);
    gtk_window_set_title       (window, title);
    g_signal_connect(window, "destroy", gtk_main_quit, NULL);

    GtkDrawingArea* drawingArea = (GtkDrawingArea*)gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), (GtkWidget*)drawingArea);
    g_signal_connect((GtkWidget*)drawingArea, "draw", G_CALLBACK(on_draw), (void*)slide_uri);

    gtk_widget_show_all((GtkWidget*)window);
    gtk_main();
}

int main (int argc, char* argv[])
{
    gtk_init(&argc, &argv);

    if (argc == 2) {
        std::string filename = argv[1];
        if (get_ext(filename) == ".sld") {
            std::string title = strip_ext(basename(filename));
            std::string uri = filename;
            show_window(title.c_str(), uri.c_str());
            return 0;
        }
    }

    if (argc == 3) {
        std::string filename = argv[1];
        std::string name = argv[2];
        if (get_ext(filename) == ".slb") {
            std::string title = strip_ext(basename(filename)) + "(" + name + ")";
            std::string uri = filename + "(" + name + ")";
            show_window(title.c_str(), uri.c_str());
            return 0;
        }
    }

    usage(basename(argv[0]));
    return 1;
}
