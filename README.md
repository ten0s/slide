AutoCAD Slide(s) Library

Features:

* Parse slide (.sld) and slide library (.slb)
* Slide drawing on Cairo surface
* Slide drawing on Cairo surface via [GObject Introspection](https://gi.readthedocs.io/en/latest/)

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
$ make install [PREFIX=/usr/local]
```

## Test

```
$ export LD_LIBRARY_PATH=`pwd` GI_TYPELIB_PATH=`pwd`
$ ...
```

## License

?
