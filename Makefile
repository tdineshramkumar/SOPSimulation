# This contains functions related to adjacency list representations
network-visualisation-test: network-visualisation-test.cpp network-generator.o network-visualisation.o gfx.o
	g++ -o network-visualisation-test network-visualisation-test.cpp network-generator.o network-visualisation.o gfx.o -lX11 -std=c++11

dijkstra-routing: dijkstra-routing.cpp network-generator.o collaboration-curiosity.o performance-measures.o explorepaths.o network-visualisation.o gfx.o adjacency-list.o
	g++ -o dijkstra-routing  dijkstra-routing.cpp  network-generator.o explorepaths.o collaboration-curiosity.o performance-measures.o network-visualisation.o gfx.o adjacency-list.o -std=c++11 -lX11 -g 
final: final.cpp network-generator.o network-visualisation.o gfx.o explorepaths.o adjacency-list.o collaboration-curiosity.o performance-measures.o path-performance-comparisons.o
	g++ -o final final.cpp network-generator.o network-visualisation.o gfx.o explorepaths.o adjacency-list.o collaboration-curiosity.o performance-measures.o path-performance-comparisons.o -lX11 -std=c++11 -g

adjacency-list.o:	adjacency-list.cpp
	g++ -c adjacency-list.cpp

network-generator.o:	network-generator.cpp
	g++ -c -std=c++11 network-generator.cpp

collaboration-curiosity.o:	collaboration-curiosity.cpp
	g++ -c collaboration-curiosity.cpp -std=c++11

network-visualisation.o:	network-visualisation.cpp 
	g++ -c network-visualisation.cpp -std=c++11

explorepaths.o:	explorepaths.cpp
	g++ -c explorepaths.cpp

path-performance-comparisons.o: path-performance-comparisons.cpp
	g++ -c path-performance-comparisons.cpp

gfx.o:	gfx.c 
	g++ -c gfx.c -lX11

performance-measures.o:	performance-measures.cpp
	g++ -c performance-measures.cpp

clean:
	rm *.o
