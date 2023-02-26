include ../Makefile.inc

all: slide slide-view

%.o: %.cpp %.hpp
	$(CXX) -c $< -fPIC $(CPPFLAGS) $(CXXFLAGS) -o $@

slide: slide.cpp ../lib/libslide.$(LIB_EXT)
	$(CXX) $< $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIB_SLIDE) $(LIB_BOOST_PROGRAM_OPTIONS) -o $@

slide-view: slide-view.cpp ../lib/libslide.$(LIB_EXT)
	$(CXX) $< $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIB_SLIDE) $(DLL_CAIRO) $(DLL_GTK3) -o $@

.PHONY: install
install:
	mkdir -p $(PREFIX)/$(BIN_DIR)
	cp slide slide-view $(PREFIX)/$(BIN_DIR)/

.PHONY: clean
clean:
	rm -f *.o slide slide-view
	rm -f *.exe
	rm -rf *.dSYM
