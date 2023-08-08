/*
A graph of 50 nodes is implemented with distance ranging from 1 to 10. The name of the nodes are from A-Za-z.
 
Graph with density 20% and 40% is created.

The shortest path is calculated using Dijkstra's Algorithm

*/





#include<iostream>
#include<vector>
#include<list>
#include<map>
#include<ctime>
#include<cstdlib>

using namespace std;




const int noedge=999999; // to represent no edge/path between two nodes 


ostream &operator<<(ostream &output, list<char> L) // Overload operator << to print list<char> variables
{
   list<char>::iterator i;
   for(i=L.begin(); i != L.end(); ++i)
     output << *i << " ";
   return output;
}


inline char vertIntToChar(int n) // Convert node numbers into chars 
{
  if (n<26)
    return static_cast<char>('A'+n);
  else
    return static_cast<char>('a'+n-26);
}




typedef struct strNode Node; // Node definitions
struct strNode
{
  int number;	
  int weight;	
  list<Node> edges;	
};


class Graph
{
  public:
    Graph();
    Graph(int numVertices, int initialValue);
    char get_node_value(int x);
    void set_node_value(char x, char name);
    int get_edge_value(char x, char y);
    void set_edge_value(char x, char y, int value);
    bool adjacent(char x, char y);
    list<char> neighbors(char x);
    int V();
    int E();
    list<char> vertices();
    void show();
  
  private:
    int numV;			// Number of nodes of the Graph
    int numE;			// Number of edges of the Graph
    vector<char> nodeNames;	// Map node numbers into node names
    map<char, int> nodeNumbers;	// Map node names into node numbers
    list<Node> adjList;		// Adjacency list representing the Graph
};


Graph::Graph()
{
  numV = 0;
  numE = 0;
  adjList.clear();
}



Graph::Graph(int numVertices, int initialValue=noedge) // Constructor of Graph Class
{
  
  numV = numVertices;
  numE = 0;
  nodeNames.resize(numVertices);
  for (int x=0; x<numV; ++x)		// Create nodes and link it to default names (0..51 -> A..Za..z)
  {  
    nodeNames[x] = vertIntToChar(x);
    nodeNumbers[vertIntToChar(x)]=x;
  }
  
  
  adjList.clear(); 			// Create adjacency list with all nodes and empty edge list
  for(int i=0; i<numVertices; ++i)
  {
    Node newNode;
    newNode.number = i;
    newNode.weight = 0;
    newNode.edges.clear();
    adjList.push_back(newNode);
  }
}


char Graph::get_node_value(int x)	// Return node name 
{
  return nodeNames[x];
}


void Graph::set_node_value(char x, char name)	// Change node name 
{
  int posX = nodeNumbers[x];	// Get the number of node 'x'
  nodeNames[posX] = name;	// Link node number to 'name'
  nodeNumbers[name]=posX;	// Link 'name' to node number
}



int Graph::get_edge_value(char x, char y) // Return edge weight
{
  for(list<Node>::iterator i=adjList.begin(); i != adjList.end(); ++i)
  {
    if ((*i).number==nodeNumbers[x])
      for(list<Node>::iterator j=(*i).edges.begin(); j != (*i).edges.end(); ++j)
      {
	 if ((*j).number==nodeNumbers[y])
	   return (*j).weight; 
      }
  }
  return noedge;
}


void Graph::set_edge_value(char x, char y, int value) // Set edge weight 
{
  bool found;
  // Add 'y' in the list of 'x' neighbors (if doesn't exist)
  // Set edge weight to value
  for(list<Node>::iterator i=adjList.begin(); i != adjList.end(); ++i)
  {
    if ((*i).number==nodeNumbers[x])
    {
      found = false;
      for(list<Node>::iterator j=(*i).edges.begin(); j != (*i).edges.end(); ++j)
      {
	 if ((*j).number==nodeNumbers[y])
	 {  
	   (*j).weight=value;
	   found = true;
	 }
      }
      if (!found)
      {
	Node newNodeY;
	newNodeY.number = nodeNumbers[y];
	newNodeY.weight = value;
	newNodeY.edges.clear();
	(*i).edges.push_back(newNodeY);
      }
    }
  }
  
  // Add 'x' in the list of 'y' neighbors (if doesn't exist)
  // Set edge weight to value
  for(list<Node>::iterator i=adjList.begin(); i != adjList.end(); ++i)
  {
    if ((*i).number==nodeNumbers[y])
    {
      found = false;
      for(list<Node>::iterator j=(*i).edges.begin(); j != (*i).edges.end(); ++j)
      {
	 if ((*j).number==nodeNumbers[x])
	 {  
	   (*j).weight=value;
	   found = true;
	 }
      }
      if (!found)
      {
	Node newNodeX;
	newNodeX.number = nodeNumbers[x];
	newNodeX.weight = value;
	newNodeX.edges.clear();
	(*i).edges.push_back(newNodeX);
	++numE;	  	// Increment the number of edges in the graph
      }
    }
  }
}


bool Graph::adjacent(char x, char y) // Return true if 'x' and 'y' are neighbors and false otherwise
{
  for(list<Node>::iterator i=adjList.begin(); i != adjList.end(); ++i)
  {
    if ((*i).number==nodeNumbers[x])
    {
      for(list<Node>::iterator j=(*i).edges.begin(); j != (*i).edges.end(); ++j)
      {
	if ((*j).number==nodeNumbers[y])
	{  
	  return true;
	}
      }
    }
  }
  return false;
}

// Return a list<char> containing the list of neighbors of 'x'
list<char> Graph::neighbors(char x)
{
  list<char> adjNodes;
  for(list<Node>::iterator i=adjList.begin(); i != adjList.end(); ++i)
  {
    if ((*i).number==nodeNumbers[x])
    {
      for(list<Node>::iterator j=(*i).edges.begin(); j != (*i).edges.end(); ++j)
      {
	adjNodes.push_back(nodeNames[(*j).number]);
      }
    }
  }
  return adjNodes;
}
   
// Return the number of nodes in the Graph
int Graph::V()
{
  return numV;
}

// Return the number of edges in the Graph
int Graph::E()
{
  return numE;
}

// Return a list<char> containing all nodes in the Graph
list<char> Graph::vertices()
{
  list<char> nodes;
  for(list<Node>::iterator i=adjList.begin(); i != adjList.end(); ++i)
  {
    nodes.push_back(nodeNames[(*i).number]);
  }
  return nodes;
}

// Print out adjacency list representing the Graph
void Graph::show()
{
  cout << "  ";
  for(list<Node>::iterator i=adjList.begin(); i != adjList.end(); ++i)
    cout << " " << nodeNames[(*i).number];
  cout << endl;
  for(list<Node>::iterator i=adjList.begin(); i != adjList.end(); ++i)
  {
    cout << " " << nodeNames[(*i).number];
    int shift=0;
    for(list<Node>::iterator j=(*i).edges.begin(); j != (*i).edges.end(); ++j)
    {
      int walk=(*j).number-shift;
      for(int k=0; k<walk; ++k)
      {
	cout << " -";
	shift++;
      }
      cout << " " << (*j).weight;
      shift++;
    }
    while (shift<numV)
    {
      cout << " -";
      shift++;
    }
    cout << endl;
  }
}


// NodeInfo Definitions

struct strNodeInfo
{
  char nodeName;	// Node name
  int minDist;		// Shortest path found to nodeName
  char through;		// Node that precede nodeName in the shortest path
};
typedef struct strNodeInfo NodeInfo;

// Compare NodeInfo by nodeName
bool compareNodeName(NodeInfo& n1, NodeInfo& n2)
{
  if (n1.nodeName < n2.nodeName) return true;
  return false;
}

// Compare NodeInfo by minDist
bool compareMinDist(NodeInfo& n1, NodeInfo& n2)
{
  if (n1.minDist < n2.minDist) return true;
  return false;
}

// Return true if two NodeInfo have the same nodeName and false otherwise
bool operator== (NodeInfo& n1, NodeInfo& n2)
{
  if (n1.nodeName == n2.nodeName) return true;
  return false;
}


// PriorityQueue Class

class PriorityQueue {
  public:
    PriorityQueue();
    void chgPriority(NodeInfo n);
    void minPriority();
    bool contains(NodeInfo n);
    bool isBetter(NodeInfo n);
    void insert(NodeInfo n);
    NodeInfo top();
    int size();
    
  private:
    list<NodeInfo> pq;		// List of known nodes/paths ordered by minDist
};

PriorityQueue::PriorityQueue()
{
  pq.clear();
}

// Change information ('minDist' and 'through') of a node named 'n' in priority queue
void PriorityQueue::chgPriority(NodeInfo n)
{
  for(list<NodeInfo>::iterator i=pq.begin(); i!=pq.end(); ++i)
    if ((*i) == n)
    {
      (*i).minDist = n.minDist;
      (*i).through = n.through;
    }
  pq.sort(compareMinDist);
}

// Remove the node with lower minDist from priority queue 
void PriorityQueue::minPriority()
{
  if (! pq.empty())
  {
    pq.pop_front();
  }
}

// Returne true if there is a node named 'n' in priority queue and false otherwise 
bool PriorityQueue::contains(NodeInfo n)
{
  for(list<NodeInfo>::iterator i=pq.begin(); i!=pq.end(); ++i)
    if ((*i).nodeName == n.nodeName)
      return true;
  return false;
}

// Return true if node 'n' has a lower minDist than the node with the same name in the priority queue and false otherwise
bool PriorityQueue::isBetter(NodeInfo n)
{
  for(list<NodeInfo>::iterator i=pq.begin(); i!=pq.end(); ++i)
    if ((*i).nodeName == n.nodeName)
      if ((*i).minDist > n.minDist)
	return true;
  return false;
}

// Insert node 'n' into priority queue
void PriorityQueue::insert(NodeInfo n)
{
  pq.push_back(n);
  pq.sort(compareMinDist);
}

// Return the node with lower minDist in priority queue (without removing it from the queue))
NodeInfo PriorityQueue::top()
{
  NodeInfo n = {' ',0};
  if (! pq.empty())
  {
    list<NodeInfo>::iterator i=pq.begin();
    n.nodeName = (*i).nodeName;
    n.minDist = (*i).minDist;
    n.through = (*i).through;
  }
  return n;
}

// Return the number of elements in the priority queue
int PriorityQueue::size()
{
  return pq.size();
}



class ShortestPath	// Dijkstra's Algorithm
{
  public:
    ShortestPath();
    ShortestPath(Graph g);
    list<char> path(char u, char w);
    int path_size(char u, char w);
  
  private:
    Graph graph;		// Graph used by Diajkstra's Algorithm
};

// Constructor of ShortestPath Class (do nothing)
ShortestPath::ShortestPath()
{
}

// Constructor of ShortestPath Class that stores Graph used by Dijkstra's Algorithm 
ShortestPath::ShortestPath(Graph g)
{
  graph = g;
}

// Return a list<char> containing the list of nodes in the shortest path between 'u' and 'w'
list<char> ShortestPath::path(char u, char w)
{
  // Initialize candidates list with all nodes
  list<char> candidates = graph.vertices(), desiredPath;
  list<NodeInfo> minPaths;
  PriorityQueue p;
  NodeInfo lastSelected, n;
     
  // Calculate shortest path from 'u' to 'w' (Dijkstra's Algorithm)
  candidates.remove(u);			// Remove 'u' from candidates list
  lastSelected.nodeName = u;		// Set 'u' as lastSelected
  lastSelected.minDist = 0;
  lastSelected.through = u;
  minPaths.push_back(lastSelected);	// Add 'u' to minPath list
  while ((!candidates.empty()) && (lastSelected.nodeName !=w))
  {
    // For each node in candidate list calculate the cost to reach that candidate through lastSelected 
    for(list<char>::iterator i=candidates.begin(); i != candidates.end(); ++i)
    {
      n.nodeName=*i;
      n.minDist=lastSelected.minDist+graph.get_edge_value(lastSelected.nodeName,*i);
      n.through=lastSelected.nodeName;
      if (!p.contains(n))	// Add candidate to priority queue if doesn't exist 
	p.insert(n);
      else
	if (p.isBetter(n))	// Update candidate minDist in priority queue if a better path was found
	  p.chgPriority(n);
    }
    lastSelected = p.top();			// Select the candidate with minDist from priority queue
    p.minPriority();				// Remove it from the priority queue
    minPaths.push_back(lastSelected);		// Add the candidate with min distance to minPath list
    candidates.remove(lastSelected.nodeName);	// Remove it from candidates list
  }
  
  // Go backward from 'w' to 'u' adding nodes in that path to desiredPath list
  lastSelected=minPaths.back();
  desiredPath.push_front(lastSelected.nodeName);
  while(lastSelected.nodeName!=u)
  {
    for(list<NodeInfo>::iterator i=minPaths.begin(); i != minPaths.end(); ++i)
      if ((*i).nodeName==lastSelected.through)
      {
	lastSelected=(*i);
	desiredPath.push_front(lastSelected.nodeName);
      }
  }
  return desiredPath;
}

// Return the size of the shortest path between 'u' and 'w'
int ShortestPath::path_size(char u, char w)
{
  int pathCost=0;
  list<char> sp;
  char current,next;
  
  // Calculate the shortest path from 'u' to 'w' and then sum up edge weights in this path
  sp = path(u,w);
  current=sp.front();
  sp.pop_front();
  for(list<char>::iterator i=sp.begin(); i!=sp.end(); ++i)
  {
    next = (*i);
    pathCost += graph.get_edge_value(current,next);
    current = next;
  }
  return pathCost; 
}								// end of algorithm


class MonteCarlo
{
  public:
    MonteCarlo();
    Graph randomGraph(int vert, double density, int minDistEdge, int maxDistEdge);
    void run(Graph g);
  
  
};

// Constructor of MonteCarlo Class
// Initializes the seed of random number generator
MonteCarlo::MonteCarlo()
{
  srand(time(NULL));
}

// Return a random Graph generated with number of nodes, density and edge weight range informed
Graph MonteCarlo::randomGraph(int numVert, double density, int minDistEdge, int maxDistEdge)
{
  int randDistEdge;
  char srcVert, dstVert;
  
  Graph g(numVert);

  for (int i=0; i<g.V(); ++i)
    for (int j=i+1; j<g.V(); ++j)
    {
      double p = ((static_cast<double>(rand())) / RAND_MAX);	// Generate random probability
      if (p < density)	// If random probability is less than density, edge (i,j) will be set
      {
	randDistEdge = rand() % (maxDistEdge - minDistEdge) + minDistEdge; // Generate random edge weight 
	srcVert = vertIntToChar(i);
	dstVert = vertIntToChar(j);
	g.set_edge_value(srcVert,dstVert,randDistEdge);
      }
    }
  
  return g;
}

// Run a simulation finding the shortest paths in a given graph 
void MonteCarlo::run(Graph g)
{
  static int turn=0;
  
  
  
  // Print out graph information
  double d = static_cast<double>(g.E())/((static_cast<double>(g.V())*static_cast<double>(g.V())-1)/2)*100;	// Calculate real density reached
  cout << "Nodes: " << g.V() << endl;
  cout << "Edges: " << g.E() << " (density: " << d << "%)" << endl;
  cout << "Graph: " << endl;
  g.show();

  // Print out shortest path information
  list<char> v = g.vertices();
  cout << endl << "Nodes: " << v << endl; 
  int reachVert=0, sumPathSize=0, avgPathSize=0;
  ShortestPath sp(g);
  for (list<char>::iterator i=++v.begin(); i != v.end(); ++i) 
  {
    char src = v.front();
    char dst = (*i);
    list<char> p = sp.path(src,dst);
    int ps = sp.path_size(src,dst);
    if (ps != noedge)
      cout << "ShortestPath (" << src << " to " << dst << "): " << ps << " -> " << p << endl;
    else
      cout << "ShortestPath (" << src << " to " << dst << "): " << "** UNREACHABLE **" << endl;      
    if (ps!=noedge)
    {
      reachVert++;		// Sum up reached nodes 
      sumPathSize += ps;	// Sum up shortest paths found
    }
  }  
  
  // Calculate average shortest path and print it out
  if (reachVert!=0)
    avgPathSize = sumPathSize / reachVert;	
  else
    avgPathSize = 0;
  cout << endl << "AVG ShortestPath Size (reachVert: " << reachVert << " - sumPathSize: " << sumPathSize << "): " << avgPathSize << endl;
}

int main()
{
  MonteCarlo simulation;
  Graph g;
  
  // Creates a graph with 50 nodes / density 20% and then run simulation
  g = simulation.randomGraph(50,0.2,1,10);
  simulation.run(g);

  // Creates a graph with 50 nodes / density 40% and then run simulation
  g = simulation.randomGraph(50,0.4,1,10);
  simulation.run(g);
  
  return 0;  
}
