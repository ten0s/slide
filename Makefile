CXX=c++

DEBUG_CPPFLAGS=
DEBUG_CXXFLAGS=-g -O0 -Wall -std=c++17
DEBUG_LDFLAGS=

RELEASE_CPPFLAGS=
RELEASE_CXXFLAGS=-g -Wall -Werror -std=c++17
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

LIB_EXT := ${shell case `uname -s` in MINGW64*) echo "dll";; Linux) echo "so";; Darwin) "dylib";; *) echo "Unknown";; esac}

NAMESPACE=Slide
NSVERSION=1.0
LIB_NAME=gslide
GIR_FILE=$(NAMESPACE)-$(NSVERSION).gir
TYPELIB_FILE=$(NAMESPACE)-$(NSVERSION).typelib
LIB_DIR=lib
SHARE_DIR=share
GIR_DIR=$(SHARE_DIR)/gir-1.0
TYPELIB_DIR=$(LIB_DIR)/girepository-1.0
PREFIX ?= `pwd`

all: libslide.$(LIB_EXT) main cairo $(TYPELIB_FILE)

%.o: %.cpp %.hpp
	$(CXX) -c $< -fPIC $(CPPFLAGS) $(CXXFLAGS) -o $@

slide_draw.o: slide_draw.cpp slide_draw.h
	$(CXX) -c $< -fPIC $(CPPFLAGS) $(CXXFLAGS) $(LIBCAIRO) -o $@

slide_record_visitor_cairo.o: slide_record_visitor_cairo.cpp slide_record_visitor_cairo.hpp
	$(CXX) -c $< -fPIC $(CPPFLAGS) $(CXXFLAGS) $(LIBCAIRO) -o $@

libslide.so: $(LIBSLIDE_OBJS)
	$(CXX) -shared $^ -fPIC $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIBCAIRO) -o $@

libslide.dll: $(LIBSLIDE_OBJS)
	$(CXX) -shared $^ -fPIC $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIBCAIRO) -Wl,--out-implib,$@.a -o $@

main: main.cpp libslide.$(LIB_EXT)
	$(CXX) $< $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIBSLIDE) -o $@

cairo: cairo.cpp libslide.$(LIB_EXT)
	$(CXX) $< $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIBSLIDE) $(LIBCAIRO) $(LIBGTK3) -o $@

libgslide.so: gslide.c libslide.so
	$(CXX) -shared $< -fPIC $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIBSLIDE) $(LIBGLIB2) $(LIBCAIRO) -o $@

libgslide.dll: gslide.c libslide.dll
	$(CXX) -shared $< -fPIC $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIBSLIDE) $(LIBGLIB2) $(LIBCAIRO) -Wl,--out-implib,$@.a -o $@

$(GIR_FILE): libgslide.$(LIB_EXT)
	g-ir-scanner gslide.[ch]             \
		--warn-all                       \
		--library-path=`pwd`             \
		--library=$(LIB_NAME)            \
		--namespace=$(NAMESPACE)         \
		--nsversion=$(NSVERSION)         \
		--symbol-prefix=$(LIB_NAME)      \
		--identifier-prefix=$(NAMESPACE) \
		--include=GLib-2.0               \
		--output=$@

$(TYPELIB_FILE): $(GIR_FILE)
	g-ir-compiler $^ --output=$@

install:
	mkdir -p $(PREFIX)/$(LIB_DIR)
	mkdir -p $(PREFIX)/$(GIR_DIR)
	mkdir -p $(PREFIX)/$(TYPELIB_DIR)
	mv libgslide.* $(PREFIX)/$(LIB_DIR)
	mv $(GIR_FILE) $(PREFIX)/$(GIR_DIR)
	mv $(TYPELIB_FILE) $(PREFIX)/$(TYPELIB_DIR)

clean:
	rm -f *.dll, *.dll.a, *.exe *.o *.so *.gir *.typelib
	rm -rf $(PREFIX)/$(LIB_DIR) $(PREFIX)/$(SHARE_DIR)
	rm -rf tmp-introspect*
