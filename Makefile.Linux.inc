LIB_SLIDE=-Wl,-Bstatic $(DLL_SLIDE) -Wl,-Bdynamic
LIB_BOOST_PROGRAM_OPTIONS=-Wl,-Bstatic -lboost_program_options -Wl,-Bdynamic

DLL_DIR=lib
DLL_EXT=so
LIB_EXT=a
