#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#endif

using namespace std;
using namespace std::chrono;

class BlossomAlgorithm {
public:
    vector<string> readSizes() {
        vector<string> sizes;
        ifstream file("sizes.txt");
        string line;
        while (getline(file, line)) sizes.push_back(line);
        return sizes;
    }

    vector<vector<int>> readGraph(const string& filename, int& maxVertex) {
        ifstream file(filename);
        vector<pair<int, int>> edges;
        maxVertex = 0;

        string line;
        while (getline(file, line)) {
            size_t space = line.find(' ');
            if (space == string::npos) continue;

            int u = stoi(line.substr(0, space));
            int v = stoi(line.substr(space + 1));
            edges.push_back({u, v});
            maxVertex = max(maxVertex, max(u, v));
        }

        vector<vector<int>> graph(maxVertex + 1, vector<int>(maxVertex + 1, 0));
        for (auto& e : edges) {
            graph[e.first][e.second] = 1;
            graph[e.second][e.first] = 1;
        }

        return graph; // Adicionado retorno faltante
    }

    int findMaxMatching(const vector<vector<int>>& graph, int instance) {
        int n = graph.size();
        vector<int> match(n, -1);
        int count = 0;

        for (int u = 0; u < n; ++u) {
            vector<bool> vis(n, false);
            if (dfs(u, vis, graph, match)) count++;
        }

        string dir = "maxGraphs";
        #ifdef _WIN32
        _mkdir(dir.c_str());
        #else 
        mkdir(dir.c_str(), 0777);
        #endif

        ofstream file(dir + "/subGraph" + to_string(instance) + ".txt");
        for (size_t i = 0; i < match.size(); ++i) { // size_t para i
            if (match[i] > static_cast<int>(i)) {
                file << i << " - " << match[i] << endl;
            }
        }

        return count;
    }

private:
    bool dfs(int u, vector<bool>& vis, const vector<vector<int>>& graph, vector<int>& match) {
        for (size_t v = 0; v < graph.size(); ++v) { // size_t para v
            if (graph[u][v] && !vis[v]) {
                vis[v] = true;
                if (match[v] == -1 || dfs(match[v], vis, graph, match)) {
                    match[v] = u;
                    return true;
                }
            }
        }
        return false;
    }
};

int main() {
    BlossomAlgorithm solver;
    auto sizes = solver.readSizes();
    const int total = 200;
    vector<int> graphSizes(total);
    vector<int> matches(total);
    vector<long long> times(total);
    long long totalTime = 0;

    for (int i = 0; i < total; ++i) {
        string file = "testGraphs/graph" + to_string(i + 1) + ".txt";
        int maxVertex;
        auto graph = solver.readGraph(file, maxVertex);

        auto start = high_resolution_clock::now();
        int match = solver.findMaxMatching(graph, i);
        auto end = high_resolution_clock::now();

        auto duration = duration_cast<milliseconds>(end - start).count();

        graphSizes[i] = i < static_cast<int>(sizes.size()) ? stoi(sizes[i]) : 0;
        matches[i] = match / 2;
        times[i] = duration;
        totalTime += duration;

        cout << "[## " << (i+1) << "/" << total << " ##] Size: " 
             << graphSizes[i] << " Match: " << matches[i] 
             << " Time: " << duration << "ms\n";
    }

    ofstream result("results.txt");
    result << "Total time: " << totalTime << "ms\nSizes: ";
    for (auto s : graphSizes) result << s << " ";
    result << "\nMatches: ";
    for (auto m : matches) result << m << " ";
    result << "\nTimes: ";
    for (auto t : times) result << t << " ";

    return 0;
}