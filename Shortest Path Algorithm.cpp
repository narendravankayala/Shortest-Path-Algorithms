#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <list>

using namespace std;

class Graph {
	int vertices;
	list<pair<int, int>> *edges;

public:
	Graph(int vertices);
	void addEdge(int u, int v, int weight);

	void shortestPath(int s);
	void minimumSpanningTree(int s);
};

Graph::Graph(int V)
{
	this->vertices = V;
	edges = new list<pair<int, int>>[vertices];
}

void Graph::addEdge(int u, int v, int weight)
{
	u = u - 'A';
	v = v - 'A';
	edges[u].push_back(make_pair(v, weight));
	edges[v].push_back(make_pair(u, weight));
}

void Graph::shortestPath(int src)
{
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> cloud;
	vector<int> dist(vertices, INT_MAX);

	src = src - 'A';
	cloud.push(make_pair(0, src));
	dist[src] = 0;
	while (!cloud.empty())
	{
		int u = cloud.top().second;
		cloud.pop();

		for (auto iter : edges[u])
		{
			int adjVertex = iter.first;
			int weight = iter.second;

			if (dist[adjVertex] > dist[u] + weight)
			{
				dist[adjVertex] = dist[u] + weight;
				cloud.push(make_pair(dist[adjVertex], adjVertex));
			}
		}
	}
	cout << "Shortest Paths (Dijkstra's Algorithm)\n";
	cout << "vertices distance from source \n";
	cout << "-----------------------------\n";
	for (int i = 0; i < vertices; i++)
		cout << "    " << (char)(i + 'A') << "\t\t" << dist[i] << endl;
}

void Graph::minimumSpanningTree(int src)
{
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> cloud;
	vector<int> dist(vertices, INT_MAX);
	vector<int> parent(vertices, INT_MAX);

	src = src - 'A';
	cloud.push(make_pair(0, src));
	dist[src] = 0;
	parent[src] = 0;
	while (!cloud.empty())
	{
		int u = cloud.top().second;
		cloud.pop();

		for (auto iter : edges[u])
		{
			int adjVertex = iter.first;
			int weight = iter.second;

			if (dist[adjVertex] > weight)
			{
				dist[adjVertex] = weight;
				parent[adjVertex] = u;
				cloud.push(make_pair(dist[adjVertex], adjVertex));
			}
		}
	}

	cout << "\nPrims Algorithm\n";
	cout << "Edges        Distances\n";
	cout << "----------------------\n";
	for (int i = 0; i < vertices; i++)
		cout << "" << (char)(parent[i] + 'A') << " - " << (char)(i + 'A') << "\t\t" << dist[i] << endl;
}

int main()
{
	ifstream file;
	file.open("input.txt");
	char src, dest;
	int weight, edge;
	int vertices = 0;

	string line;
	string delimeter = " ";

	file >> vertices >> edge;
	Graph g(vertices);

	char* srcEdge = new char[1];
	char* destEdge = new char[1];
	for (int counter = 0; counter <= edge && getline(file, line); counter++)
	{
		vector<string> tokenizer;
		size_t pos = 0;
		string token;
		while ((pos = line.find(delimeter)) != string::npos)
		{
			tokenizer.push_back(line.substr(0, pos));
			line.erase(0, pos + delimeter.length());
		}
		if (counter) 
		{
			strcpy(srcEdge, tokenizer[0].c_str());
			strcpy(destEdge, tokenizer[1].c_str());
			g.addEdge(srcEdge[0], destEdge[0], stoi(line));
		}
		tokenizer.clear();
	}

	file >> src;
	file.close();

	g.shortestPath(src);
	g.minimumSpanningTree(src);
}
