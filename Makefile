SHELL=bash
FLAGS = -pedantic-errors -std=c++11
SDIR = src
UTILS = $(SDIR)/utils
MODEL = $(SDIR)/model
OBJECTS =

all: game1
	cp $<.o $@.o

# FIXME: this target
game1: $(wildcard $(SDIR)/*.cpp) $(wildcard $(UTILS)/*.cpp)
	g++ $(FLAGS) $^ -o $@.o

demo: src/demo.cpp src/playerinfo.hpp $(wildcard $(UTILS)/*.cpp) $(wildcard $(MODEL)/*.cpp)
	g++ $(FLAGS) $^ -o $@.o

main: src/main.cpp $(UTILS)
	g++ $(FLAGS) $^ -o $@.o

clean:
	rm -f *.o

.PHONY:
	clean

.SILENT:
	clean
