CXX = g++
CXXFLAGS = -g -L/usr/local/lib -std=c++11
LIBS = -lgtest_main -lgtest -lpthread
INCS = -I./ -I/usr/local/include -I../src

CPPSOURCES = list_test.cpp vector_test.cpp
OBJS = $(CPPSOURCES:.cpp=.o)

testAll: $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCS) -o testAll  Main_TestAll.cpp $(OBJS) $(LIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCS)

clean:
	rm testAll *.o testAll.xml
