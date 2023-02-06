#include <iostream>
#include <gtk/gtk.h>
#include <cairo.h>
#include "slide.hpp"
#include "slide_record_visitor_cairo.hpp"
#include "slide_util.hpp"

void usage(const std::string& prog)
{
    std::cerr << "Usage: " << prog << " SLIDE\n";
}

gboolean on_draw(GtkWidget* widget, cairo_t* cr, gpointer data)
{
    Slide* slide = static_cast<Slide*>(data);

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

int main (int argc, char* argv[]) {
    gtk_init(&argc, &argv);

    if (argc < 2) {
        usage(basename(argv[0]));
        return 1;
    }

    try {
        Slide slide = Slide::from_file(argv[1]);
        std::cout << slide;

        const char* title = slide.name().c_str();

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

            g_signal_connect((GtkWidget*)drawingArea, "draw", G_CALLBACK(on_draw), &slide);
        }

        gtk_widget_show_all((GtkWidget*)window);
        gtk_main();
    } catch (...) {
        // TODO:
        throw;
        //return 1;
    }

    return 0;
}
