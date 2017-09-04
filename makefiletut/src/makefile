# test makefile that compiles the helloworld file

# explanation: 
# target: sources ... target that depends on the sources
# next line needs to be indented
# explains how target is to be made
# in this case, use compiler with flags, $@ equivalent to target. i.e. %.o
# $< equivalent to first item in dependency list. i.e. %.cpp
# $^ eqivalent to dependency list

# following are some macros
IDIR = ../include
CC = g++							# c++ compiler
CFLAGS = -std=c++11 -I.				# compiler flags: c++11 standard, include files in same folder as makefile

ODIR = ../obj
LDIR = ../lib

LIBS = -lm

_DEPS = hellomake.hpp						# all headers
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))		# substitues all headers in _DEPS with ../include/filename

_OBJ = helloworld.o hellofunc.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)					# explicit rule to make object files
	$(CC) -c -o $@ $<  $(CFLAGS)

helloworldapp: $(OBJ) 				# explicit rule to compile application
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean: 
	rm -f $(ODIR)/* helloworldapp
