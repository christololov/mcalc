# Flags to be given to the compiler.
# -Wall and -Werror is not used because an error arises from the fractions library.
CXXFLAGS = -g -pedantic #-Wall -Werror

# Define any directories containing header files other than /usr/include.
INCLUDES = -I./fraclib/

# Path to Fractions.cpp
SRC_PATH = ./fraclib

# Defines the C++ source files.
SRCS = main.cpp matrix_list.cpp matrix.cpp operations.cpp parser.cpp buttons.cpp ${SRC_PATH}/Fraction.cpp

STD = -std=c++11

.PHONY: all clean

all: main

main: $(SRCS)
	$(CXX) $(STD) $(CXXFLAGS) `fltk-config --cxxflags` $(SRCS) `fltk-config --ldflags` $(INCLUDES) -o $@

clean:
	-rm main
