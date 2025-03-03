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

class AlgoritmoBlossom {
public:
    // Função para ler os tamanhos dos grafos a partir de um arquivo
    vector<string> lerTamanhos() {
        vector<string> tamanhos;
        ifstream arquivo("tamanhos.txt");
        string linha;
        while (getline(arquivo, linha)) tamanhos.push_back(linha);
        return tamanhos;
    }

    // Função para ler um grafo a partir de um arquivo e retornar sua representação matricial
    vector<vector<int>> lerGrafo(const string& nomeArquivo, int& verticeMaximo) {
        ifstream arquivo(nomeArquivo);
        vector<pair<int, int>> arestas;
        verticeMaximo = 0;

        string linha;
        while (getline(arquivo, linha)) {
            size_t espaco = linha.find(' ');
            if (espaco == string::npos) continue;

            int u = stoi(linha.substr(0, espaco));
            int v = stoi(linha.substr(espaco + 1));
            arestas.push_back({u, v});
            verticeMaximo = max(verticeMaximo, max(u, v));
        }

        vector<vector<int>> grafo(verticeMaximo + 1, vector<int>(verticeMaximo + 1, 0));
        for (auto& aresta : arestas) {
            grafo[aresta.first][aresta.second] = 1;
            grafo[aresta.second][aresta.first] = 1;
        }

        return grafo;
    }

    // Função para encontrar o emparelhamento máximo em um grafo
    int encontrarEmparelhamentoMaximo(const vector<vector<int>>& grafo, int instancia) {
        int n = grafo.size();
        vector<int> emparelhamento(n, -1);
        int contagem = 0;

        for (int u = 0; u < n; ++u) {
            vector<bool> visitado(n, false);
            if (buscaProfundidade(u, visitado, grafo, emparelhamento)) contagem++;
        }

        string diretorio = "grafosMaximos";
        #ifdef _WIN32
        _mkdir(diretorio.c_str());
        #else 
        mkdir(diretorio.c_str(), 0777);
        #endif

        ofstream arquivo(diretorio + "/subGrafo" + to_string(instancia) + ".txt");
        for (size_t i = 0; i < emparelhamento.size(); ++i) {
            if (emparelhamento[i] > static_cast<int>(i)) {
                arquivo << i << " - " << emparelhamento[i] << endl;
            }
        }

        return contagem;
    }

private:
    // Função de busca em profundidade para encontrar emparelhamentos
    bool buscaProfundidade(int u, vector<bool>& visitado, const vector<vector<int>>& grafo, vector<int>& emparelhamento) {
        for (size_t v = 0; v < grafo.size(); ++v) {
            if (grafo[u][v] && !visitado[v]) {
                visitado[v] = true;
                if (emparelhamento[v] == -1 || buscaProfundidade(emparelhamento[v], visitado, grafo, emparelhamento)) {
                    emparelhamento[v] = u;
                    return true;
                }
            }
        }
        return false;
    }
};

int main() {
    AlgoritmoBlossom solver;
    auto tamanhos = solver.lerTamanhos();
    const int total = 200;
    vector<int> tamanhosGrafos(total);
    vector<int> emparelhamentos(total);
    vector<long long> tempos(total);
    long long tempoTotal = 0;

    for (int i = 0; i < total; ++i) {
        string arquivo = "grafosTeste/grafo" + to_string(i + 1) + ".txt";
        int verticeMaximo;
        auto grafo = solver.lerGrafo(arquivo, verticeMaximo);

        auto inicio = high_resolution_clock::now();
        int emparelhamento = solver.encontrarEmparelhamentoMaximo(grafo, i);
        auto fim = high_resolution_clock::now();

        auto duracao = duration_cast<milliseconds>(fim - inicio).count();

        tamanhosGrafos[i] = i < static_cast<int>(tamanhos.size()) ? stoi(tamanhos[i]) : 0;
        emparelhamentos[i] = emparelhamento / 2;
        tempos[i] = duracao;
        tempoTotal += duracao;

        cout << "[## " << (i+1) << "/" << total << " ##] Tamanho: " 
             << tamanhosGrafos[i] << " Emparelhamento: " << emparelhamentos[i] 
             << " Tempo: " << duracao << "ms\n";
    }

    ofstream resultado("resultados.txt");
    resultado << "Tempo total: " << tempoTotal << "ms\nTamanhos: ";
    for (auto tamanho : tamanhosGrafos) resultado << tamanho << " ";
    resultado << "\nEmparelhamentos: ";
    for (auto emparelhamento : emparelhamentos) resultado << emparelhamento << " ";
    resultado << "\nTempos: ";
    for (auto tempo : tempos) resultado << tempo << " ";

    return 0;
}