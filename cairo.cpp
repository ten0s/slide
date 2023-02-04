#include <string.h> // basename

#include <iostream>
#include <fstream>
#include <memory>

#include <gtk/gtk.h>
#include <cairo.h>

#include "slide_file.h"
#include "slide_cairo_visitor.h"

using namespace std;

void usage(const std::string& prog)
{
    cerr << "Usage: " << prog << " SLIDE" << endl;
}

gboolean on_draw(GtkWidget* widget, cairo_t* cr, gpointer data)
{
    SlideFile* file = static_cast<SlideFile*>(data);

    SlideCairoVisitor visitor{cr};
    file->visit_records(visitor);

    guint width = gtk_widget_get_allocated_width(widget);
    guint height = gtk_widget_get_allocated_height(widget);

    //cairo_move_to(cr, 10, 20);
    cairo_arc (cr,
               width / 2.0, height / 2.0,
               MIN (width, height) / 2.0,
               0, 2 * G_PI);

    //GdkRGBA color = {rgb.red, rgb.green, rgb.blue, 1};
    //gdk_cairo_set_source_rgba(cr, &color);
    cairo_fill(cr);

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

            GtkWindow* window;
            {
                window = (GtkWindow*)gtk_window_new(GTK_WINDOW_TOPLEVEL);
                gtk_window_set_default_size(window, 300, 200);
                gtk_window_set_position    (window, GTK_WIN_POS_CENTER);
                gtk_window_set_title       (window, "Drawing");

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
