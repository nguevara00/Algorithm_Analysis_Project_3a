CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

TARGET = index

all:
	$(CXX) $(CXXFLAGS) main.cpp bst.cpp TwoThreeTree.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)