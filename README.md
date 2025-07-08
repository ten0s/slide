[Русская версия](README-ru.md)

# AutoCAD slide library and command line utilities

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
* Draw slide on [Cairo](https://www.cairographics.org/) surface | [Example](#draw-slide-on-cairo-surface)
* Draw slide on [Cairo](https://www.cairographics.org/) surface via [GObject Introspection](https://gi.readthedocs.io/en/latest/) | [Example](#draw-slide-on-cairo-surface-via-gobject-introspection)
* Backend for the `slide_image` function in [VeLisp](https://github.com/ten0s/velisp)
* Windows, MacOS and Linux support

## Install

### Windows

You can find the latest release [here](https://github.com/ten0s/slide/releases).

#### Install via automatic setup

1. Download https://github.com/ten0s/slide/releases/download/0.10.2/slide-0.10.2-win-x64-setup.zip
2. Open Downloads folder
3. Double-click on slide-0.10.2-win-x64-setup.zip to open the archive
4. Run slide-0.10.2-win-x64-setup.exe
5. Follow the installation wizard. Default options are ok for most users

#### Install via binary archive

##### Download and unarchive

1. Download https://github.com/ten0s/slide/releases/download/0.10.2/slide-0.10.2-win-x64.zip
2. Open Downloads folder
3. Select slide-0.10.2-win-x64.zip
4. Press the right mouse button
5. Click 'Extract All...'
6. Enter 'C:\'
7. Click 'Extract'

##### Add the folder to PATH

1. Press 'Windows + Break' to open the System Info
2. Click 'Advanced system settings'
3. Click 'Environment Variables...'
4. Select 'Path' inside the User variables block
5. Click 'Edit...'
6. Click 'New...'
7. Enter 'C:\slide-0.10.2-win-x64\bin'
8. Click 'OK'
9. Click 'OK'

### MacOS

#### Homebrew

You should have [Homebrew](https://brew.sh/) installed.

```
% brew tap ten0s/velisp
% brew install slide
```

### Linux

```
$ make
$ make install [PREFIX=$PWD/install]
```

## Examples

### Create slide from text records

```
$ cat test/square.txt
(COLOR 1) ; red
(SOLID_FILL_POLYGON 10 10 10 190 190 190 190 10)

(COLOR 2) ; yellow
(SOLID_FILL_POLYGON 20 20 20 180 180 180 180 20)

(COLOR 3) ; green
(SOLID_FILL_POLYGON 30 30 30 170 170 170 170 30)

(COLOR 5) ; blue
(SOLID_FILL_POLYGON 40 40 40 160 160 160 160 40)

(COLOR 255) ; white
(SOLID_FILL_POLYGON 50 50 50 150 150 150 150 50)
```

```
$ slide-create --width 200 --height 200 square.sld test/square.txt
```

```
$ cat test/triangle.txt
(COLOR 1) ; red

(VECTOR 10 10 100 190)
(VECTOR 100 190 190 10)
(VECTOR 190 10 10 10)

(VECTOR 20 20 100 180)
(VECTOR 100 180 180 20)
(VECTOR 180 20 20 20)

(VECTOR 40 40 100 160)
(VECTOR 100 160 160 40)
(VECTOR 160 40 40 40)

(VECTOR 80 80 100 120)
(VECTOR 100 120 120 80)
(VECTOR 120 80 80 80)
```

```
$ slide-create --width 200 --height 200 triangle.sld test/triangle.txt
```

### Create slide library from slides

#### Create empty slide library

```
$ slidelib-create mylib.slb
```

#### Create slide library from slide file

```
$ slidelib-create mylib.slb square.sld
```

#### Create slide library from multiple slide files

```
$ slidelib-create mylib.slb *.sld
```

### Print various slide info

#### Print info about slide file

```
$ slide-info square.sld
Type   : AutoCAD Slide 2.0
Name   : square
Size   : 223
Width  : 200
Height : 200
Ratio  : 1
Endian : Little
```

#### Print info about slide in library

```
$ slide-info --what=records mylib.slb square
(COLOR 1)
(SOLID_FILL_POLYGON 10 10 10 190 190 190 190 10)
(COLOR 2)
(SOLID_FILL_POLYGON 20 20 20 180 180 180 180 20)
(COLOR 3)
(SOLID_FILL_POLYGON 30 30 30 170 170 170 170 30)
(COLOR 5)
(SOLID_FILL_POLYGON 40 40 40 160 160 160 160 40)
(COLOR 255)
(SOLID_FILL_POLYGON 50 50 50 150 150 150 150 50)
(END_OF_FILE)
```

#### Print info about slide in library by its index

```
$ slide-info --what=all mylib.slb 0
Info:
  Type   : AutoCAD Slide 2.0
  Name   : SQUARE
  Size   : 223
  Width  : 200
  Height : 200
  Ratio  : 1
  Endian : Little
Records:
  (COLOR 1)
  (SOLID_FILL_POLYGON 10 10 10 190 190 190 190 10)
  (COLOR 2)
  (SOLID_FILL_POLYGON 20 20 20 180 180 180 180 20)
  (COLOR 3)
  (SOLID_FILL_POLYGON 30 30 30 170 170 170 170 30)
  (COLOR 5)
  (SOLID_FILL_POLYGON 40 40 40 160 160 160 160 40)
  (COLOR 255)
  (SOLID_FILL_POLYGON 50 50 50 150 150 150 150 50)
  (END_OF_FILE)
```

### Print various slide library info

```
$ slide-info mylib.slb
Type   : AutoCAD Slide Library 1.0
Name   : mylib
Size   : 327
Slides : 1
```

```
$ slide-info --what=names mylib.slb
SQUARE
```

```
$ slide-info --what=all mylib.slb
Info:
  Type   : AutoCAD Slide Library 1.0
  Name   : mylib
  Size   : 327
  Slides : 1
Names:
  SQUARE
```

### Import slides to slide library

#### Import one slide file to library

```
$ slidelib-import mylib.slb triangle.sld
```

```
$ slide-info --what=all mylib.slb
Info:
  Type   : AutoCAD Slide Library 1.0
  Name   : mylib
  Size   : 494
  Slides : 2
Names:
  SQUARE
  TRIANGLE
```

#### Import multiple slide files to library

```
$ slidelib-import mylib.slb *.sld
```

### Export slides from slide library

#### Export one slide from library

```
$ slidelib-export mylib.slb square
```

#### Export all slides from library

```
$ slidelib-export --all mylib.slb
```

### Delete slides from slide library

```
$ slidelib-delete mylib.slb triangle
```

```
$ slide-info --what=all mylib.slb
Info:
  Type   : AutoCAD Slide Library 1.0
  Name   : mylib
  Size   : 327
  Slides : 1
Names:
  SQUARE
```

### Convert slide to PNG and SVG

```
$ slide-convert --to=png square.sld
```

```
$ slide-convert --to=svg mylib.slb square
```

### Draw slide on [Cairo](https://www.cairographics.org/) surface

```
$ slide-view square.sld
```

```
$ slide-view mylib.slb square
```

```
$ slile-view mylib.slb 0
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
