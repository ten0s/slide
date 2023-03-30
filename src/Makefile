include ../Makefile.inc

SOURCES := $(wildcard *.cpp)
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))
DEPENDS := $(patsubst %.cpp,%.d,$(SOURCES))

UTILS = \
	slide-convert   \
	slide-create    \
	slide-info      \
	slide-view      \
	slidelib-create \
	slidelib-delete \
	slidelib-export \
	slidelib-import

all: $(UTILS)

-include $(DEPENDS)

%.o: %.cpp
	$(CXX) -c $< -fPIC $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -o $@

slide-convert.o: slide-convert.cpp
	$(CXX) -c $< -fPIC $(CPPFLAGS) $(CXXFLAGS) $(INC_CAIRO) -MMD -MP -o $@

slide-view.o: slide-view.cpp
	$(CXX) -c $< -fPIC $(CPPFLAGS) $(CXXFLAGS) $(INC_CAIRO) $(INC_GTK3) -MMD -MP -o $@

slide-convert: slide-convert.o ../lib/libslide.$(LIB_EXT)
	$(CXX) $< $(LDFLAGS) $(LIB_SLIDE) $(LIB_BOOST_PROGRAM_OPTIONS) $(DLL_CAIRO) -o $@

slide-create: slide-create.o ../lib/libslide.$(LIB_EXT)
	$(CXX) $< $(LDFLAGS) $(LIB_SLIDE) $(LIB_BOOST_PROGRAM_OPTIONS) -o $@

slide-info: slide-info.o ../lib/libslide.$(LIB_EXT)
	$(CXX) $< $(LDFLAGS) $(LIB_SLIDE) $(LIB_BOOST_PROGRAM_OPTIONS) -o $@

slide-view: slide-view.o ../lib/libslide.$(LIB_EXT)
	$(CXX) $< $(LDFLAGS) $(LIB_SLIDE) $(LIB_BOOST_PROGRAM_OPTIONS) $(DLL_CAIRO) $(DLL_GTK3) -o $@

slidelib-create: slidelib-create.o ../lib/libslide.$(LIB_EXT)
	$(CXX) $< $(LDFLAGS) $(LIB_SLIDE) $(LIB_BOOST_PROGRAM_OPTIONS) -o $@

slidelib-delete: slidelib-delete.o ../lib/libslide.$(LIB_EXT)
	$(CXX) $< $(LDFLAGS) $(LIB_SLIDE) $(LIB_BOOST_PROGRAM_OPTIONS) -o $@

slidelib-export: slidelib-export.o ../lib/libslide.$(LIB_EXT)
	$(CXX) $< $(LDFLAGS) $(LIB_SLIDE) $(LIB_BOOST_PROGRAM_OPTIONS) -o $@

slidelib-import: slidelib-import.o ../lib/libslide.$(LIB_EXT)
	$(CXX) $< $(LDFLAGS) $(LIB_SLIDE) $(LIB_BOOST_PROGRAM_OPTIONS) -o $@

.PHONY: install
install:
	mkdir -p $(PREFIX)/$(BIN_DIR)
	cp $(UTILS) $(PREFIX)/$(BIN_DIR)/

.PHONY: clean
clean:
	rm -f *.d *.o $(UTILS)
	rm -f *.exe
	rm -rf *.dSYM
