/* Note that this algorithm is not original. Sources to this
 * algorithm can be found on geeksforgeeks.org under Prim's Algorithm. 
 * This is simply a modeified version of that algorithm. */
 
#include "Prims.hpp"
#include <iostream>

Prims::Prims(int V) {
	Vertices = V;
	adj = new std::list<std::pair<int,int> >[V];
	adjacencyList = new std::list<int>[V];
	numberOfOddVertices = 0;
	oddVertices = new int [V];	

}

void Prims::addEdge(int u, int v, int w) {
	adj[u].push_back(std::make_pair(v,w));
	adj[v].push_back(std::make_pair(u,w));	
}

// Outputs a adjacency matric of the minimum spanning tree
void Prims::primsAlgorithm() {
	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int,int> >, std::greater<std::pair<int,int> > > priorityQ;

	std::vector<int> key(Vertices, INT_MAX); // INT_MAX acts as infinity

	std::vector<int> parent(Vertices, -1); // Negative 1 represents no parent predecssor

	std::vector<bool> inMST(Vertices, false); // This tracks the vertices if they are in the minimum spanning tree

	// Set up the priority Queue
	int source = 0;
	priorityQ.push(std::make_pair(0, source));
	key[source] = 0;		 

	// Keep looping until all vertices have been added to the MST
	while (!priorityQ.empty()) {
		int u = priorityQ.top().second;
		priorityQ.pop();

		inMST[u] = true;

		std::list<std::pair<int, int> >::iterator itr;
		for (itr = adj[u].begin(); itr != adj[u].end(); itr++) {
			int v = (*itr).first;
			int weight = (*itr).second;

			if (inMST[v] == false && key[v] > weight) {
				key[v] = weight;
				priorityQ.push(std::make_pair(key[v], v));
				parent[v] = u;
			}	
		}
		
	}

	for (int i = 0; i < Vertices; i++) {
		if (i != 0) {
			adjacencyList[parent[i]].push_back(i);
		}

		// -1 represented no parental ancestor therefore we remove it in the adjacency list
		if (parent[i] != -1) {  
			adjacencyList[i].push_back(parent[i]);
		}
	}

	int counter = 0;	
	std::list<int>::iterator it;
	for (int i = 0; i < Vertices; i++) {
		for (it = adjacencyList[i].begin(); it != adjacencyList[i].end(); it++) {
			counter++;
		}
		if (counter % 2 == 1) {
			oddVertices[numberOfOddVertices] = i;
			numberOfOddVertices++; 	
		}	
		counter = 0;
	}

// Test output
/*
	//std::list<int>::iterator it;
	for (int i = 0; i < Vertices; i++) {
		for (it = adjacencyList[i].begin(); it != adjacencyList[i].end(); it++) {
			std::cout << *it << " "; 
		}
		std::cout << std::endl;
	}
*/
}
