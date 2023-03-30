CXX=c++
CC=cc

DEBUG_CPPFLAGS=
DEBUG_CXXFLAGS=-g -O0 -Wall -std=c++17
DEBUG_CFLAGS  =-g -O0 -Wall
DEBUG_LDFLAGS =

RELEASE_CPPFLAGS=
RELEASE_CXXFLAGS=-g -Wall -Werror -std=c++17
RELEASE_CFLAGS  =-g -Wall -Werror
RELEASE_LDFLAGS =

CPPFLAGS=$(DEBUG_CPPFLAGS)
CXXFLAGS=$(DEBUG_CXXFLAGS)
CFLAGS  =$(DEBUG_CFLAGS)
LDFLAGS =$(DEBUG_LDFLAGS)

BIN_DIR=bin
LIB_DIR=lib
SHARE_DIR=share

GIR_DIR=$(SHARE_DIR)/gir-1.0
TYPELIB_DIR=$(LIB_DIR)/girepository-1.0

# Use Windows notation for dynamic vs static linking
# DLL_ is for dynamic linking (.dll, .dynlib, .so)
# LIB_ is for static linking (.a, .lib)

# Dynamic libs
INC_CAIRO=`pkg-config --cflags cairo`
DLL_CAIRO=`pkg-config --libs cairo`
INC_GLIB2=`pkg-config --cflags glib-2.0`
DLL_GLIB2=`pkg-config --libs glib-2.0`
INC_GTK3=`pkg-config --cflags gtk+-3.0`
DLL_GTK3=`pkg-config --libs gtk+-3.0`
DLL_SLIDE=-L../lib -lslide

# Static libs
LIB_SLIDE= *** Setup Below ***
LIB_BOOST_PROGRAM_OPTIONS= *** Setup Below ***

# Where to put dynamic libraries
DLL_DIR= *** Setup Below ***
# Dynamic library extension
DLL_EXT= *** Setup Below ***
# Static library extension
LIB_EXT= *** Setup Below ***

SYSTEM := $(shell uname -s)
ifeq ($(SYSTEM), Linux)
include ../Makefile.Linux.inc
endif

ifeq ($(findstring MINGW64, $(SYSTEM)), MINGW64)
include ../Makefile.MinGW64.inc
endif

ifeq ($(SYSTEM), Darwin)
include ../Makefile.MacOS.inc
endif
