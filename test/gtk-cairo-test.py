import cairo
import cairocffi
import gi

gi.require_version('Gtk', '3.0')
gi.require_version('SlideLib', '1.0')

from gi.repository import Gtk
from gi.repository import SlideLib

# https://cairocffi.readthedocs.io/en/stable/cffi_api.html#converting-pycairo-wrappers-to-cairocffi
# https://cairocffi.readthedocs.io/en/stable/cffi_api.html#converting-cairocffi-wrappers-to-pycairo
def unsafe_cairo_ptr(ctx):
    if not isinstance(ctx, cairo.Context):
        raise TypeError(f'Expected a cairo.Context, got {ctx}')

    context = cairocffi.Context._from_pointer(
        cairocffi.ffi.cast('cairo_t **', id(ctx) + object.__basicsize__)[0],
        incref=True)
    return int(cairocffi.ffi.cast('uintptr_t', context._pointer))

def on_draw(da, ctx):
    offset = 5
    width  = da.get_allocated_width()
    heigth = da.get_allocated_height()

    # Draw black background
    ctx.set_source_rgb(0, 0, 0)
    ctx.rectangle(offset, offset, width-2*offset, heigth-2*offset)
    ctx.fill()

    # Draw slide
    SlideLib.draw(unsafe_cairo_ptr(ctx),
                  offset, offset,
                  width-2*offset, heigth-2*offset,
                  'xyz.sld')

    return False

window = Gtk.Window(title='Slide View')
window.set_default_size(800, 600)
window.set_resizable(True)
window.connect('destroy', Gtk.main_quit)

drawingarea = Gtk.DrawingArea()
drawingarea.connect('draw', on_draw)
window.add(drawingarea)

window.show_all()
Gtk.main()
