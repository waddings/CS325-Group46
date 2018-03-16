#include "Prims.hpp"

int main() {

	int V = 9;
	Prims prims(V);

	prims.addEdge(0, 1, 4);
	prims.addEdge(0, 7, 8);
	prims.addEdge(1, 2, 8);
	prims.addEdge(1, 7, 11);
	prims.addEdge(2, 3, 7);
	prims.addEdge(2, 8, 2);
	prims.addEdge(2, 5, 4);
	prims.addEdge(3, 4, 9);
	prims.addEdge(3, 5, 14);
	prims.addEdge(4, 5, 10);
	prims.addEdge(5, 6, 2);
	prims.addEdge(6, 7, 1);
	prims.addEdge(6, 8, 6);
	prims.addEdge(7, 8, 7);

	prims.primsAlgorithm();
	return 0;
}
