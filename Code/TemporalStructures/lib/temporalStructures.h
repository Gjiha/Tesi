#ifndef TEMPORALSTRUCTURES_H
#define TEMPORALSTRUCTURES_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <set>
#include <utility>
#include <limits>
#include <algorithm>

using namespace std;

struct Edge
{
    string start;
    string end;
    vector<int> timestamps;

    Edge(string start, string end, vector<int> timestamps);
    Edge(string start, string end);
};

class TemporalTree
{

public:
    set<string> setOfNodes;
    unordered_map<string, unordered_map<string, vector<int>>> tree;
    unordered_map<string, string> fatherMap;
    string root;

    TemporalTree(string root);
    TemporalTree(vector<Edge> edges);

    void addEdge(Edge newEdge);
    void removeNode(string nodeToDelete);

    void printTreeHelper(const string &node, string prefix, bool isLast);
    void printTree();
};

class TemporalGraph
{

public:
    set<string> setOfNodes;
    unordered_map<string, unordered_map<string, vector<int>>> graph;

    TemporalGraph(vector<Edge> edgesList);

    void printGraph();
    void addNode(string newNode, vector<string> neighbours, vector<vector<int>> timestamps);
    void removeNode(string delNode);
    void addEdge(Edge newEdge);
    void removeEdge(Edge edgeToDel);
    bool existEdge(string start, string end);

    vector<Edge> edgeStream(string source, bool reverse);

    unordered_map<string, int> earliestTime(string source);
    unordered_map<string, int> latestDeparture(string source);

    TemporalTree earliestTimeTree(string source);
    TemporalTree latestDepartureTree(string destination);
};

#endif
