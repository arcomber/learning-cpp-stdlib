CXXFLAGS=-Wall -pedantic -std=c++17
CPPSOURCES = main.cpp resizing_array.hpp
OBJ = $(CPPSOURCES:.cpp=.o)

resizing_array_test: $(OBJ)
	$(CXX) -o $@ $^ $(LINKERFLAGS)

%.o: %.cpp
	g++ $(CXXFLAGS) -o $@ -c $<

%.o: %.c
	cc -o $@ -c $<
