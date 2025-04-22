# Compiler and flags
COMPILER = g++
FLAGS = -std=c++17 -Wall -Wextra -O2

# Target and sources
TARGET = parser
SRC = model/parser.cpp
OBJS = $(SRC:.cpp=.o)

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(COMPILER) $(FLAGS) -o $@ $^

%.o: %.cpp parser.hpp
	$(COMPILER) $(FLAGS) -c $<

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean