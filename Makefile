SHELL=bash
FLAGS = -pedantic-errors -std=c++11
SDIR = src
UTILS = $(SDIR)/utils
MODEL = $(SDIR)/model
INC = include/

all: $(wildcard $(SDIR)/*.cpp) $(wildcard $(UTILS)/*.cpp)
	g++ $(FLAGS) -I$(INC) $^ -o $@.o

# FIXME: this target
game1: $(wildcard $(SDIR)/*.cpp) $(wildcard $(UTILS)/*.cpp)
	g++ $(FLAGS) $^ -o $@.o

demo: src/demo.cpp src/playerinfo.hpp $(wildcard $(UTILS)/*.cpp) $(wildcard $(MODEL)/*.cpp)
	g++ $(FLAGS) -I$(INC) $^ -o $@.o

main: src/main.cpp src/playerinfo.hpp src/campfire.cpp src/start.cpp src/battle.cpp
	g++ $(FLAGS) $^ -g -o $@.o

clean:
	rm -f *.o

.PHONY:
	clean
