SHELL=bash
FLAGS = -pedantic-errors -std=c++11
SDIR = src
UTILS = $(SDIR)/utils
OBJECTS =

all: game
	cp $<.o $@.o

# FIXME: this target
game: $(wildcard $(SDIR)/*.cpp) $(wildcard $(UTILS)/*.cpp)
	g++ $(FLAGS) $^ -o $@.o

demo: src/demo.cpp $(UTILS)
	g++ $(FLAGS) $< -o $@.o

main: src/main.cpp $(UTILS)
	g++ $(FLAGS) $^ -o $@.o

clean:
	rm -f *.o

.PHONY:
	clean

.SILENT:
	clean
	all