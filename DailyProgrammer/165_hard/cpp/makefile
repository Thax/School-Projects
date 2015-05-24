CXX=g++
RM=rm
SOURCES=$(shell find . -type f -name '*.cpp')
OBJECTS=$(subst .cpp,.o,$(SOURCES))

all: forest

forest: $(OBJECTS)
	$(CXX) -o forest $(OBJECTS)

forest.o: forest.cpp

clean:
	$(RM) $(OBJECTS) forest
