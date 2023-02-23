AutoCAD slide library and command line utility

Features:

* Parse slide (.sld) and slide library (.slb)
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

?
