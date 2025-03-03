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

// Função para gerar um grafo aleatório com um número específico de vértices e arestas
vector<pair<int, int>> gerarGrafo(int vertices, int arestas) {
    vector<pair<int, int>> listaArestas;
    if (vertices <= 0 || arestas <= 0) return listaArestas;

    set<pair<int, int>> conjuntoArestas;
    int maxArestas = vertices * (vertices - 1) / 2;
    arestas = min(arestas, maxArestas);

    while (listaArestas.size() < static_cast<size_t>(arestas)){
        int u = rand() % vertices;
        int v = rand() % vertices;

        if (u == v) continue;
        if (u > v) swap(u, v);

        if (!conjuntoArestas.count({u, v})) {
            conjuntoArestas.insert({u, v});
            listaArestas.push_back({u, v});
        }
    }

    return listaArestas;
}

int main() {
    srand(time(0));
    const int instancias = 200;
    const string diretorio = "grafosTeste";

    // Criação de diretório cross-platform
    #ifdef _WIN32
    _mkdir(diretorio.c_str());
    #else 
    mkdir(diretorio.c_str(), 0777);
    #endif

    ofstream arquivoTamanhos("tamanhos.txt");
    ofstream arquivoResultados("resultados.txt");

    for (int i = 0; i < instancias; ++i) {
        int vertices = rand() % 19999 + 1; // 1-20000
        int arestas = rand() % 20000;

        int maxArestas = vertices * (vertices - 1) / 2;
        arestas = min(arestas, maxArestas);

        auto grafo = gerarGrafo(vertices, arestas);

        string nomeArquivo = diretorio + "/grafo" + to_string(i + 1) + ".txt";
        ofstream arquivo(nomeArquivo);
        for (auto& aresta : grafo) {
            arquivo << aresta.first << " " << aresta.second << endl;
        }
        arquivo.close();

        arquivoTamanhos << (vertices + arestas) << endl;
        arquivoResultados << "Grafo " << i << ": " << vertices << "x" << arestas << " | ";
    }

    return 0;
}