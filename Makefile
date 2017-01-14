CXX=g++

CPPFLAGS=
CXXFLAGS=-O3 -g0 -std=c++11
LDFLAGS=-lm

all: routescheduling

routescheduling: Main.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -o bin/$@ $(addprefix obj/,$^)

%.o: src/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o obj/$@ $^

clean:
	rm -f bin/*
	find obj -type f -delete
