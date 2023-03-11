include ../Makefile.inc

SLIDE_OBJS=\
	slide.o \
	slide_binary_parser.o \
	slide_binary_writer.o \
	slide_header_binary_parser.o \
	slide_header_binary_writer.o \
	slide_library.o \
	slide_library_binary_parser.o \
	slide_library_binary_writer.o \
	slide_library_header_binary_parser.o \
	slide_library_directory_binary_parser.o \
	slide_info_printers.o \
	slide_library_info_printers.o \
	slide_record_binary_parser.o \
	slide_records_visitor_binary_writer.o \
	slide_records_visitor_text_writer.o \
	slide_records_visitor_cairo_drawer.o \
	slide_cache.o \
	slide_loader.o \
	slide_colors.o \
	slide_draw.o \
	slide_util.o

all: libslide.$(DLL_EXT) libslide.$(LIB_EXT)

%.o: %.cpp %.hpp
	$(CXX) -c $< -fPIC $(CPPFLAGS) $(CXXFLAGS) -o $@

slide_draw.o: slide_draw.cpp slide_draw.h
	$(CXX) -c $< -fPIC $(CPPFLAGS) $(CXXFLAGS) $(DLL_CAIRO) -o $@

slide_records_visitor_cairo_drawer.o: slide_records_visitor_cairo_drawer.cpp slide_records_visitor_cairo_drawer.hpp
	$(CXX) -c $< -fPIC $(CPPFLAGS) $(CXXFLAGS) $(DLL_CAIRO) -o $@

libslide.so: $(SLIDE_OBJS)
	$(CXX) -shared $^ -fPIC $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(DLL_CAIRO) -o $@

libslide.dll: $(SLIDE_OBJS)
	$(CXX) -shared $^ -fPIC $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(DLL_CAIRO) -Wl,--out-implib,$@.a -o $@

libslide.dylib: $(SLIDE_OBJS)
	$(CXX) -shared $^ -fPIC $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(DLL_CAIRO) -o $@

libslide.a: $(SLIDE_OBJS)
	ar rcv $@ $^

.PHONY: install
install:
	mkdir -p $(PREFIX)/$(DLL_DIR)
	cp libslide.$(DLL_EXT) $(PREFIX)/$(DLL_DIR)/

.PHONY: clean
clean:
	rm -f *.o *.so
	rm -f *.dll
	rm -f *.dylib
	rm -r *.a
	rm -rf *.dSYM
