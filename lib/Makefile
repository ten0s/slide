include ../Makefile.inc

SOURCES := $(wildcard *.cpp)
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))
DEPENDS := $(patsubst %.cpp,%.d,$(SOURCES))

all: libslide.$(DLL_EXT) libslide.$(LIB_EXT)

-include $(DEPENDS)

%.o: %.cpp
	$(CXX) -c $< -fPIC $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -o $@

slide_draw_cairo.o: slide_draw_cairo.cpp
	$(CXX) -c $< -fPIC $(CPPFLAGS) $(CXXFLAGS) $(INC_CAIRO) -MMD -MP -o $@

slide_records_visitor_cairo_drawer.o: slide_records_visitor_cairo_drawer.cpp
	$(CXX) -c $< -fPIC $(CPPFLAGS) $(CXXFLAGS) $(INC_CAIRO) -MMD -MP -o $@

libslide.so: $(OBJECTS)
	$(CXX) -shared $^ $(LDFLAGS) $(DLL_CAIRO) -o $@

libslide.dll: $(OBJECTS)
	$(CXX) -shared $^ $(LDFLAGS) $(DLL_CAIRO) -Wl,--out-implib,$@.a -o $@

libslide.dylib: $(OBJECTS)
	$(CXX) -shared $^ $(LDFLAGS) $(DLL_CAIRO) -o $@

libslide.a: $(OBJECTS)
	ar rcv $@ $^

.PHONY: install
install:
	mkdir -p $(PREFIX)/$(DLL_DIR)
	cp libslide.$(DLL_EXT) $(PREFIX)/$(DLL_DIR)/

.PHONY: clean
clean:
	rm -f *.a *.d *.o
	rm -f *.so
	rm -f *.dll
	rm -f *.dylib
	rm -rf *.dSYM
