# ---------------------------------------------------------------
# SWIG Python Makefile
#
# This file can be used to build various Python extensions with SWIG.
# By default this file is set up for dynamic loading, but it can
# be easily customized for static extensions by modifying various
# portions of the file.
#
#        SRCS       = C source files
#        CXXSRCS    = C++ source files
#        OBJCSRCS   = Objective-C source files
#        OBJS       = Additional .o files (compiled previously)
#        INTERFACE  = SWIG interface file
#        TARGET     = Name of target module or executable
#
# Many portions of this file were created by the SWIG configure
# script and should already reflect your machine.
#----------------------------------------------------------------

SRCS          =
CXXSRCS       = bloom_filter.cpp ams_sketch.cpp skim_sketch.cpp sketch.cpp cold_filter.cpp hash.cpp hashtable.cpp MurmurHash3.cpp CityHash.cpp
OBJS          =
INTERFACE     = bloom_filter.i
WRAPFILE      = $(INTERFACE:.i=_wrap.cpp)
WRAPOBJ       = $(INTERFACE:.i=_wrap.o)
TARGET        = _bloom_filter.so

prefix        = /usr/local
exec_prefix   = ${prefix}

CC            = gcc
CXX           = g++
CFLAGS        =
CXXFLAGS	  = -std=c++11
INCLUDES      =
LIBS          =

# SWIG Options
#     SWIG      = location of the SWIG executable
#     SWIGOPT   = SWIG compiler options
#     SWIGCC    = Compiler used to compile the wrapper file

SWIG          = $(exec_prefix)/bin/swig
SWIGOPT       = -c++ -python
SWIGCC        = $(CXX)

# SWIG Library files.  Uncomment if rebuilding the Python interpreter
#SWIGLIBS      = -lembed.i

# Rules for creating .o files from source.

COBJS         = $(SRCS:.c=.o)
CXXOBJS       = $(CXXSRCS:.cpp=.o)
OBJCOBJS      = $(OBJCSRCS:.m=.o)
ALLOBJS       = $(COBJS) $(CXXOBJS) $(OBJCOBJS) $(OBJS)

# Command that will be used to build the final extension.
BUILD         = $(SWIGCC)

# Uncomment the following if you are using dynamic loading
CCSHARED      = -fpic
BUILD         = g++ -lpython -dynamiclib

# Uncomment the following if you are using dynamic loading with C++ and
# need to provide additional link libraries (this is not always required).

#DLL_LIBS      = -L/usr/local/lib/gcc-lib/sparc-sun-solaris2.5.1/2.7.2 \
		 -L/usr/local/lib -lg++ -lstdc++ -lgcc

# Python installation

PY_INCLUDE    = -I/usr/include/python2.7 # -DHAVE_CONFIG_H /usr/include/python2.7

# Build options

BUILD_LIBS    = $(LIBS) # Dynamic loading

# Compilation rules for non-SWIG components

.SUFFIXES: .c .cxx .cpp .m

.c.o:
	$(CC) $(CCSHARED) $(CFLAGS) $(INCLUDES) -c $<

.cxx.o:
	$(CXX) $(CCSHARED) $(CXXFLAGS) $(INCLUDES) -c $<

.cpp.o:
	$(CXX) $(CCSHARED) $(CXXFLAGS) $(INCLUDES) -c $<

.m.o:
	$(OBJC) $(CCSHARED) $(CFLAGS) $(INCLUDES) -c $<


# ----------------------------------------------------------------------
# Rules for building the extension
# ----------------------------------------------------------------------

all: $(TARGET)

# Convert the wrapper file into an object file

$(WRAPOBJ) : $(WRAPFILE)
	$(SWIGCC) -c $(CCSHARED) $(CXXFLAGS) $(WRAPFILE) $(INCLUDES) $(PY_INCLUDE)

$(WRAPFILE) : $(INTERFACE)
	$(SWIG) $(SWIGOPT) -o $(WRAPFILE) $(SWIGLIBS) $(INTERFACE)

$(TARGET): $(WRAPOBJ) $(ALLOBJS)
	$(BUILD) $(WRAPOBJ) $(ALLOBJS) $(BUILD_LIBS) -o $(TARGET)

clean:
	rm -f $(COBJS) $(CXXOBJS) $(OBJCOBJS) $(WRAPOBJ) $(WRAPFILE) $(TARGET)