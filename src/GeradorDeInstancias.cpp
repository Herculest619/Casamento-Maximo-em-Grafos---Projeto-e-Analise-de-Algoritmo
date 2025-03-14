// Hércules Aparecido Teixeira - 18.2.8072

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <set>
#include <utility>
#include <string>
#include <algorithm>
#include <random>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#endif

using namespace std;

int instancias = 500; // Número de instâncias a serem geradas
int nodes = 200000; // Número máximo de vértices permitido

struct Grafo {
    int vertices;
    vector<vector<int>> listaAdjacencia;
};

// Função para gerar um grafo aleatório
Grafo gerarGrafo(int vertices, int arestas, mt19937& gen) {
    Grafo grafo;
    grafo.vertices = vertices;
    grafo.listaAdjacencia.resize(vertices);

    if (vertices <= 1) {
        return grafo; // Sem arestas se vertices for 0 ou 1
    }

    set<pair<int, int>> arestas_geradas;
    uniform_int_distribution<int> distrib(0, vertices - 1);

    while ((int)arestas_geradas.size() < arestas) {
        int u = distrib(gen);
        int v = distrib(gen);
        if (u == v) continue; // Ignora laços
        if (u > v) swap(u, v); // Garante u < v para evitar duplicatas
        pair<int, int> aresta(u, v);
        if (arestas_geradas.insert(aresta).second) {
            grafo.listaAdjacencia[u].push_back(v);
            grafo.listaAdjacencia[v].push_back(u);
        }
    }

    return grafo;
}

// Função para salvar um grafo em um arquivo
void salvarGrafoIndividual(const Grafo& grafo, int index, const string& diretorio) {
    string nomeArquivo = diretorio + "/grafo_" + to_string(index) + ".txt";
    ofstream arquivo(nomeArquivo);
    arquivo << "Vertices: " << grafo.vertices << "\n";
    arquivo << "Arestas:\n";
    for (int u = 0; u < grafo.vertices; ++u) {
        for (int v : grafo.listaAdjacencia[u]) {
            if (u < v) { // Garante que cada aresta seja escrita uma vez
                arquivo << u << " " << v << "\n";
            }
        }
    }
    arquivo.close();
}

int main() {
    // Usa o tempo atual como semente para o gerador de números aleatórios
    unsigned seed = std::time(nullptr);
    mt19937 gen(seed); // Inicializa o gerador com a semente baseada no tempo

    uniform_int_distribution<int> distribVertices(1, nodes);
    const string diretorio = "Grafos";

    #ifdef _WIN32
    _mkdir(diretorio.c_str());
    #else 
    mkdir(diretorio.c_str(), 0777);
    #endif

    clock_t start = clock();

    // Gera os grafos e salva em arquivos
    for (int i = 0; i < instancias; ++i) {
        int vertices = distribVertices(gen);

        // Arestas aleatórias entre 0 e vertices - 1
        uniform_int_distribution<int> distribArestas(0, vertices - 1);
        int arestas = distribArestas(gen);

        Grafo grafo = gerarGrafo(vertices, arestas, gen);
        salvarGrafoIndividual(grafo, i, diretorio);

        cout << "Grafo " << i << ": " << vertices << " vertices e " << arestas << " arestas\n";
    }

    clock_t end = clock();
    double tempo = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    cout << "Tempo total: " << tempo << " segundos\n\n";

    return 0;
}