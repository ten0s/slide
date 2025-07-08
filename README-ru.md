[English version](README.md)

# Библиотека и утилиты для работы с AutoCAD слайдами

Функции:

* Поддержка слайдов (.sld) [версии 2.0](https://web.archive.org/web/20191223211310/http://www.autodesk.com/techpubs/autocad/acadr14/dxf/slide_file_format_al_u05_b.htm)
  и [версии 1.0](https://web.archive.org/web/20191223211321/https://www.autodesk.com/techpubs/autocad/acadr14/dxf/old_slide_header_al_u05_b.htm)
* Поддержка библиотек слайдов (.slb) [версии 1.0](https://web.archive.org/web/20191229052120/http://www.autodesk.com/techpubs/autocad/acadr14/dxf/slide_library_file_format_al_u05_b.htm)
* Создание библиотеки слайдов из отдельных слайдов | [Пример](#создание-библиотеки-слайдов-из-отдельных-слайдов)
* Импорт слайдов в библиотеку слайдов | [Пример](#импорт-слайдов-в-библиотеку-слайдов)
* Экспорт слайдов из библиотеки слайдов | [Пример](#экспорт-слайдов-из-библиотеки-слайдов)
* Удаление слайдов из библиотеки слайдов | [Пример](#удаление-слайдов-из-библиотеки-слайдов)
* Конвертация слайда в PNG и SVG | [Пример](#конвертация-слайда-в-png-и-svg)
* Создание слайда из текстового описания | [Пример](#создание-слайда-из-текстового-описания)
* Печать различной информации о слайде | [Пример](#печать-различной-информации-о-слайде)
* Печать различной информации о библиотеке слайдов | [Пример](#печать-различной-информации-о-библиотеке-слайдов)
* Рисование слайда на [Cairo](https://www.cairographics.org/) поверхности | [Пример](#рисование-слайда-на-cairo-поверхности)
* Рисование слайда на [Cairo](https://www.cairographics.org/) поверхности через [GObject Introspection](https://gi.readthedocs.io/en/latest/) | [Пример](#рисование-слайда-на-cairo-поверхности-через-gobject-introspection)
* Используется для реализации функции `slide_image` в [VeLisp](https://github.com/ten0s/velisp)
* Windows, MacOS и Linux

## Установка

### Windows

Последнюю версию можно найти [здесь](https://github.com/ten0s/slide/releases).

#### Автоматическая установка

1. Скачайте https://github.com/ten0s/slide/releases/download/0.10.2/slide-0.10.2-win-x64-setup.zip
2. Откройте папку Загрузки
3. Распакуйте slide-0.10.2-win-x64-setup.zip
4. Запустите slide-0.10.2-win-x64-setup.exe
5. Следуйте указаниям установщика. Настройки по умолчанию подходят большинству пользователей

#### Установка из архива

##### Скачайте и распакуйте

1. Скачайте https://github.com/ten0s/slide/releases/download/0.10.2/slide-0.10.2-win-x64.zip
2. Откройте папку Загрузки
3. Выделите slide-0.10.2-win-x64.zip
4. Нажмите правую кнопку мышки
5. Выберите 'Распаковать все содержимое...'
6. Введите 'C:\'
7. Выберите 'Распаковать'

##### Добавьте папку в переменную окружения PATH

1. Нажмите 'Windows + Break' чтобы открыть Информацию о системе
2. Выберите 'Advanced system settings'
3. Выберите 'Environment Variables...'
4. Выберите 'Path' inside the User variables block
5. Выберите 'Edit...'
6. Выберите 'New...'
7. Введите 'C:\slide-0.10.2-win-x64\bin'
8. Выберите 'OK'
9. Выберите 'OK'


### MacOS

#### Homebrew

У вас должен быть установлен [Homebrew](https://brew.sh/).

```
% brew tap ten0s/velisp
% brew install slide
```

### Linux

```
$ make
$ make install [PREFIX=$PWD/install]
```

## Примеры

### Создание слайда из текстового описания

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

### Создание библиотеки слайдов из отдельных слайдов

#### Создание пустой библиотеки слайдов

```
$ slidelib-create mylib.slb
```

#### Создание библиотеки слайдов из одного слайда

```
$ slidelib-create mylib.slb square.sld
```

#### Создание библиотеки из нескольких слайдов

```
$ slidelib-create mylib.slb *.sld
```

### Печать различной информации о слайде

#### Печать информации о слайде в файле

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

#### Печать информации о слайде в библиотеке слайдов

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

#### Печать информации о слайде по его индексу в библиотеке слайдов

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

### Печать различной информации о библиотеке слайдов

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

### Импорт слайдов в библиотеку слайдов

#### Импорт одного слайда в библиотеку слайдов

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

#### Импорт нескольких слайдов в библиотеку слайдов

```
$ slidelib-import mylib.slb *.sld
```

### Экспорт слайдов из библиотеки слайдов

#### Экспорт одного слайда из библиотеки слайдов

```
$ slidelib-export mylib.slb square
```

#### Экспорт всех слайдов из библиотеки слайдов

```
$ slidelib-export --all mylib.slb
```

### Удаление слайдов из библиотеки слайдов

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

### Конвертация слайда в PNG и SVG

```
$ slide-convert --to=png square.sld
```

```
$ slide-convert --to=svg mylib.slb square
```

### Рисование слайда на [Cairo](https://www.cairographics.org/) поверхности

```
$ slide-view square.sld
```

```
$ slide-view mylib.slb square
```

```
$ slile-view mylib.slb 0
```

### Рисование слайда на [Cairo](https://www.cairographics.org/) поверхности через [GObject Introspection](https://gi.readthedocs.io/en/latest/)

```
$ export LD_LIBRARY_PATH=$PWD/install/lib GI_TYPELIB_PATH=$PWD/install/lib/girepository-1.0

$ python3 -m venv env
$ source env/bin/activate
$ pip3 install -r test/requirements.txt

$ python3 test/gtk-cairo-test.py
```

## Лицензия

Проект выпущен под лицензией GNU General Public License v3.0 или более поздней версии.
Полную информацию о лицензии можно найти в [LICENSE](LICENSE) или
https://spdx.org/licenses/GPL-3.0-or-later.html.
