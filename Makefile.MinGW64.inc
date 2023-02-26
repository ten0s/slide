LIB_SLIDE=-Wl,-Bstatic $(DLL_SLIDE) -Wl,-Bdynamic
LIB_BOOST_PROGRAM_OPTIONS=-Wl,-Bstatic -lboost_program_options-mt -Wl,-Bdynamic

DLL_DIR=bin
DLL_EXT=dll
LIB_EXT=a
