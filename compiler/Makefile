CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -pedantic

SRC = main.cpp lexer.cpp parser.cpp codegen.cpp util.cpp
OUT = ../test/smallc

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT) *.o
