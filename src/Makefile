include ../Makefile.inc

UTILS = \
	slide-convert   \
	slide-info      \
	slide-view      \
	slidelib-create \
	slidelib-delete \
	slidelib-export \
	slidelib-import

all: $(UTILS)

%.o: %.cpp %.hpp
	$(CXX) -c $< -fPIC $(CPPFLAGS) $(CXXFLAGS) -o $@

slide-convert: slide-convert.cpp ../lib/libslide.$(LIB_EXT)
	$(CXX) $< $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIB_SLIDE) $(LIB_BOOST_PROGRAM_OPTIONS) $(DLL_CAIRO) -o $@

slide-info: slide-info.cpp ../lib/libslide.$(LIB_EXT)
	$(CXX) $< $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIB_SLIDE) $(LIB_BOOST_PROGRAM_OPTIONS) -o $@

slide-view: slide-view.cpp ../lib/libslide.$(LIB_EXT)
	$(CXX) $< $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIB_SLIDE) $(LIB_BOOST_PROGRAM_OPTIONS) $(DLL_CAIRO) $(DLL_GTK3) -o $@

slidelib-create: slidelib-create.cpp ../lib/libslide.$(LIB_EXT)
	$(CXX) $< $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIB_SLIDE) $(LIB_BOOST_PROGRAM_OPTIONS) -o $@

slidelib-delete: slidelib-delete.cpp ../lib/libslide.$(LIB_EXT)
	$(CXX) $< $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIB_SLIDE) $(LIB_BOOST_PROGRAM_OPTIONS) -o $@

slidelib-export: slidelib-export.cpp ../lib/libslide.$(LIB_EXT)
	$(CXX) $< $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIB_SLIDE) $(LIB_BOOST_PROGRAM_OPTIONS) -o $@

slidelib-import: slidelib-import.cpp ../lib/libslide.$(LIB_EXT)
	$(CXX) $< $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIB_SLIDE) $(LIB_BOOST_PROGRAM_OPTIONS) -o $@

.PHONY: install
install:
	mkdir -p $(PREFIX)/$(BIN_DIR)
	cp $(UTILS) $(PREFIX)/$(BIN_DIR)/

.PHONY: clean
clean:
	rm -f *.o $(UTILS)
	rm -f *.exe
	rm -rf *.dSYM
