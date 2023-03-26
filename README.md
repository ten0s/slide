AutoCAD slide library and command line utilities

Features:

* Parse slide file format (.sld) [version 2.0](https://web.archive.org/web/20191223211310/http://www.autodesk.com/techpubs/autocad/acadr14/dxf/slide_file_format_al_u05_b.htm)
  and [version 1.0](https://web.archive.org/web/20191223211321/https://www.autodesk.com/techpubs/autocad/acadr14/dxf/old_slide_header_al_u05_b.htm)
* Parse slide library file format (.slb) [version 1.0](https://web.archive.org/web/20191229052120/http://www.autodesk.com/techpubs/autocad/acadr14/dxf/slide_library_file_format_al_u05_b.htm)
* Create slide library from slides | [Example](#create-slide-library-from-slides)
* Import slides to slide library | [Example](#import-slides-to-slide-library)
* Export slides from slide library | [Example](#export-slides-from-slide-library)
* Delete slides from slide library | [Example](#delete-slides-from-slide-library)
* Convert slide to PNG and SVG | [Example](#convert-slide-to-png-and-svg)
* Create slide from text records | [Example](#create-slide-from-text-records)
* Print various info about slide | [Example](#print-various-info-about-slide)
* Print various info about slide library | [Example](#print-various-info-about-slide-library)
* Draw slide on [Cairo](https://www.cairographics.org/) surface| [Example](#draw-slide-on-cairo-surface)
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

## Examples

```
$ export PATH=$PATH:install/bin
```

### Create slide from text records

```
$ slide-create ...
```

### Convert slide to PNG and SVG

```
$ slide-convert ...
```

### Create slide library from slides

```
$ slidelib-create ...
```

### Print various slide info

```
$ slide-info ...
```

### Print various slide library info

```
$ slide-info ...
```

### Import slides to slide library

```
$ slidelib-import ...
```

### Export slides from slide library

```
$ slidelib-export ...
```

### Delete slides from slide library

```
$ slidelib-delete ...
```

### Draw slide on [Cairo](https://www.cairographics.org/) surface

```
$ slide-view abc.sld
$ slile-view mylib.slb xyz
$ slile-view mylib.slb 2
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
