0710880.o:0710880.cpp func.o graph.o
	g++ -o 0710880.o 0710880.cpp  func.o graph.o  -std=c++11
func.o:func.h func.cpp
	g++ -c func.cpp -std=c++11
graph.o:graph.h graph.cpp
	g++ -c graph.cpp -std=c++11

clean:
	rm *.o
