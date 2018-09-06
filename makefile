ijcp: main.o
	g++ -g -o ijcp main.o -std=c++11

main.o: src/main.cpp
	g++ -g -c -pthread src/main.cpp -std=c++11