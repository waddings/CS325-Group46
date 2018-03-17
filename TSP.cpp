#include <iostream>
#include <fstream>
#include <stack>
#include <string.h>
#include <string>
#include <math.h>
#include <vector>
#include <limits.h>
#include <time.h>

using namespace std;

struct city{
    int name, x, y;
};

struct city *buildCityList(char**, int*);
double d(struct city, struct city);
void fillDistanceGraph(int, int**, struct city*);
void primMST(int, int**, vector<int>*);
int minKey(int*, bool*, int);
void storeOdds(vector<int>, vector<int>*, int);
void findPerfectMatching(int **, vector<int>, vector<int> *);
void CreateEuler(int pos, vector<int> &path, int numcities, vector<int> *alist);
void convertToHamiltonian(std::vector<int> &path, int &pathDistance, int numcities, int* graph[]);
void outputFile(int &pathDistance, vector<int> &circuit, char **, struct city*, int);

int main(int argc, char* argv[]){
    // Ensure the user entered a filename argument
    if (argc < 2) {
	     return(-1);
    }

    // Initialize starting Variables
    int numcities;
    int pathDistance = 0;
    int finalPathDistance = INT_MAX; // Set equal to really high number
    vector<int> circuit;
    struct city *cities = buildCityList(argv, &numcities);
    int **graph = new int*[numcities];
    for(int i = 0; i < numcities; i++){
      graph[i] = new int[numcities];
    }

    for(int i= 0; i < numcities; i++){
      for(int j = 0; j < numcities; j++)
          graph[i][j] = 0;
    }

    vector<int> *alist = new vector<int> [numcities];
    vector<int> odds;

  //  clock_t t1, t2;
  //  t1 = clock();
    fillDistanceGraph(numcities, graph, cities);
    int starterVertex = 0;
    primMST(numcities, graph, alist);
    storeOdds(odds, alist, numcities);
    findPerfectMatching(graph, odds, alist);
    CreateEuler(starterVertex, circuit, numcities, alist);
    convertToHamiltonian(circuit, finalPathDistance, numcities, graph);
  //  t2 = clock();
  //  cout << ((float)t2 - (float)t1)/ CLOCKS_PER_SEC << endl;
    outputFile(finalPathDistance, circuit, argv, cities, numcities);
    delete [] cities;
    for(int i = 0; i < numcities; i++)
        delete [] graph[i];

    return 0;
}

void findPerfectMatching(int *graph[], vector<int> odds, vector<int> *alist){
    int closest, length; //int d;
    vector<int>::iterator tmp, first;

    // for each odd node
    while (!odds.empty()) {
      first = odds.begin();
      vector<int>::iterator it = odds.begin() + 1;
      vector<int>::iterator end = odds.end();
      length = INT_MAX;
      for (; it != end; ++it) {
        // if this node is closer than the current closest, update closest and length
        if (graph[*first][*it] < length) {
          length = graph[*first][*it];
          closest = *it;
          tmp = it;
        }
      }	// two nodes are matched, end of list reached
      alist[*first].push_back(closest);
      alist[closest].push_back(*first);
      odds.erase(tmp);
      odds.erase(first);
    }
}

void storeOdds(vector<int> odds, vector<int> *alist, int n){
    for (int i = 0; i < n; i++) {
      if ((alist[i].size() % 2) != 0 )
        odds.push_back(i);
  }
}

// Borrowed from github.com/backysag/traveling-salesman/blob/master/tsp.cpp
void CreateEuler(int pos, vector<int> &path, int numcities, vector<int> *alist ) {
	// copy the adjacency list so we can modify it later
	std::vector<int> * temp = new vector<int> [numcities];
	for (int i = 0; i < numcities; i++) {
		temp[i].resize(alist[i].size());
		temp[i] = alist[i];
	}

	path.clear();

	// Loop until the stack is empty
	std::stack<int> stack;
	while (!stack.empty() || temp[pos].size() > 0) {
		// If the vertex has no neighboring vertices
		if (temp[pos].size() == 0) {
			// add to circuit
			path.push_back(pos);
			// remove the last vertex from teh stack
			int last = stack.top();
			stack.pop();
			pos = last;
		}
		else {
			// add vertex to the stack
			stack.push(pos);
			// take any of neighboring vertices
			int neighbor = temp[pos].back();
			// remove edge between selected neighbor
			temp[pos].pop_back();

			for (int i = 0; i < temp[neighbor].size(); i++) {
				if (temp[neighbor][i] == pos) {
					temp[neighbor].erase(temp[neighbor].begin() + i);
					break;
				}
			}
			pos = neighbor;
		}
	}
	path.push_back(pos);
	vector<int>::iterator itr;
}

void convertToHamiltonian(vector<int> &path, int &pathDistance, int numcities, int* graph[]) {
	// remove nodes that have been visited
	bool visited[numcities];
	for (int i = 0; i < numcities; i++) {
		visited[i] = false;
	}

	pathDistance = 0;

	// Set up the iterators and visited roots
	int root = path.front();
	vector<int>::iterator cur = path.begin();
	vector<int>::iterator next = path.begin() + 1;
	visited[root] = true;

	// Loop until finished the circuit
	while (next != path.end()) {
		// Visit next city if its vertex hasn't been visited
		if(!visited[*next]) {
			pathDistance += graph[*cur][*next];
			cur = next;
			visited[*cur] = true;
			next = cur + 1;
		} else {
			next = path.erase(next); // remove from path
		}
	}

	// Add the distance back to the root
	//pathDistance += graph[*cur][*next];
}

void primMST(int n, int* graph[], vector<int> *alist){             //borrowed from: https://github.com/beckysag/traveling-salesman/blob/master/tsp.cpp
  int key[n];   // Key values used to pick minimum weight edge in cut
	bool in_mst[n];  // To represent set of vertices not yet included in MST
	int parent[n];

	// For each vertex v in V
	for (int v = 0; v < n; v++) {
		// Initialize all keys to infinity
		key[v] = INT_MAX;

		// Mark as not being in mst yet
		in_mst[v] = false;
	}

	// Node 0 is the root node so give it the lowest distance (key)
	key[0] = 0;
	parent[0] = -1; // First node is always root of MST

	for (int i = 0; i < n - 1; i++) {
		// Find closest remaining (not in tree) vertex
		int v = minKey(key, in_mst, n);

		// Add vertex v to the MST
		in_mst[v] = true;

		// Look at each vertex u adjacent to v that's not yet in mst
		for (int u = 0; u < n; u++) {
			if (graph[v][u] && in_mst[u] == false && graph[v][u] < key[u]) {
				// Update parent index of u
				parent[u] = v;

				// Update the key only if dist is smaller than key[u]
				key[u] = graph[v][u];
			}
		}
	}

	// map relations from parent array onto matrix
	for (int v1 = 0; v1 < n; v1++) {
		// there is an edge between v1 and parent[v1]
		int v2 = parent[v1];
		if (v2 != -1) {
			alist[v1].push_back(v2);
			alist[v2].push_back(v1);
		}
	}
}

int minKey(int key[], bool mstSet[], int n) {
	// Initialize min value
	int min = INT_MAX;
	int min_index;
	for (int v = 0; v < n; v++)
		if (mstSet[v] == false && key[v] < min) {
			min = key[v];
			min_index = v;
		}
	return min_index;
}

void fillDistanceGraph(int n, int* g[], struct city * c){
    for(int i = 0; i < n; i++){
        for(int j = i; j < n; j++) {
            g[i][j] = g[j][i] = d(c[i], c[j]);
	}
    }
}

double d(struct city c1, struct city c2){
    int dx = ((float)((c2.x - c1.x) * (c2.x - c1.x)));
    int dy = ((float)((c2.y - c1.y) * (c2.y - c1.y)));
    return (floor((float)(sqrt(dx + dy))+ 0.5));
}

struct city *buildCityList(char* argv[], int *s){
    int n = 0;
    char filename[50];
    ifstream f;
    strcpy(filename, argv[1]);
  //  cout << filename << endl;
    f.open(filename);

    string buffer;
    while(getline(f, buffer))
        n++;
  //  cout << "n is: " << n << endl;
    *s = n;
    f.clear();
    f.seekg(0);
    struct city* cityList = new struct city[n];
    int i = 0;
    while(!f.eof()){
      f >> cityList[i].name >> cityList[i].x >> cityList[i].y;
      i++;
    }

    return cityList;
}

void outputFile(int &pathDistance, vector<int> &circuit, char *argv[], struct city *cities, int numcities) {
	ofstream outputFile;
  char *outstring = new char [strlen(argv[1]) + 5];
  strcpy(outstring, argv[1]);
  strcat(outstring, ".tour");
	outputFile.open(outstring);
  struct city c [(int)circuit.size()];

  int i = 0;
	for (vector<int>::iterator itr = circuit.begin(); itr != circuit.end(); itr++) {
    c[i] = cities[*itr];
    i++;
	}
  int dist = 0;
  for(i = 0; i < numcities && i+1 < numcities; i++)
      dist += d(c[i], c[i+1]);
  dist += d(c[0], c[numcities-1]);
  outputFile << dist << endl;
  for(i = 0; i < numcities; i++)
      outputFile << c[i].name << endl;
	outputFile.close();

  delete [] outstring;
}
