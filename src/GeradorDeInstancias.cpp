#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <set>
#include <utility>
#include <string>
#include <algorithm>
#include <sys/stat.h>  // Para sistemas Unix/Linux
#ifdef _WIN32
#include <direct.h>    // Para Windows
#endif

using namespace std;

vector<pair<int, int>> generateGraph(int vertices, int edges) {
    vector<pair<int, int>> edgeList;
    if (vertices <= 0 || edges <= 0) return edgeList;

    set<pair<int, int>> edgeSet;
    int maxEdges = vertices * (vertices - 1) / 2;
    edges = min(edges, maxEdges);

    while (edgeList.size() < static_cast<size_t>(edges)){
        int u = rand() % vertices;
        int v = rand() % vertices;

        if (u == v) continue;
        if (u > v) swap(u, v);

        if (!edgeSet.count({u, v})) {
            edgeSet.insert({u, v});
            edgeList.push_back({u, v});
        }
    }

    return edgeList;
}

int main() {
    srand(time(0));
    const int instances = 200;
    const string directory = "testGraphs";

    // Criação de diretório cross-platform
    #ifdef _WIN32
    _mkdir(directory.c_str());
    #else 
    mkdir(directory.c_str(), 0777);
    #endif

    ofstream sizesFile("sizes.txt");
    ofstream resultsFile("results.txt");

    for (int i = 0; i < instances; ++i) {
        int vertices = rand() % 19999 + 1; // 1-20000
        int edges = rand() % 20000;

        int maxEdges = vertices * (vertices - 1) / 2;
        edges = min(edges, maxEdges);

        auto graph = generateGraph(vertices, edges);

        string filename = directory + "/graph" + to_string(i + 1) + ".txt";
        ofstream file(filename);
        for (auto& edge : graph) {
            file << edge.first << " " << edge.second << endl;
        }
        file.close();

        sizesFile << (vertices + edges) << endl;
        resultsFile << "Grafo " << i << ": " << vertices << "x" << edges << " | ";
    }

    return 0;
}