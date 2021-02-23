# Makefile for main

# *****************************************************
# Variables to control Makefile operation

CXX = g++
CXXFLAGS = -Wall -g -std=c++11

VPATH = source

OUT = bin/

# OBJECTS = $(OUT)main.o $(OUT)getInputParameters.o $(OUT)output.o \
#           $(OUT)readData.o $(OUT)getTimeStamp.o $(OUT)filterBad.o \
# 		  $(OUT)filterPiledup.o $(OUT)getPHDPID.o $(OUT)tailVStotal.o

OBJECTS = $(OUT)main.o $(OUT)getInputParameters.o $(OUT)output.o \
          $(OUT)readData.o $(OUT)coincidence.o $(OUT)getTimeStamp.o \
		  $(OUT)rejection.o $(OUT)getPlot.o



LIBS1 = `root-config --cflags --glibs`
LIBS2 = `root-config --cflags --glibs`

# ****************************************************
# Targets needed to bring the executable up to date

$(OUT)main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LIBS1)

	# The main.o target

$(OUT)main.o: main.cc getInputParameters.h events.h \
              output.h readData.h getTimeStamp.h \
			  coincidence.h rejection.h getPlot.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS1)

	# The getInputParameters.o target

$(OUT)getInputParameters.o: getInputParameters.cc getInputParameters.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS2)

	# The output.o target

$(OUT)output.o: output.cc output.h getInputParameters.h events.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS2)

	# The readData.o target

$(OUT)readData.o: readData.cc readData.h getInputParameters.h events.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS2)

	# The coincidence.o target

$(OUT)coincidence.o: coincidence.cc coincidence.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS2)


	# The getTimeStamp.o target

$(OUT)getTimeStamp.o: getTimeStamp.cc getTimeStamp.h getInputParameters.h events.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS2)

	# The rejection.o target

$(OUT)rejection.o: rejection.cc rejection.h getInputParameters.h events.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS2)

	# The getPlot.o target

$(OUT)getPlot.o: getPlot.cc getPlot.h getInputParameters.h events.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS2)

.PHONY: clean

clean :
	rm $(OUT)*.o