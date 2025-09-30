#include "../lib/temporalStructures.h"
#include "array"

using namespace std;

void printMap(const unordered_map<string, vector<array<int, 2>>> &myMap)
{
    for (const auto &[key, vec] : myMap)
    { // C++17 structured binding
        cout << key << ": [";
        for (size_t i = 0; i < vec.size(); ++i)
        {
            cout << "[" << vec[i][0] << ", " << vec[i][1] << "]";
            if (i + 1 < vec.size())
                cout << ", ";
        }
        cout << "]\n";
    }
}

unordered_map<string, vector<array<int, 2>>> windowAlgorithmEa(string source, TemporalGraph g)
{
    vector<Edge> stream = g.edgeStream(source, false);
    unordered_map<string, vector<array<int, 2>>> eaMap;

    for (string node : g.setOfNodes)
    {
        if (node == source)
        {
            eaMap[node] = {{0, 0}};
        }
        else
        {
            eaMap[node] = {{0, numeric_limits<int>::max()}};
        }
    }

    int level = 0;

    for (const auto &[start, end, timestamp] : stream)
    {
        if (start == source && timestamp[0] > eaMap[source].back()[1])
        {
            level += 1;
            eaMap[source].push_back(array<int, 2>{level, timestamp[0]});
        }

        auto [lvStart, eaStart] = eaMap[start].back();
        auto [lvEnd, eaEnd] = eaMap[end].back();

        if (lvStart > lvEnd && lvStart != 0 && eaStart <= timestamp[0])
        {
            eaMap[end].push_back({lvStart, timestamp[0]});
        }
        else if (lvStart == lvEnd && lvStart != 0 && eaStart <= timestamp[0] && eaEnd > timestamp[0])
        {
            eaMap[end].pop_back();
            eaMap[end].push_back({lvEnd, timestamp[0]});
        }
    }

    return eaMap;
}

unordered_map<string, vector<array<int, 2>>> windowAlgorithmLd(string source, TemporalGraph g)
{
    vector<Edge> stream = g.edgeStream(source, true);
    unordered_map<string, vector<array<int, 2>>> ldMap;

    for (string node : g.setOfNodes)
    {
        if (node == source)
        {
            ldMap[node] = {{0, numeric_limits<int>::max()}};
        }
        else
        {
            ldMap[node] = {{0, numeric_limits<int>::min()}};
        }
    }

    int level = 0;

    for (const auto &[start, end, timestamp] : stream)
    {
        if (start == source && timestamp[0] < ldMap[source].back()[1])
        {
            level += 1;
            ldMap[source].push_back(array<int, 2>{level, timestamp[0]});
        }

        auto [lvStart, ldStart] = ldMap[start].back();
        auto [lvEnd, ldEnd] = ldMap[end].back();

        if (lvStart > lvEnd && lvStart != 0 && timestamp[0] <= ldStart)
        {
            ldMap[end].push_back({lvStart, timestamp[0]});
        }
        else if (lvStart == lvEnd && lvStart != 0 && ldStart <= timestamp[0] && timestamp[0] > ldEnd)
        {
            ldMap[end].pop_back();
            ldMap[end].push_back({lvEnd, timestamp[0]});
        }
    }

    return ldMap;
}

int main()
{
    TemporalGraph g({
        {"A", "B", {2, 5}},
        {"C", "B", {1, 5}},
        {"C", "H", {3, 4}},
        {"D", "B", {2, 7}},
        {"D", "E", {9}},
        {"E", "H", {7, 8}},
        {"E", "F", {1, 8}},
        {"G", "F", {5, 6}},
        {"G", "D", {3, 4}},
        {"H", "alfa", {0}},
    });

    TemporalGraph t({
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
    });

    unordered_map<string, vector<array<int, 2>>>
        gMap = windowAlgorithmEa("D", g);

    unordered_map<string, vector<array<int, 2>>>
        tMap = windowAlgorithmEa("s", t);

    // printMap(tMap);

    unordered_map<string, vector<array<int, 2>>>
        ldGMap = windowAlgorithmLd("D", g);

    unordered_map<string, vector<array<int, 2>>>
        ldTMap = windowAlgorithmLd("s", t);

    printMap(ldGMap);
}