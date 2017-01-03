CXX=g++

CPPFLAGS=
CXXFLAGS=-O3 -lm
LDFLAGS=

all: routescheduling

routescheduling: main.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -o bin/$@ $(addprefix obj/,$^)

%.o: src/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o obj/$@ $^

clean:
	rm -f bin/*
	find obj -type f -delete
