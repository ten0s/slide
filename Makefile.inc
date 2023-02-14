CXX=c++
CC=cc

DEBUG_CPPFLAGS=
DEBUG_CXXFLAGS=-g -O0 -Wall -std=c++17
DEBUG_CFLAGS=-g -O0 -Wall
DEBUG_LDFLAGS=

RELEASE_CPPFLAGS=
RELEASE_CXXFLAGS=-g -Wall -Werror -std=c++17
RELEASE_CFLAGS=-g -Wall -Werror
RELEASE_LDFLAGS=-static-libstdc++ -static-libgcc

CPPFLAGS=$(DEBUG_CPPFLAGS)
CXXFLAGS=$(DEBUG_CXXFLAGS)
CFLAGS=$(DEBUG_CFLAGS)
LDFLAGS=$(DEBUG_LDFLAGS)

LIBCAIRO=`pkg-config --cflags --libs cairo`
LIBGLIB2=`pkg-config --cflags --libs glib-2.0`
LIBGTK3=`pkg-config --cflags --libs gtk+-3.0`
LIBSLIDE=-L`pwd`/../lib -lslide

BIN_DIR=bin
LIB_DIR=lib
SHARE_DIR=share

LIB_EXT := ${shell case `uname -s` in MINGW64*) echo "dll";; Linux) echo "so";; Darwin) echo "dylib";; *) echo "Unknown";; esac}
LD_RPATH := ${shell case `uname -s` in MINGW64* | Linux) echo "-Wl,-rpath='\$$ORIGIN'";; Darwin) echo "";; *) echo "Unknown";; esac}