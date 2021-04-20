dijkstra: main.o graph.o heap.o
	g++ main.o graph.o heap.o -o dijkstra

main.o:
	g++ -c main.cpp

graph.o:
	g++ -c graph.cc

heap.o:
	g++ -c heap.cc

clear:
	rm dijkstra main.o graph.o heap.o