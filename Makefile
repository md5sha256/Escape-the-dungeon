SHELL=bash
FLAGS = -pedantic-errors -std=c++11
_UTILS = $(wildcard src/utils)
UTILS = $(wildcard $(_UTILS)/*.hpp) $(wildcard $(_UTILS)/*.cpp)

all: game
	cp game.o $@.o

# FIXME: this target
game: $(UTILS) src/main.cpp
	g++ $(FLAGS) $^ -o $@.o

demo:  $(UTILS) src/demo.cpp
	g++ $(FLAGS)  $^ -g -o $@.o

clean:
	rm -f *.o

.PHONY:
	clean