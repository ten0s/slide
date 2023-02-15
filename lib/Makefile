include ../Makefile.inc

LIBSLIDE_OBJS=\
	slide.o \
	slide_parser.o \
	slide_header.o \
	slide_header_parser.o \
	slide_library.o \
	slide_library_parser.o \
	slide_library_header.o \
	slide_library_header_parser.o \
	slide_library_directory.o \
	slide_library_directory_parser.o \
	slide_record_parser.o \
	slide_record_visitor_ostream.o \
	slide_record_visitor_cairo.o \
	slide_cache.o \
	slide_loader.o \
	slide_colors.o \
	slide_draw.o \
	slide_util.o

all: libslide.$(DLL_EXT)

%.o: %.cpp %.hpp
	$(CXX) -c $< -fPIC $(CPPFLAGS) $(CXXFLAGS) -o $@

slide_draw.o: slide_draw.cpp slide_draw.h
	$(CXX) -c $< -fPIC $(CPPFLAGS) $(CXXFLAGS) $(LIBCAIRO) -o $@

slide_record_visitor_cairo.o: slide_record_visitor_cairo.cpp slide_record_visitor_cairo.hpp
	echo $(LIBCAIRO)
	$(CXX) -c $< -fPIC $(CPPFLAGS) $(CXXFLAGS) $(LIBCAIRO) -o $@

libslide.so: $(LIBSLIDE_OBJS)
	$(CXX) -shared $^ -fPIC $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIBCAIRO) -o $@

libslide.dll: $(LIBSLIDE_OBJS)
	$(CXX) -shared $^ -fPIC $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIBCAIRO) -Wl,--out-implib,$@.a -o $@

libslide.dylib: $(LIBSLIDE_OBJS)
	$(CXX) -shared $^ -fPIC $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIBCAIRO) -o $@

.PHONY: install
install:
	mkdir -p $(PREFIX)/$(DLL_DIR)
	cp libslide.$(DLL_EXT) $(PREFIX)/$(DLL_DIR)/

.PHONY: clean
clean:
	rm -f *.o *.so
	rm -f *.dll *.dll.a
	rm -f *.dylib
	rm -rf *.dSYM
