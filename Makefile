DEBUG_CPPFLAGS=
DEBUG_CXXFLAGS=-g -O0 -Wall
DEBUG_LDFLAGS=

RELEASE_CPPFLAGS=
RELEASE_CXXFLAGS=-g -Wall -Werror
RELEASE_LDFLAGS=-static-libstdc++ -static-libgcc

CPPFLAGS=$(DEBUG_CPPFLAGS)
CXXFLAGS=$(DEBUG_CXXFLAGS)
LDFLAGS=$(DEBUG_LDFLAGS)

LIBSLIDE=-L`pwd` -lslide -Wl,-rpath='$$ORIGIN'
LIBCAIRO=`pkg-config --cflags --libs cairo`
LIBGTK3=`pkg-config --cflags --libs gtk+-3.0`
LIBGLIB2=`pkg-config --cflags --libs glib-2.0`

LIBSLIDE_OBJS=\
	slide.o \
	slide_cache.o \
	slide_parser.o \
	slide_header.o \
	slide_header_parser.o \
	slide_colors.o \
	slide_draw.o \
	slide_library.o \
	slide_library_parser.o \
	slide_library_header.o \
	slide_library_header_parser.o \
	slide_library_directory.o \
	slide_library_directory_parser.o \
	slide_loader.o \
	slide_record_parser.o \
	slide_record_visitor_ostream.o \
	slide_record_visitor_cairo.o \
	slide_util.o

NAMESPACE=Slide
NSVERSION=1.0
LIB_NAME=gslide
SYM_PREFIX=gslide
LIB_FILE=lib$(LIB_NAME).so
GIR_FILE=$(NAMESPACE)-$(NSVERSION).gir
TYPELIB_FILE=$(NAMESPACE)-$(NSVERSION).typelib
LIB_DIR=lib
SHARE_DIR=share
GIR_DIR=$(SHARE_DIR)/gir-1.0
TYPELIB_DIR=$(LIB_DIR)/girepository-1.0
PREFIX ?= `pwd`

all: libslide.so main cairo $(TYPELIB_FILE)

%.o: %.cpp %.hpp
	g++ -c $< -fPIC $(CPPFLAGS) $(CXXFLAGS) -o $@

slide_draw.o: slide_draw.cpp slide_draw.h
	g++ -c $< -fPIC $(CPPFLAGS) $(CXXFLAGS) $(LIBCAIRO) -o $@

slide_record_visitor_cairo.o: slide_record_visitor_cairo.cpp slide_record_visitor_cairo.hpp
	g++ -c $< -fPIC $(CPPFLAGS) $(CXXFLAGS) $(LIBCAIRO) -o $@

libslide.so: $(LIBSLIDE_OBJS)
	g++ -shared $^ -fPIC $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIBCAIRO) -o $@

main: main.cpp libslide.so
	g++ $< $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIBSLIDE) -o $@

cairo: cairo.cpp libslide.so
	g++ $< $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIBSLIDE) $(LIBCAIRO) $(LIBGTK3) -o $@

$(LIB_FILE): gslide.c libslide.so
	g++ -shared $< -fPIC $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIBSLIDE) $(LIBGLIB2) $(LIBCAIRO) -o $@

$(GIR_FILE): $(LIB_FILE)
	g-ir-scanner gslide.[ch]             \
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
