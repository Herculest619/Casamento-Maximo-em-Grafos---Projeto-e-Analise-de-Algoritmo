#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <set>
#include <utility>
#include <string>
#include <algorithm>
#include <random> // Para geração de números aleatórios de alta qualidade
#include <sys/stat.h>  // Para sistemas Unix/Linux
#ifdef _WIN32
#include <direct.h>    // Para Windows
#endif

using namespace std;

// Variáveis globais
int instancias = 100;
int nodes = 10000; // Número máximo de vértices e arestas(Estava inutilizando o uso de ((n(n-1))/2) para o número de arestas)

// Struct para representar um grafo usando lista de adjacência
struct Grafo {
    int vertices;
    vector<vector<int>> listaAdjacencia;
};

// Função para gerar um grafo aleatório com um número específico de vértices e arestas
Grafo gerarGrafo(int vertices, int arestas, mt19937& gen) {
    Grafo grafo;
    grafo.vertices = vertices;
    grafo.listaAdjacencia.resize(vertices);

    set<pair<int, int>> conjuntoArestas;
    uniform_int_distribution<int> distrib(0, vertices - 1); // Distribuição uniforme para vértices

    while (conjuntoArestas.size() < static_cast<size_t>(arestas)) {
        int u = distrib(gen);
        int v = distrib(gen);

        if (u == v) continue; // Evita laços (arestas do tipo u-u)
        if (u > v) swap(u, v); // Garante que u < v para evitar duplicação

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
    // Inicialização do gerador de números aleatórios
    random_device rd; // Dispositivo de entropia para semente
    mt19937 gen(rd()); // Gerador Mersenne Twister
    uniform_int_distribution<int> distribVertices(1, nodes); // Distribuição para vértices

    const string diretorio = "Grafos";  // Pasta onde os arquivos serão salvos

    // Criação de diretório cross-platform
    #ifdef _WIN32
    _mkdir(diretorio.c_str());
    #else 
    mkdir(diretorio.c_str(), 0777);
    #endif

    // Iniciar a contagem de tempo
    clock_t start = clock();

    vector<Grafo> grafos;
    for (int i = 0; i < instancias; ++i) {
        int vertices = distribVertices(gen); // Número de vértices entre 1 e nodes
        int arestas = vertices; // Número de arestas igual ao número de vértices

        cout << "Grafo " << i << ": " << vertices << " vertices e " << arestas << " arestas\n";

        Grafo grafo = gerarGrafo(vertices, arestas, gen);
        grafos.push_back(grafo);
    }

    // Salvar grafos na pasta "Grafos"
    salvarGrafos(grafos, diretorio + "/grafos.txt");

    // Calcular e exibir o tempo total
    clock_t end = clock();
    double tempo = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    cout << "Tempo total: " << tempo << " segundos\n";
    
    cout << "Grafos gerados e salvos em " << diretorio << "/grafos.txt\n\n" << endl;

    return 0;
}