CXX=g++
FLAGS=-std=c++17 -Wall -Wextra -Wpedantic -D_GNU_SOURCE -Werror=all
LBLIBS= $(shell pkg-config --libs sdl2 SDL2_image)
CLASSES=$(wildcard *.hpp)
OBJECTS=$(CLASSES:.hpp=.o)
ECS=$(wildcard ECS/*.cpp)
ECS_OBJECTS=$(ECS:.cpp=.o)

all : run

main: main.cpp $(OBJECTS) $(ECS_OBJECTS)
	$(CXX) -o $@ $^ $(LBLIBS) $(FLAGS) 

%.o: %.cpp %.hpp
	$(CXX) -c $<

ECS/%.o: ECS/%.cpp ECS/Components.hpp
	$(CXX) -c $< -o $@


run:
	./check_ECS_changes.sh
	./main 
	
build:
	make clean
	make main

clean:
	find . -name \*.o -type f -delete
	