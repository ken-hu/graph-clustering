CXX 		= g++
CXXFLAGS 	= -Wall -std=c++11 -O2

OBJECTS 	= main.o graph.o lanczos.o tqli.o
TESTOBJECTS = test.o graph.o lanczos.o tqli.o
TARGET 		= main
TESTTARGET 	= test

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

%.o:%.cc
	$(CXX) $(CXXFLAGS) -c $<

# Explicit dependencies required for headers
graph.o: 	graph.h
lanczos.o: 	lanczos.h
tqli.o:		tqli.h

# Phony target to get around problem of having a file called 'clean'
.PHONY: clean
clean:
	rm -f $(OBJECTS) $(TARGET) $(TESTOBJECTS) $(TESTTARGET)

$(TESTTARGET): $(TESTOBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(TESTOBJECTS)
	time ./$(TESTTARGET)

output: main
	time ./main > graph.dot

unit_tests: $(TARGET).o
	make -C unit_tests test
