#include <string.h> // basename

#include <iostream>
#include <fstream>
#include <memory>

#include <gtk/gtk.h>
#include <cairo.h>

#include "slide_file.hpp"
#include "slide_visitor_cairo.hpp"

using namespace std;

void usage(const std::string& prog)
{
    cerr << "Usage: " << prog << " SLIDE" << endl;
}

gboolean on_draw(GtkWidget* widget, cairo_t* cr, gpointer data)
{
    SlideFile* file = static_cast<SlideFile*>(data);

    guint width = gtk_widget_get_allocated_width(widget);
    guint height = gtk_widget_get_allocated_height(widget);

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);

    SlideCairoVisitor visitor{cr};
    file->visit_records(visitor);

    return FALSE;
}

int main (int argc, char* argv[]) {
    gtk_init(&argc, &argv);

    const std::string prog = basename(argv[0]);
    if (argc < 2) {
        usage(prog);
        return 1;
    }

    const std::string name = argv[1];
    ifstream is{name, std::ios::binary | std::ios::ate};
    if (is.is_open()) {
        // Read the whole file.
        size_t size = is.tellg();
        std::unique_ptr<uint8_t[]> buf{new uint8_t[size]};
        is.seekg(0);
        if (is.read((char*)buf.get(), size)) {

            auto [file, offset] = parse_slide_file(name, buf.get(), size);
            cout << file;

            const char* title = file.name().c_str();
            const size_t width = file.header().high_x_dot();
            const size_t height = file.header().high_y_dot();

            GtkWindow* window;
            {
                window = (GtkWindow*)gtk_window_new(GTK_WINDOW_TOPLEVEL);
                gtk_window_set_default_size(window, width, height);
                gtk_window_set_position    (window, GTK_WIN_POS_CENTER);
                gtk_window_set_title       (window, title);

                g_signal_connect(window, "destroy", gtk_main_quit, NULL);
            }

            GtkDrawingArea* drawingArea;
            {
                drawingArea = (GtkDrawingArea*) gtk_drawing_area_new();
                gtk_container_add(GTK_CONTAINER(window), (GtkWidget*)drawingArea);

                g_signal_connect((GtkWidget*)drawingArea, "draw", G_CALLBACK(on_draw), &file);
            }

            gtk_widget_show_all((GtkWidget*)window);
            gtk_main();

        } else {
            cerr << "Read failed\n";
        }

    } else {
        // TODO: use `perror` or analog
        cerr << "File not found\n";
    }

    return 0;
}
