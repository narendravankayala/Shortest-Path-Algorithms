#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <vector>
#include <string>
#include <queue>
#include <list>

using namespace std;

class Graph {
	int vertices;
	list<pair<int, int>> *unDirectedEdges;	//stores only undirected edges
	list<pair<int, int>> *directedEdges;	//stores only directed edges
	

public:
	Graph(int vertices);
	void addEdge(int u, int v, int weight, bool directed);

	void printParentPath(vector<int> parent, int i, int src);
	void shortestPath(int s, bool directed);
	void minimumSpanningTree(int s);
};

Graph::Graph(int V)
{
	this->vertices = V;

	//Define both undirected & directed edges list with number of vertices
	unDirectedEdges = new list<pair<int, int>>[vertices];
	directedEdges = new list<pair<int, int>>[vertices];
}

//Adds one edge at a time, based on the directed flag
void Graph::addEdge(int u, int v, int weight, bool directed)
{
	u = u - 'A';
	v = v - 'A';

	if (directed)	//If this is true, adds only directed edges
	{
		directedEdges[u].push_back(make_pair(v, weight));
	}
	else			//If this is true, adds only undirected edges
	{
		unDirectedEdges[u].push_back(make_pair(v, weight));
		unDirectedEdges[v].push_back(make_pair(u, weight));
	}
}

//This is a recursive function prints the parent node of a particular node
void Graph::printParentPath(vector<int> parent, int i, int src)
{
	if (i != src)
	{
		printParentPath(parent, parent[i], src);
		cout << " - " << (char)(i + 'A');
	}
	else
	{
		cout << (char)(src + 'A');
	}
}

//Shortest Path using Dijkstra's algorithm
void Graph::shortestPath(int src, bool directed)
{
	//Using cloud as a priority Queue helps us to reduce the time complexity, 
	//priority queue doen't have option to reduce already present node's key, so we are inserting a
	//duplicate key with reduced weight, this way while picking it will pick the minimum dist node
	//dist vector is used to store that particular node dist which is subject to change,
	//later we may revisit if there is any lesser distance than the available one.
	//parent vector is used to keep track of the current node parent, used to print the path
	//Explored vector is used to keep track of already explored nodes.

	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> cloud;
	vector<int> dist(vertices, INT_MAX);		//Initialize everthing with INT_MAX
	vector<int> parent(vertices, INT_MAX);
	vector<bool> explored(vertices, false);		//Initialize everthing with false

	src = src - 'A';
	cloud.push(make_pair(0, src));				//push the source node into the cloud
	dist[src] = 0;								//source node distance will be '0' 
	parent[src] = 0;							//parent of the source node will not be there. hence assigning '0'

	while (!cloud.empty())						//traverse until there are no nodes left in the cloud
	{
		//pick the top most node, as it is min-heap based priority queue, the least distance node will always
		//be at the top of the heap
		int u = cloud.top().second;
		cloud.pop();
		
		if (directed)	//Algorithm for directed graph
		{
			//pick the adjacent edges of the current vertex
			for (auto iter : directedEdges[u])
			{
				int adjVertex = iter.first;		//pick the vertex
				int weight = iter.second;		//pick its available weight

				//If the adjacent vertex has already been explored, no need to check its distance again
				if (explored[adjVertex])
					continue;

				//If the adjavent vertex is not yet explored, recalculate its distance if possible with any optimized path
				if (dist[adjVertex] > dist[u] + weight)
				{
					dist[adjVertex] = dist[u] + weight;
					parent[adjVertex] = u;
					cloud.push(make_pair(dist[adjVertex], adjVertex));	//push to the cloud to be picked later
				}
				explored[u] = true;				//mark it as explored, we don't need it to be picked again
			}
		}
		else			//Algorithm for undirected graph
		{
			//pick the adjacent edges of the current vertex
			for (auto iter : unDirectedEdges[u])
			{
				int adjVertex = iter.first;		//pick the vertex
				int weight = iter.second;		//pick its available weight

				//If the adjacent vertex has already been explored, no need to check its distance again
				if (explored[adjVertex])
					continue;

				//If the adjavent vertex is not yet explored, recalculate its distance if possible with any optimized path
				if (dist[adjVertex] > dist[u] + weight)
				{
					dist[adjVertex] = dist[u] + weight;
					parent[adjVertex] = u;
					cloud.push(make_pair(dist[adjVertex], adjVertex));		//push to the cloud to be picked later
				}
				explored[u] = true;				//mark it as explored, we don't need it to be picked again
			}
		}
	}
	cout << "Shortest Paths (Dijkstra's Algorithm)\n";
	cout << "Path Cost		Path \n";
	cout << "----------------------------------------\n";
	for (int i = 0; i < vertices; i++)
	{
		cout << "   " << dist[i] << "\t\t\t";

		if (i == src)
			cout << (char)(src + 'A') << " - ";

		printParentPath(parent, i, src);
		cout << endl;
	}
}

void Graph::minimumSpanningTree(int src)
{
	//Using cloud as a priority Queue helps us to reduce the time complexity
	//priority queue doen't have option to reduce already present node's key, so we are inserting a
	//duplicate key with reduced weight, this way while picking it will pick the minimum dist node
	//dist vector is used to store that particular node dist which is subject to change,
	//later we may revisit if there is any lesser distance than the available one.
	//parent vector is used to keep track of the current node parent, used to print the path
	//Explored vector is used to keep track of already explored nodes.
	//Total Cost variable stores the total minimum spanning tree cost

	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> cloud;
	vector<int> dist(vertices, INT_MAX);
	vector<int> parent(vertices, INT_MAX);
	vector<bool> explored(vertices, false);
	int totalCost = 0;

	src = src - 'A';
	cloud.push(make_pair(0, src));			//push the source node into the cloud
	dist[src] = 0;							//source node distance will be '0' 
	parent[src] = src;						//parent of the source node will not be there

	while (!cloud.empty())					//traverse until there are no nodes left in the cloud
	{
		//pick the top most node, as it is min-heap based priority queue, the least distance node will always
		//be at the top of the heap
		int u = cloud.top().second;
		cloud.pop();

		//pick the adjacent edges of the current vertex
		for (auto iter : unDirectedEdges[u])
		{
			int adjVertex = iter.first;		//pick the vertex
			int weight = iter.second;		//pick its available weight

			//If the adjacent vertex has already been explored, no need to check its distance again
			if (explored[adjVertex])
				continue;

			//If the adjavent vertex is not yet explored, recalculate its distance if possible with any optimized path
			if (dist[adjVertex] > weight)
			{
				dist[adjVertex] = weight;
				parent[adjVertex] = u;
				cloud.push(make_pair(dist[adjVertex], adjVertex));		//push to the cloud to be picked later
			}
			explored[u] = true;				//mark it as explored, we don't need it to be picked again
		}
	}

	cout << "\nPrims Algorithm\n";
	cout << "Edges        Distances\n";
	cout << "----------------------\n";
	for (int i = 0; i < vertices; i++)
	{
		cout << "" << (char)(parent[i] + 'A') << " - " << (char)(i + 'A') << "\t\t" << dist[i] << endl;
		totalCost += dist[i];
	}
	cout << "Total Cost: " << totalCost << endl;
}

int main()
{
	srand(time(NULL));
	ifstream file;

	file.open("input.txt");			//open the input file for reading
	char src = 0;
	char dest, directed;
	int weight, numberOfEdges, srcNode;
	int vertices = 0;

	string line;
	string delimeter = " ";

	//read number of vertices, no of edges & direction of graph
	file >> vertices >> numberOfEdges >> directed;
	Graph g(vertices);

	char* srcEdge = new char[1];
	char* destEdge = new char[1];

	//read all the edges and store it in their corresponding vectors
	for (int counter = 0; counter <= numberOfEdges && getline(file, line); counter++)
	{
		vector<string> tokenizer;
		size_t pos = 0;
		while ((pos = line.find(delimeter)) != string::npos)
		{
			tokenizer.push_back(line.substr(0, pos));
			line.erase(0, pos + delimeter.length());
		}
		if (counter) 
		{
			strcpy(srcEdge, tokenizer[0].c_str());
			strcpy(destEdge, tokenizer[1].c_str());
			if(directed == 'D')
				g.addEdge(srcEdge[0], destEdge[0], stoi(line), true);
			else
				g.addEdge(srcEdge[0], destEdge[0], stoi(line), false);
		}
		tokenizer.clear();
	}

	//read the source vertex if available
	file >> src;
	file.close();

	if (!src)		//If the source vertex info is unavailable, it reaches here
	{
		//If the input is a Directed Graph, ask user to input source vertex and try again, otherwise if we proceed
		//it may causes exception, becoz from a particular node we may not reach all nodes 
		if (directed == 'D')
		{
			cout << "Choose the source node & run again!!!" << endl;
			return 0;
		}

		//If source vertes is not mentioned, choose it randomly from the given vertices
		cout << "Source node isn't mentioned!!! Choosing source node randomly ";
		srcNode = rand() % vertices;
		srcNode = srcNode + 'A';
	}
	else
		srcNode = src;

	cout << "src: " << (char)srcNode << endl << endl;

	if (directed == 'U')		//If it is undirected graph, call both shortest paths & minumum spanning tree algorithms
	{
		g.shortestPath(srcNode, false);
		g.minimumSpanningTree(srcNode);
	}
	else if(directed == 'D')	//Else if it is a directed graph, call only shortest paths algorithm
		g.shortestPath(srcNode, true);
}
