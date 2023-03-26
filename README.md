AutoCAD slide library and command line utilities

Features:

* Parse slide file format (.sld) [version 2.0](https://web.archive.org/web/20191223211310/http://www.autodesk.com/techpubs/autocad/acadr14/dxf/slide_file_format_al_u05_b.htm)
  and [version 1.0](https://web.archive.org/web/20191223211321/https://www.autodesk.com/techpubs/autocad/acadr14/dxf/old_slide_header_al_u05_b.htm)
* Parse slide library file format (.slb) [version 1.0](https://web.archive.org/web/20191229052120/http://www.autodesk.com/techpubs/autocad/acadr14/dxf/slide_library_file_format_al_u05_b.htm)
* Print various info about slide and slide library (slide-info)
* Create slide library from slides (slidelib-create)
* Import slides to slide library (slidelib-import)
* Export slides from slide library (slidelib-export)
* Delete slides from slide library (slidelib-delete)
* Convert slide to PNG and SVG (slide-convert)
* Create slide from text records (slide-create)
* Draw slide on [Cairo](https://www.cairographics.org/) surface (slide-view)
* Draw slide on [Cairo](https://www.cairographics.org/) surface via [GObject Introspection](https://gi.readthedocs.io/en/latest/) | [Example](#draw-slide-on-cairo-surface-via-gobject-introspection)
* Linux, Windows and MacOS support

## Build

```
$ make
```

## Install

```
$ make install [PREFIX=$PWD/install]
```

## Test

```
$ src/slide-create  ...
$ src/slide-info    ...
$ src/slide-view    ...
$ src/slide-convert ...
$ src/slidelib-create ...
$ src/slidelib-delete ...
$ src/slidelib-export ...
$ src/slidelib-import ...
```

### Draw slide on [Cairo](https://www.cairographics.org/) surface via [GObject Introspection](https://gi.readthedocs.io/en/latest/)

```
$ export LD_LIBRARY_PATH=$PWD/install/lib GI_TYPELIB_PATH=$PWD/install/lib/girepository-1.0

$ python3 -m venv env
$ source env/bin/activate
$ pip3 install -r test/requirements.txt

$ python3 test/gtk-cairo-test.py
```

## License

The project is licensed under the GNU General Public License v3.0 or later.
See [LICENSE](LICENSE) or
https://spdx.org/licenses/GPL-3.0-or-later.html
for full license information.
