include ../Makefile.inc

all: slide-info slide-view

%.o: %.cpp %.hpp
	$(CXX) -c $< -fPIC $(CPPFLAGS) $(CXXFLAGS) -o $@

slide-info: slide-info.cpp ../lib/libslide.$(LIB_EXT)
	$(CXX) $< $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIB_SLIDE) $(LIB_BOOST_PROGRAM_OPTIONS) -o $@

slide-view: slide-view.cpp ../lib/libslide.$(LIB_EXT)
	$(CXX) $< $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIB_SLIDE) $(DLL_CAIRO) $(DLL_GTK3) -o $@

.PHONY: install
install:
	mkdir -p $(PREFIX)/$(BIN_DIR)
	cp slide-info slide-view $(PREFIX)/$(BIN_DIR)/

.PHONY: clean
clean:
	rm -f *.o slide-info slide-view
	rm -f *.exe
	rm -rf *.dSYM
