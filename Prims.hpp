/* NOTE that this algorithm is not original. Sources to this
 * algorithm can be found on geeksforgeeks.org under Prim's
 * Algorithm. This is simply a modified version of that algorithm*/
 
#ifndef Prims_HPP
#define Prims_HPP

#include <list>
#include <utility>
#include <limits.h>
#include <queue>

class Prims {
	private:
		int Vertices; // Number of Vertices
	
		std::list<std::pair<int, int> > * adj;					
	public:
		Prims(int V); // The constructor takes the # of Vertices as an argument

		void addEdge(int u, int v, int w);
		
		void primsAlgorithm();
};


#endif
