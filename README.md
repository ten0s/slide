AutoCAD slide library and command line utility

Features:

* Parse slide file format (.sld) [version 2.0](https://web.archive.org/web/20191223211310/http://www.autodesk.com/techpubs/autocad/acadr14/dxf/slide_file_format_al_u05_b.htm)
  and [version 1.0](https://web.archive.org/web/20191223211321/https://www.autodesk.com/techpubs/autocad/acadr14/dxf/old_slide_header_al_u05_b.htm).
* Parse slide library file format (.slb) [version 1.0](https://web.archive.org/web/20191229052120/http://www.autodesk.com/techpubs/autocad/acadr14/dxf/slide_library_file_format_al_u05_b.htm)
* Print various info about slide and slide library
* Slide drawing on [Cairo](https://www.cairographics.org/) surface
* Slide drawing on [Cairo](https://www.cairographics.org/) surface
  via [GObject Introspection](https://gi.readthedocs.io/en/latest/)
* Linux, Windows and MacOS support

Todo:

* Convert slide to png
* Make slide from text records
* Make slide library from slides (what SLIDELIB does)
* Extract slides from slide library
* Add slide to slide library
* Delete slide from slide library
* Rename slides in slide library

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
$ src/slide ...
$ src/slide-view ..
```

## License

The project is licensed under the GNU General Public License v3.0 or later.
See [LICENSE](LICENSE) or
https://spdx.org/licenses/GPL-3.0-or-later.html
for full license information.
