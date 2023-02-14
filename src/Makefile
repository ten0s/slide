include ../Makefile.inc

all: main cairo

%.o: %.cpp %.hpp
	$(CXX) -c $< -fPIC $(CPPFLAGS) $(CXXFLAGS) -o $@

main: main.cpp ../lib/libslide.$(LIB_EXT)
	$(CXX) $< $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIBSLIDE) $(LD_RPATH) -o $@

cairo: cairo.cpp ../lib/libslide.$(LIB_EXT)
	$(CXX) $< $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIBSLIDE) $(LIBCAIRO) $(LIBGTK3) $(LD_RPATH) -o $@

.PHONY: install
install:
	mkdir -p $(PREFIX)/$(BIN_DIR)
	cp cairo main $(PREFIX)/$(BIN_DIR)

.PHONY: clean
clean:
	rm -f *.o cairo main
	rm -f *.exe
	rm -rf *.dSYM
