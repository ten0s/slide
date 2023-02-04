CFLAGS=-Wall -g `pkg-config --cflags glib-2.0 cairo`
LIBS=`pkg-config --libs glib-2.0 cairo gtk+-3.0`

NAMESPACE=Slide
NSVERSION=1.0
LIB_NAME=gislide
SYM_PREFIX=gi_slide
LIB_FILE=lib$(LIB_NAME).so
GIR_FILE=$(NAMESPACE)-$(NSVERSION).gir
TYPELIB_FILE=$(NAMESPACE)-$(NSVERSION).typelib
LIB_DIR=lib
SHARE_DIR=share
GIR_DIR=$(SHARE_DIR)/gir-1.0
TYPELIB_DIR=$(LIB_DIR)/girepository-1.0
PREFIX ?= `pwd`

all: $(TYPELIB_FILE) main cairo

%.o: %.c %.cpp
	gcc -c $< $(CFLAGS) -o $@

gi-slide.o: gi-slide.h slide.o

slide.o: slide.h

main: main.cpp slide_file.hpp slide_file.cpp slide_visitor_ostream.hpp
	g++ -g main.cpp slide_file.cpp -o $@

cairo: cairo.cpp slide_file.hpp slide_file.cpp colors.hpp slide_record.hpp slide_visitor_cairo.hpp
	g++ -g cairo.cpp slide_file.cpp `pkg-config --cflags --libs cairo gtk+-3.0` -o $@

$(LIB_FILE): gi-slide.o slide.o
	gcc -shared $^ $(CFLAGS) $(LIBS) -o $@

$(GIR_FILE): $(LIB_FILE)
	g-ir-scanner gi-slide.[ch]           \
		--warn-all                       \
		--library-path=`pwd`             \
		--library=$(LIB_NAME)            \
		--namespace=$(NAMESPACE)         \
		--nsversion=$(NSVERSION)         \
		--symbol-prefix=$(SYM_PREFIX)    \
		--identifier-prefix=$(NAMESPACE) \
		--include=GLib-2.0               \
		--output=$@

$(TYPELIB_FILE): $(GIR_FILE)
	g-ir-compiler $^ --output=$@

install:
	mkdir -p $(PREFIX)/$(LIB_DIR)
	mkdir -p $(PREFIX)/$(GIR_DIR)
	mkdir -p $(PREFIX)/$(TYPELIB_DIR)
	mv $(LIB_FILE) $(PREFIX)/$(LIB_DIR)
	mv $(GIR_FILE) $(PREFIX)/$(GIR_DIR)
	mv $(TYPELIB_FILE) $(PREFIX)/$(TYPELIB_DIR)

clean:
	rm -f *.o *.so *.gir *.typelib
	rm -rf $(PREFIX)/$(LIB_DIR) $(PREFIX)/$(SHARE_DIR)
	rm -rf tmp-introspect*
