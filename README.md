AutoCAD slide library and command line utilities

Features:

* Parse slide file format (.sld) [version 2.0](https://web.archive.org/web/20191223211310/http://www.autodesk.com/techpubs/autocad/acadr14/dxf/slide_file_format_al_u05_b.htm)
  and [version 1.0](https://web.archive.org/web/20191223211321/https://www.autodesk.com/techpubs/autocad/acadr14/dxf/old_slide_header_al_u05_b.htm)
* Parse slide library file format (.slb) [version 1.0](https://web.archive.org/web/20191229052120/http://www.autodesk.com/techpubs/autocad/acadr14/dxf/slide_library_file_format_al_u05_b.htm)
* Print various info about slide and slide library
* Create slide library from slides
* Import slides to slide library
* Export slides from slide library
* Delete slides from slide library
* Convert slide to PNG and SVG
* Create slide from text records
* Draw slide on [Cairo](https://www.cairographics.org/) surface
* Draw slide on [Cairo](https://www.cairographics.org/) surface
  via [GObject Introspection](https://gi.readthedocs.io/en/latest/)
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
$ export LD_LIBRARY_PATH=$PWD/install/lib GI_TYPELIB_PATH=$PWD/install/lib/girepository-1.0
$ src/slide-create  ...
$ src/slide-info    ...
$ src/slide-view    ...
$ src/slide-convert ...
$ src/slidelib-create ...
$ src/slidelib-delete ...
$ src/slidelib-export ...
$ src/slidelib-import ...
```

## License

The project is licensed under the GNU General Public License v3.0 or later.
See [LICENSE](LICENSE) or
https://spdx.org/licenses/GPL-3.0-or-later.html
for full license information.
