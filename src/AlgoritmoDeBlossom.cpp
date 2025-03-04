#include <iostream>
#include <fstream> // Usado para manipulação de arquivos
#include <vector>
#include <string>
#include <cstring>
#include <algorithm> // Usado para count. Count é usado para contar o número de emparelhamentos
#include <ctime> // Para clock_t
#include <sys/stat.h> // Para sistemas Unix/Linux
#ifdef _WIN32
#include <direct.h> // Para Windows
#endif

using namespace std;

// Struct para representar um grafo usando lista de adjacência
struct Grafo {
    int vertices;
    vector<vector<int>> listaAdjacencia;
};

class AlgoritmoBlossom {
public:
    // Função para ler os grafos a partir de um arquivo
    vector<Grafo> lerGrafos(const string& nomeArquivo) {
        vector<Grafo> grafos;
        ifstream arquivo(nomeArquivo);
        string linha;

        while (getline(arquivo, linha)) {
            if (linha.find("Grafo") != string::npos) {
                Grafo grafo;
                getline(arquivo, linha); // Lê a linha "Vertices: X"
                grafo.vertices = stoi(linha.substr(10));
                grafo.listaAdjacencia.resize(grafo.vertices);

                getline(arquivo, linha); // Lê a linha "Arestas:"
                while (getline(arquivo, linha)) {
                    if (linha.empty()) break; // Fim das arestas
                    size_t espaco = linha.find(' ');
                    int u = stoi(linha.substr(0, espaco));
                    int v = stoi(linha.substr(espaco + 1));
                    grafo.listaAdjacencia[u].push_back(v);
                    grafo.listaAdjacencia[v].push_back(u);
                }
                grafos.push_back(grafo);
            }
        }

        return grafos;
    }

    // Função para encontrar o emparelhamento máximo em um grafo e retornar como string
    string encontrarEmparelhamentoMaximo(const Grafo& grafo, int instancia) {
        int n = grafo.vertices;
        vector<int> emparelhamento(n, -1);
        int contagem = 0;

        for (int u = 0; u < n; ++u) {
            vector<bool> visitado(n, false);
            if (buscaProfundidade(u, visitado, grafo.listaAdjacencia, emparelhamento)) contagem++;
        }

        // Construir a string com o emparelhamento
        string resultado = "Grafo " + to_string(instancia) + ":\n";
        for (size_t i = 0; i < emparelhamento.size(); ++i) {
            if (emparelhamento[i] > static_cast<int>(i)) {
                resultado += to_string(i) + " - " + to_string(emparelhamento[i]) + "\n";
            }
        }
        resultado += "\n";

        return resultado;
    }

private:
    // Função de busca em profundidade para encontrar emparelhamentos
    bool buscaProfundidade(int u, vector<bool>& visitado, const vector<vector<int>>& listaAdjacencia, vector<int>& emparelhamento) {
        for (int v : listaAdjacencia[u]) {
            if (!visitado[v]) {
                visitado[v] = true;
                if (emparelhamento[v] == -1 || buscaProfundidade(emparelhamento[v], visitado, listaAdjacencia, emparelhamento)) {
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
    const string diretorio = "Grafos";  // Pasta onde os arquivos serão salvos
    const string nomeArquivo = diretorio + "/grafos.txt";
    auto grafos = solver.lerGrafos(nomeArquivo);

    const int total = grafos.size();
    vector<int> emparelhamentos(total);
    vector<double> tempos(total); // Usando double para armazenar tempos em segundos

    // Arquivo único para salvar todos os subgrafos
    ofstream arquivoSubgrafos(diretorio + "/subgrafos.txt");

    for (int i = 0; i < total; ++i) {
        clock_t inicio = clock(); // Inicia a contagem do tempo
        string resultado = solver.encontrarEmparelhamentoMaximo(grafos[i], i);
        clock_t fim = clock(); // Finaliza a contagem do tempo

        double duracao = double(fim - inicio) / CLOCKS_PER_SEC; // Calcula o tempo em segundos

        // Salvar o resultado no arquivo único
        arquivoSubgrafos << resultado;

        emparelhamentos[i] = count(resultado.begin(), resultado.end(), '\n') / 2; // Contar emparelhamentos
        tempos[i] = duracao;

        cout << "[## " << (i+1) << "/" << total << " ##] Vertices: " 
             << grafos[i].vertices << " Emparelhamento: " << emparelhamentos[i] 
             << " Tempo: " << duracao << "s\n"; // Exibe o tempo em segundos
    }

    arquivoSubgrafos.close();

    // Salvar resultados gerais na pasta "Grafos"
    ofstream resultado(diretorio + "/resultados.txt");
    resultado << "Emparelhamentos: ";
    for (auto emparelhamento : emparelhamentos) resultado << emparelhamento << " ";
    resultado << "\nTempos: ";
    for (auto tempo : tempos) resultado << tempo << " ";

    return 0;
}