#include "../lib/temporalStructures.h"

int main()
{

    vector<Edge> edges = {
        {"s", "a", {1}},
        {"s", "e", {5}},
        {"b", "a", {6}},
        {"b", "c", {6}},
        {"a", "d", {2}},
        {"d", "c", {4}},
        {"b", "d", {5}},
        {"d", "s", {7}},
        {"d", "e", {5}},
        {"d", "f", {8}},
        {"f", "e", {2}},
        {"c", "t", {4}},
        {"f", "t", {10}},
    };

    TemporalGraph g(edges);

    unordered_map<string, int> map = g.earliestTime("s");
    for (const auto &[node, earliest] : map)
    {
        cout << "node: " << node << " earliest time: " << earliest << endl;
    }

    TemporalTree eaTree = g.earliestTimeTree("s");
    eaTree.printTree();

    cout << "------------------" << endl;

    unordered_map<string, int> ldMap = g.latestDeparture("s");
    for (const auto &[node, latest] : ldMap)
    {
        cout << "node: " << node << " latest time: " << latest << endl;
    }

    TemporalTree ldTree = g.latestDepartureTree("s");
    ldTree.printTree();
}