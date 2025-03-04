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

// Struct para representar um grafo usando lista de adjacência
struct Grafo {
    int vertices;
    vector<vector<int>> listaAdjacencia;
};

// Função para gerar um grafo aleatório com um número específico de vértices e arestas
Grafo gerarGrafo(int vertices, int arestas) {
    Grafo grafo;
    grafo.vertices = vertices;
    grafo.listaAdjacencia.resize(vertices);

    set<pair<int, int>> conjuntoArestas;
    int maxArestas = vertices * (vertices - 1) / 2;
    arestas = min(arestas, maxArestas);

    while (conjuntoArestas.size() < static_cast<size_t>(arestas)) {
        int u = rand() % vertices;
        int v = rand() % vertices;

        if (u == v) continue;
        if (u > v) swap(u, v);

        if (!conjuntoArestas.count({u, v})) {
            conjuntoArestas.insert({u, v});
            grafo.listaAdjacencia[u].push_back(v);
            grafo.listaAdjacencia[v].push_back(u);
        }
    }

    return grafo;
}

// Função para salvar todos os grafos em um único arquivo
void salvarGrafos(const vector<Grafo>& grafos, const string& nomeArquivo) {
    ofstream arquivo(nomeArquivo);
    for (size_t i = 0; i < grafos.size(); ++i) {
        arquivo << "Grafo " << i << ":\n";  // Começa em 0
        arquivo << "Vertices: " << grafos[i].vertices << "\n";
        arquivo << "Arestas:\n";
        for (int u = 0; u < grafos[i].vertices; ++u) {
            for (int v : grafos[i].listaAdjacencia[u]) {
                if (u < v) {  // Para evitar duplicação
                    arquivo << u << " " << v << "\n";
                }
            }
        }
        arquivo << "\n";
    }
}

int main() {
    srand(time(0));
    const int instancias = 200;
    const string diretorio = "Grafos";  // Pasta onde os arquivos serão salvos

    // Criação de diretório cross-platform
    #ifdef _WIN32
    _mkdir(diretorio.c_str());
    #else 
    mkdir(diretorio.c_str(), 0777);
    #endif

    vector<Grafo> grafos;
    for (int i = 0; i < instancias; ++i) {
        int vertices = rand() % 19999 + 1; // 1-20000
        int arestas = rand() % 20000;

        Grafo grafo = gerarGrafo(vertices, arestas);
        grafos.push_back(grafo);
    }

    // Salvar grafos na pasta "Grafos"
    salvarGrafos(grafos, diretorio + "/grafos.txt");

    cout << "Grafos gerados e salvos em " << diretorio << "/grafos.txt" << endl;

    return 0;
}