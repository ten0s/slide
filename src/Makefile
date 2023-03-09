include ../Makefile.inc

all: slide-info slide-lib slide-view slide-convert

%.o: %.cpp %.hpp
	$(CXX) -c $< -fPIC $(CPPFLAGS) $(CXXFLAGS) -o $@

slide-info: slide-info.cpp ../lib/libslide.$(LIB_EXT)
	$(CXX) $< $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIB_SLIDE) $(LIB_BOOST_PROGRAM_OPTIONS) -o $@

slide-lib: slide-lib.cpp ../lib/libslide.$(LIB_EXT)
	$(CXX) $< $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIB_SLIDE) $(LIB_BOOST_PROGRAM_OPTIONS) -o $@

slide-view: slide-view.cpp ../lib/libslide.$(LIB_EXT)
	$(CXX) $< $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIB_SLIDE) $(LIB_BOOST_PROGRAM_OPTIONS) $(DLL_CAIRO) $(DLL_GTK3) -o $@

slide-convert: slide-convert.cpp ../lib/libslide.$(LIB_EXT)
	$(CXX) $< $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIB_SLIDE) $(LIB_BOOST_PROGRAM_OPTIONS) $(DLL_CAIRO) -o $@

.PHONY: install
install:
	mkdir -p $(PREFIX)/$(BIN_DIR)
	cp slide-info    $(PREFIX)/$(BIN_DIR)/
	cp slide-lib     $(PREFIX)/$(BIN_DIR)/
	cp slide-view    $(PREFIX)/$(BIN_DIR)/
	cp slide-convert $(PREFIX)/$(BIN_DIR)/

.PHONY: clean
clean:
	rm -f *.o slide-info slide-lib slide-view slide-convert
	rm -f *.exe
	rm -rf *.dSYM
