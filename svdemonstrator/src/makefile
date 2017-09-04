IDIR = ../include
# c++ compiler
CC = g++						
# compiler flags: c++11 standard, include files in same folder as makefile
CFLAGS = -std=c++11 -Wall -Wextra -pedantic -I/home/wolfgang/libs/opencvlinux/include -L$(LDIR)

ODIR = ../obj

LDIR = /home/wolfgang/libs/opencvlinux/lib
LIBS = -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_calib3d -lopencv_highgui -lopencv_videoio -lopencv_features2d
_DEPS = 									# all headers
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))		# substitues all headers in _DEPS with ../include/filename

_OBJ = calibration.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)					# explicit rule to make object files
	$(CC) -c -o $@ $<  $(CFLAGS)

calibrationapp: $(OBJ) 				# explicit rule to compile application
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean: 
	rm -f $(ODIR)/* calibrationapp
