CXX=g++
RM=rm -f -r
SOURCES=$(shell find . -type f -name '*.cpp')
HEADERS=$(shell find . -type f -name '*.h')
OBJECTS=$(subst .cpp,.o,$(SOURCES))
HEADEROBJECTS=$(subst .h,.o,$(HEADERS))

all: forest

forest: $(OBJECTS)
	$(CXX) -o forest $(OBJECTS)

forest.o: forest.cpp
actor.o: actor.h
tree.o: tree.cpp

clean:
	$(RM) $(OBJECTS) $(HEADEROBJECTS) forest
