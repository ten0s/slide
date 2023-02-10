DEBUG_CXXFLAGS=-g -O0 -Wall
DEBUG_LDFLAGS=

RELEASE_CXXFLAGS=-g -Wall -Werror
RELEASE_LDFLAGS=-static-libstdc++ -static-libgcc

CXXFLAGS=$(DEBUG_CXXFLAGS)
LDFLAGS=$(DEBUG_LDFLAGS)

LIBSLIDE=-L`pwd` -lslide -Wl,-rpath='$$ORIGIN'
LIBCAIRO=`pkg-config --cflags --libs cairo`
LIBGTK3=`pkg-config --cflags --libs gtk+-3.0`
LIBGLIB2=`pkg-config --cflags --libs glib-2.0`

SLIDE_FILES=\
	slide.cpp \
	slide.hpp \
	slide_cache.cpp \
	slide_cache.hpp \
	slide_parser.cpp \
	slide_parser.hpp \
	slide_header.cpp \
	slide_header.hpp \
	slide_header_parser.cpp \
	slide_header_parser.hpp \
	slide_colors.cpp \
	slide_colors.hpp \
	slide_draw.cpp \
	slide_draw.h \
	slide_library.cpp \
	slide_library.hpp \
	slide_library_parser.cpp \
	slide_library_parser.hpp \
	slide_library_header.cpp \
	slide_library_header.hpp \
	slide_library_header_parser.cpp \
	slide_library_header_parser.hpp \
	slide_library_directory.cpp \
	slide_library_directory.hpp \
	slide_library_directory_parser.cpp \
	slide_library_directory_parser.hpp \
	slide_parser_util.hpp \
	slide_record.hpp \
	slide_record_parser.cpp \
	slide_record_parser.hpp \
	slide_record_visitor_ostream.cpp \
	slide_record_visitor_ostream.hpp \
	slide_record_visitor_cairo.cpp \
	slide_record_visitor_cairo.hpp \
	slide_util.cpp \
	slide_util.hpp

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

libslide.so: $(SLIDE_FILES)
	g++ -shared -fPIC $^ $(CXXFLAGS) $(LDFLAGS) $(LIBCAIRO) -o $@

main: main.cpp libslide.so
	g++ $< $(CXXFLAGS) $(LDFLAGS) $(LIBSLIDE) -o $@

cairo: cairo.cpp libslide.so
	g++ $< $(CXXFLAGS) $(LDFLAGS) $(LIBSLIDE) $(LIBCAIRO) $(LIBGTK3) -o $@

$(LIB_FILE): gslide.c libslide.so
	g++ -shared -fPIC $< $(CXXFLAGS) $(LDFLAGS) $(LIBSLIDE) $(LIBGLIB2) $(LIBCAIRO) -o $@

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
