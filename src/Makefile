include ../Makefile.inc

all: slide slide-view

%.o: %.cpp %.hpp
	$(CXX) -c $< -fPIC $(CPPFLAGS) $(CXXFLAGS) -o $@

slide: slide.cpp ../lib/libslide.$(DLL_EXT)
	$(CXX) $< $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIBSLIDE) $(LD_RPATH) -o $@

slide-view: slide-view.cpp ../lib/libslide.$(DLL_EXT)
	$(CXX) $< $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIBSLIDE) $(LIBCAIRO) $(LIBGTK3) $(LD_RPATH) -o $@

.PHONY: install
install:
	mkdir -p $(PREFIX)/$(BIN_DIR)
	cp slide slide-view $(PREFIX)/$(BIN_DIR)/

.PHONY: clean
clean:
	rm -f *.o slide slide-view
	rm -f *.exe
	rm -rf *.dSYM
