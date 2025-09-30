#include "temporalStructures.h"

using namespace std;

Edge::Edge(string start, string end, vector<int> timestamps)
{
    this->start = start;
    this->end = end;
    this->timestamps = timestamps;
}

Edge::Edge(string start, string end)
{
    this->start = start;
    this->end = end;
    this->timestamps = {};
}

TemporalTree::TemporalTree(string root)
{
    this->root = root;
    this->setOfNodes.insert(root);
}

TemporalTree::TemporalTree(vector<Edge> edges)
{
    this->root = edges[0].start;
    for (const auto &[start, end, timestamps] : edges)
    {
        this->tree[start][end] = timestamps;
        this->fatherMap[end] = start;
        this->setOfNodes.insert(end);
    }
}

void TemporalTree::printTreeHelper(const string &node, string prefix, bool isLast)
{
    cout << prefix;

    if (!prefix.empty())
    {
        cout << (isLast ? "\u2514\u2500\u2500 " : "\u251c\u2500\u2500 ");
    }

    cout << node;

    // se il nodo ha figli, mostro anche i timestamp degli archi
    if (tree.find(node) != tree.end())
    {
        cout << " (";
        bool first = true;
        for (auto &[child, timestamps] : tree[node])
        {
            if (!first)
                cout << ", ";
            cout << child << ": [";
            for (size_t i = 0; i < timestamps.size(); i++)
            {
                cout << timestamps[i];
                if (i < timestamps.size() - 1)
                    cout << ", ";
            }
            cout << "]";
            first = false;
        }
        cout << ")";
    }

    cout << endl;

    // ricorsione sui figli
    if (tree.find(node) != tree.end())
    {
        auto &children = tree[node];
        size_t i = 0, n = children.size();
        for (auto &[child, _] : children)
        {
            printTreeHelper(child, prefix + (isLast ? "    " : "\u2502   "), i == n - 1);
            i++;
        }
    }
}

void TemporalTree::printTree()
{
    cout << "Temporal Tree (root = " << root << ")" << endl;
    printTreeHelper(root, "", true);
}

void TemporalTree::addEdge(Edge newEdge)
{
    this->setOfNodes.insert(newEdge.end);
    this->fatherMap[newEdge.end] = newEdge.start;
    this->tree[newEdge.start][newEdge.end] = newEdge.timestamps;
}

void TemporalTree::removeNode(string nodeToDelete)
{
    if (this->tree[nodeToDelete].size() != 0)
    {
        vector<string> children;
        for (const auto &[childToDelete, _] : this->tree[nodeToDelete])
        {
            children.push_back(childToDelete);
        }
        for (string childToDelete : children)
        {
            removeNode(childToDelete);
        }
    }
    this->setOfNodes.erase(nodeToDelete);
    string father = this->fatherMap[nodeToDelete];
    this->tree[father].erase(nodeToDelete);
}

vector<Edge> TemporalGraph::edgeStream(string source, bool reverse)
{
    set<vector<string>> visited;
    vector<Edge> stream;

    vector<Edge> stack;

    for (const auto &[end, timestamp] : this->graph[source])
    {
        stack.push_back({source, end, timestamp});
    }

    while (stack.size() != 0)
    {
        Edge currentEdge = stack[stack.size() - 1];
        stack.pop_back();

        if (visited.find({currentEdge.start, currentEdge.end}) == visited.end())
        {
            visited.insert({currentEdge.start, currentEdge.end});
            for (int i = 0; i < currentEdge.timestamps.size(); i++)
            {
                stream.push_back({currentEdge.start, currentEdge.end, {currentEdge.timestamps[i]}});
            }

            for (const auto &[neighbour, timestamp] : this->graph[currentEdge.end])
            {
                stack.push_back({currentEdge.end, neighbour, timestamp});
            }
        }
    }
    if (!reverse)
    {
        stable_sort(stream.begin(), stream.end(),
                    [](const Edge &a, const Edge &b)
                    {
                        return a.timestamps[0] < b.timestamps[0];
                    });
    }
    else
    {
        stable_sort(stream.begin(), stream.end(),
                    [](const Edge &a, const Edge &b)
                    {
                        return a.timestamps[0] > b.timestamps[0];
                    });
    }

    return stream;
}

TemporalGraph::TemporalGraph(vector<Edge> edgesList)
{
    for (int i = 0; i < edgesList.size(); i++)
    {
        string startNode = edgesList[i].start;
        string endNode = edgesList[i].end;
        this->setOfNodes.insert(startNode);
        this->setOfNodes.insert(endNode);

        this->graph[startNode][endNode] = edgesList[i].timestamps;
        this->graph[endNode][startNode] = edgesList[i].timestamps;
    }
}

void TemporalGraph::printGraph()
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

void TemporalGraph::addNode(string newNode, vector<string> neighbours, vector<vector<int>> timestamps)
{

    this->setOfNodes.insert(newNode);

    for (int i = 0; i < neighbours.size(); i++)
    {
        this->graph[newNode][neighbours[i]] = timestamps[i];
        this->graph[neighbours[i]][newNode] = timestamps[i];
    }
}

void TemporalGraph::removeNode(string delNode)
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

void TemporalGraph::addEdge(Edge newEdge)
{
    string newStart = newEdge.start;
    string newEnd = newEdge.end;
    vector<int> timestamps = newEdge.timestamps;

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

void TemporalGraph::removeEdge(Edge edgeToDel)
{
    string startToDel = edgeToDel.start;
    string endToDel = edgeToDel.end;
    this->graph[startToDel].erase(endToDel);
    this->graph[endToDel].erase(startToDel);
}

bool TemporalGraph::existEdge(string start, string end)
{
    if (this->graph[start].find(end) == this->graph[start].end())
    {
        return false;
    }
    return true;
}

unordered_map<string, int> TemporalGraph::earliestTime(string source)
{
    unordered_map<string, int> eaMap;
    vector<Edge> stream = this->edgeStream(source, false);

    for (string node : this->setOfNodes)
    {
        eaMap[node] = numeric_limits<int>::max();
    }

    eaMap[source] = numeric_limits<int>::min();

    for (int i = 0; i < 2; i++)
    {
        for (const Edge edge : stream)
        {
            string start = edge.start;
            string end = edge.end;
            int timestamp = edge.timestamps[0];

            if (timestamp >= eaMap[start])
            {
                if (timestamp < eaMap[end])
                {
                    eaMap[end] = timestamp;
                }
            }
        }
    }

    return eaMap;
}

TemporalTree TemporalGraph::earliestTimeTree(string source)
{
    unordered_map<string, int> eaMap;
    vector<Edge> stream = this->edgeStream(source, false);
    TemporalTree eaTree(source);

    for (string node : this->setOfNodes)
    {
        eaMap[node] = numeric_limits<int>::max();
    }

    eaMap[source] = numeric_limits<int>::min();

    for (int i = 0; i < 2; i++)
    {
        for (const Edge edge : stream)
        {
            string start = edge.start;
            string end = edge.end;
            int timestamp = edge.timestamps[0];

            if (timestamp >= eaMap[start])
            {
                if (timestamp < eaMap[end])
                {
                    eaMap[end] = timestamp;
                    eaTree.addEdge({start, end, {timestamp}});
                }
            }
        }
    }

    return eaTree;
}

unordered_map<string, int> TemporalGraph::latestDeparture(string source)
{
    unordered_map<string, int> ldMap;
    vector<Edge> stream = this->edgeStream(source, true);

    for (string node : this->setOfNodes)
    {
        ldMap[node] = numeric_limits<int>::min();
    }

    ldMap[source] = numeric_limits<int>::max();

    for (int i = 0; i < 2; i++) // va fatto 2 volte poichè gli archi sono tutti duplicati e se non lo facessi non si ha un'ordine di visita prestabilito perdo informazioni
    {
        for (const Edge edge : stream)
        {
            string start = edge.start;
            string end = edge.end;
            int timestamp = edge.timestamps[0];

            if (timestamp <= ldMap[end])
            {
                if (timestamp >= ldMap[start])
                {
                    ldMap[start] = timestamp;
                }
            }
        }
    }

    return ldMap;
}

TemporalTree TemporalGraph::latestDepartureTree(string destination)
{
    unordered_map<string, int> ldMap;
    vector<Edge> stream = this->edgeStream(destination, true);
    TemporalTree ldTree(destination);

    for (string node : this->setOfNodes)
    {
        ldMap[node] = numeric_limits<int>::min();
    }

    ldMap[destination] = numeric_limits<int>::max();

    for (int i = 0; i < 2; i++) // va fatto 2 volte poichè gli archi sono tutti duplicati e se non lo facessi non si ha un'ordine di visita prestabilito perdo informazioni
    {
        for (const Edge edge : stream)
        {
            string start = edge.start;
            string end = edge.end;
            int timestamp = edge.timestamps[0];

            if (timestamp <= ldMap[end])
            {
                if (timestamp >= ldMap[start])
                {
                    ldMap[start] = timestamp;
                    ldTree.addEdge({end, start, {timestamp}});
                }
            }
        }
    }

    return ldTree;
}