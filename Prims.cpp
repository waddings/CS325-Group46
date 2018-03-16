/* Note that this algorithm is not original. Sources to this
 * algorithm can be found on geeksforgeeks.org under Prim's Algorithm. 
 * This is simply a modeified version of that algorithm. */
 
#include "Prims.hpp"
#include <iostream>

Prims::Prims(int V) {
	Vertices = V;
	adj = new std::list<std::pair<int,int> >[V]; 
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

	int source = 0;
	priorityQ.push(std::make_pair(0, source));
	key[source] = 0;		 

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

	for (int i = 1; i < Vertices; i++) {
		std::cout << parent[i] << " - " << i << std::endl;
	}		
}
