CXX = g++
CXXFLAGS = -pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -nostdinc++ -lm -pg

lab5: Arch.o Linearizator.o AVLTree.o PositionInTree.o Common.o SuffixTree.o SuffixTreeBuilder.o Vertex.o Parser.o Worker.o main.o
	$(CXX) $(CXXFLAGS) -o lab5 Common.o Arch.o AVLTree.o Linearizator.o PositionInTree.o SuffixTree.o SuffixTreeBuilder.o Vertex.o Parser.o Worker.o main.o

clean :
	rm *.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

Arch.o : Arch.cpp
	$(CXX) $(CXXFLAGS) -c Arch.cpp

AVLTree.o : AVLTree.cpp
	$(CXX) $(CXXFLAGS) -c AVLTree.cpp

Linearizator.o : Linearizator.cpp
	$(CXX) $(CXXFLAGS) -c Linearizator.cpp

PositionInTree.o : PositionInTree.cpp
	$(CXX) $(CXXFLAGS) -c PositionInTree.cpp

Common.o : Common.cpp
	$(CXX) $(CXXFLAGS) -c Common.cpp

SuffixTree.o : SuffixTree.cpp
	$(CXX) $(CXXFLAGS) -c SuffixTree.cpp

SuffixTreeBuilder.o : SuffixTreeBuilder.cpp
	$(CXX) $(CXXFLAGS) -c SuffixTreeBuilder.cpp

Vertex.o : Vertex.cpp
	$(CXX) $(CXXFLAGS) -c Vertex.cpp

Parser.o : Parser.cpp
	$(CXX) $(CXXFLAGS) -c Parser.cpp

Worker.o : Worker.cpp
	$(CXX) $(CXXFLAGS) -c Worker.cpp
