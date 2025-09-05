#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <set>

using namespace std;

class TemporalGraph
{
private:
    set<string> setOfNodes;

public:
    unordered_map<string, unordered_map<string, vector<int>>> graph;

    TemporalGraph(vector<vector<string>> edgesList, vector<vector<int>> timestampsList)
    {
        for (int i = 0; i < edgesList.size(); i++)
        {
            string startNode = edgesList[i][0];
            string endNode = edgesList[i][1];
            this->setOfNodes.insert(startNode);
            this->setOfNodes.insert(endNode);

            this->graph[startNode][endNode] = timestampsList[i];
            this->graph[endNode][startNode] = timestampsList[i];
        }
    }

    void printGraph()
    {
        for (const auto &[from, neighbors] : graph)
        {
            cout << from << " :" << endl;
            for (const auto &[to, times] : neighbors)
            {
                cout << "   -> " << to << " : ";
                for (int t : times)
                    cout << t << ", ";
                cout << endl;
            }
        }
    }

    void addNode(string newNode, vector<string> neighbours, vector<vector<int>> timestamps)
    {

        this->setOfNodes.insert(newNode);

        for (int i = 0; i < neighbours.size(); i++)
        {
            this->graph[newNode][neighbours[i]] = timestamps[i];
            this->graph[neighbours[i]][newNode] = timestamps[i];
        }
    }

    void removeNode(string delNode)
    {
        this->setOfNodes.erase(delNode);
        vector<string> neighbourToDel;

        for (auto [neighbour, _] : this->graph[delNode])
        {
            neighbourToDel.push_back(neighbour);
        }

        for (string neighbour : neighbourToDel)
        {
            this->graph[neighbour].erase(delNode);
        }
        this->graph.erase(delNode);
    }

    void addEdge(string newStart, string newEnd, vector<int> timestamps)
    {
        if (this->setOfNodes.find(newStart) == this->setOfNodes.end())
        {
            this->addNode(newStart, {newEnd}, {timestamps});
        }
        else if (this->setOfNodes.find(newEnd) == this->setOfNodes.end())
        {
            this->addNode(newEnd, {newStart}, {timestamps});
        }
        else
        {
            this->graph[newStart][newEnd] = timestamps;
            this->graph[newEnd][newStart] = timestamps;
        }
    }

    void removeEdge(string newStart, string newEnd)
    {
        this->graph[newStart].erase(newEnd);
        this->graph[newEnd].erase(newStart);
    }

    bool existEdge(string start, string end)
    {
        if (this->graph[start].find(end) == this->graph[start].end())
        {
            return false;
        }
        return true;
    }
};

int main()
{
    vector<vector<string>> edges = {{"A", "B"},
                                    {"B", "C"},
                                    {"C", "A"},
                                    {"A", "D"}};
    vector<vector<int>> timestamps = {{1},
                                      {2, 4},
                                      {3},
                                      {4}};

    TemporalGraph g(edges, timestamps);

    g.addNode("F", {"A", "B"}, {{3000, 200}, {40000, 400404}});
    g.printGraph();
    g.removeNode("F");
    cout << "--------------" << endl;

    g.printGraph();
    cout << "--------------" << endl;

    g.addEdge("B", "D", {100000000});
    g.printGraph();
    cout << "--------------" << endl;

    g.removeEdge("B", "D");
    g.printGraph();
    cout << "--------------" << endl;

    cout << g.existEdge("A", "B") << "||||" << g.existEdge("D", "C") << endl;
}