include ../Makefile.inc

NAMESPACE=SlideLib
NSVERSION=1.0

GIR_FILE=$(NAMESPACE)-$(NSVERSION).gir
TYPELIB_FILE=$(NAMESPACE)-$(NSVERSION).typelib

all: $(TYPELIB_FILE)

libgislide.so: gi_slide.c ../lib/libslide.so
	$(CC) -shared $< -fPIC $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(DLL_SLIDE) $(DLL_GLIB2) $(DLL_CAIRO) -o $@

libgislide.dll: gi_slide.c ../lib/libslide.dll
	$(CC) -shared $< -fPIC $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(DLL_SLIDE) $(DLL_GLIB2) $(DLL_CAIRO) -Wl,--out-implib,$@.a -o $@

libgislide.dylib: gi_slide.c ../lib/libslide.dylib
	$(CC) -shared $< -fPIC $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(DLL_SLIDE) $(DLL_GLIB2) $(DLL_CAIRO) -o $@

$(GIR_FILE): libgislide.$(DLL_EXT)
	g-ir-scanner gi_slide.[ch]           \
		--warn-all                       \
		--library-path=$(CURDIR)         \
		--library-path=$(CURDIR)/../lib  \
		--library=gislide                \
		--symbol-prefix=gi_slide         \
		--namespace=$(NAMESPACE)         \
		--nsversion=$(NSVERSION)         \
		--identifier-prefix=$(NAMESPACE) \
		--include=GLib-2.0               \
		--output=$@

$(TYPELIB_FILE): $(GIR_FILE)
	g-ir-compiler $^ --output=$@

.PHONY: install
install:
	mkdir -p $(PREFIX)/$(DLL_DIR)
	mkdir -p $(PREFIX)/$(GIR_DIR)
	mkdir -p $(PREFIX)/$(TYPELIB_DIR)
	cp libgislide.$(DLL_EXT) $(PREFIX)/$(DLL_DIR)/
	cp $(GIR_FILE) $(PREFIX)/$(GIR_DIR)/
	cp $(TYPELIB_FILE) $(PREFIX)/$(TYPELIB_DIR)/

.PHONY: clean
clean:
	rm -f *.o *.so
	rm -f *.dll *.dll.a
	rm -f *.dylib
	rm -rf *.dSYM
	rm -f *.gir *.typelib
	rm -rf tmp-introspect*
