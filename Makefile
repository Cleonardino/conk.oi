# Compiler and flags
COMPILER = g++
FLAGS = -std=c++17 -Wall -Wextra -O2

# Target and sources
TARGET = parser
SRC = model/parser.cpp
OBJS = $(SRC:.cpp=.o)

# Default target
game: model/map.o model/parser.o model/game.o
	$(COMPILER) -o game model/*.o $(FLAGS)

%.o: %.cpp
	$(COMPILER) -o $@ -c $< $(FLAGS)

clean:
	rm -f model/*.o