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

GIR_DIR=$(SHARE_DIR)/gir-1.0
TYPELIB_DIR=$(LIB_DIR)/girepository-1.0

# Where to put dynamic libraries
DLL_DIR := ${shell case `uname -s` in MINGW64*) echo "bin";; Linux | Darwin) echo "lib";; *) echo "Unknown-DLL_DIR";; esac}
# Dynamic library extension
DLL_EXT := ${shell case `uname -s` in MINGW64*) echo "dll";; Linux) echo "so";; Darwin) echo "dylib";; *) echo "Unknown-DLL_EXT";; esac}
# Static library extension
LIB_EXT  := ${shell case `uname -s` in MINGW64*) echo "a";; Linux) echo "a";; Darwin) echo "a";; *) echo "Unknown-LIB_EXT";; esac}
