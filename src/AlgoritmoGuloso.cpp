// Hércules Aparecido Teixeira - 18.2.8072

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <ctime>
#include <numeric> // Para accumulate
#include <sys/stat.h> // Para criar diretórios
#include <dirent.h> // Para listar arquivos no diretório
#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

using namespace std;

// Estrutura para representar um grafo
struct Grafo {
    int vertices;
    int arestas;
    vector<vector<int>> listaAdjacencia;
};

// Função para listar todos os arquivos .txt na pasta "Grafos"
vector<string> listarArquivosGrafos(const string& diretorio) {
    vector<string> arquivos;
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(diretorio.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            string nomeArquivo = ent->d_name;
            if (nomeArquivo.find("grafo_") != string::npos && nomeArquivo.find(".txt") != string::npos) {
                arquivos.push_back(diretorio + "/" + nomeArquivo);
            }
        }
        closedir(dir);
    } else {
        cerr << "Erro: Nao foi possivel abrir o diretorio " << diretorio << endl;
    }
    return arquivos;
}

class AlgoritmoGuloso {
public:
    // Lê o grafo a partir do arquivo
    Grafo lerGrafo(const string& nomeArquivo) {
        Grafo grafo;
        ifstream arquivo(nomeArquivo);
        string linha;

        if (!arquivo.is_open()) {
            cerr << "Erro: Arquivo " << nomeArquivo << " nao encontrado." << endl;
            return grafo;
        }

        // Lê o número de vértices
        if (!getline(arquivo, linha)) {
            cerr << "Erro: Arquivo vazio ou invalido (" << nomeArquivo << ")" << endl;
            return grafo;
        }
        size_t pos = linha.find("Vertices: ");
        if (pos == string::npos) {
            cerr << "Erro: Formato invalido em " << nomeArquivo << endl;
            return grafo;
        }
        grafo.vertices = stoi(linha.substr(pos + 10));

        // Inicializa a lista de adjacência
        grafo.listaAdjacencia.resize(grafo.vertices);
        grafo.arestas = 0;

        // Lê as arestas
        if (!getline(arquivo, linha) || linha.find("Arestas:") == string::npos) {
            cerr << "Erro: Secao de arestas faltante em " << nomeArquivo << endl;
            return grafo;
        }

        while (getline(arquivo, linha)) {
            if (linha.empty()) break;
            size_t espaco = linha.find(' ');
            if (espaco == string::npos) continue;
            int u = stoi(linha.substr(0, espaco));
            int v = stoi(linha.substr(espaco + 1));
            grafo.listaAdjacencia[u].push_back(v);
            grafo.listaAdjacencia[v].push_back(u);
            grafo.arestas++;
        }
        return grafo;
    }

    // Implementação gulosa para emparelhamento:
    // Para cada vértice não emparelhado, emparelha-o com o primeiro vizinho livre.
    vector<int> emparelhamentoGuloso(const Grafo &grafo) {
        int n = grafo.vertices;
        vector<int> match(n, -1);

        for (int i = 0; i < n; i++) {
            if (match[i] == -1) {
                for (int u : grafo.listaAdjacencia[i]) {
                    if (match[u] == -1) {
                        match[i] = u;
                        match[u] = i;
                        break;
                    }
                }
            }
        }
        return match;
    }

    // Função que chama o emparelhamento guloso, gera a saída e preenche o contador de emparelhamentos
    string encontrarEmparelhamentoMaximo(const Grafo& grafo, int instancia, int &contagemEmparelhamentos) {
        vector<int> match = emparelhamentoGuloso(grafo);
        int count = 0;
        string resultado = "Grafo " + to_string(instancia) + ":\n";
        resultado += "Vertices: " + to_string(grafo.vertices) + "\n";
        resultado += "Arestas: " + to_string(grafo.arestas) + "\n";
        resultado += "Emparelhamentos (metodologia gulosa):\n";
        // Apenas imprime cada aresta do emparelhamento uma única vez
        for (int i = 0; i < grafo.vertices; i++) {
            if (match[i] != -1 && i < match[i]) {
                resultado += to_string(i) + " - " + to_string(match[i]) + "\n";
                count++;
            }
        }
        resultado += "Total de emparelhamentos: " + to_string(count) + "\n\n";
        contagemEmparelhamentos = count;
        return resultado;
    }
};

int main() {
    AlgoritmoGuloso solver;
    const string dirGrafos = "Grafos";
    const string dirEmparelhamentos = "Emparelhamentos";

    // Lista os arquivos de grafos
    vector<string> arquivosGrafos = listarArquivosGrafos(dirGrafos);
    int totalGrafos = arquivosGrafos.size();

    if (totalGrafos == 0) {
        cerr << "Nenhum arquivo de grafo encontrado na pasta " << dirGrafos << endl;
        return 1;
    }

    // Cria o diretório para os emparelhamentos (se não existir)
    #ifdef _WIN32
    _mkdir(dirEmparelhamentos.c_str());
    #else 
    mkdir(dirEmparelhamentos.c_str(), 0777);
    #endif

    vector<int> temposProcessamento;
    vector<int> emparelhamentosTotais;
    vector<int> vetorVertices;
    vector<int> vetorArestas;

    // Processa cada grafo
    for (size_t i = 0; i < arquivosGrafos.size(); i++) {
        string caminhoGrafo = arquivosGrafos[i];
        Grafo grafo = solver.lerGrafo(caminhoGrafo);

        // Extrai o índice a partir do nome do arquivo (ex.: "grafo_3.txt")
        size_t inicio = caminhoGrafo.find_last_of("_") + 1;
        size_t fim = caminhoGrafo.find_last_of(".");
        int indice = stoi(caminhoGrafo.substr(inicio, fim - inicio));

        clock_t inicioTempo = clock();
        int contagemEmparelhamentos = 0;
        string resultado = solver.encontrarEmparelhamentoMaximo(grafo, indice, contagemEmparelhamentos);
        clock_t fimTempo = clock();
        double tempoProcessamento = double(fimTempo - inicioTempo) / CLOCKS_PER_SEC;

        // Salva o resultado do emparelhamento em um arquivo
        string caminhoEmparelhamento = dirEmparelhamentos + "/emparelhamento_" + to_string(indice) + ".txt";
        ofstream arquivo(caminhoEmparelhamento);
        arquivo << resultado;
        arquivo.close();

        cout << "Processado grafo " << indice << " (" << (i+1) << "/" << totalGrafos << ") "
             << "Vertices: " << grafo.vertices << " Arestas: " << grafo.arestas << " "
             << "Emparelhamentos: " << contagemEmparelhamentos << " "
             << "Tempo: " << tempoProcessamento << "s\n";

        temposProcessamento.push_back(static_cast<int>(tempoProcessamento * 1000));
        emparelhamentosTotais.push_back(contagemEmparelhamentos);
        vetorVertices.push_back(grafo.vertices);
        vetorArestas.push_back(grafo.arestas);
    }

    // Grava o arquivo resultados.txt com os dados finais
    ofstream resultados("resultados.txt");
    resultados << "Tempo total: " << accumulate(temposProcessamento.begin(), temposProcessamento.end(), 0) << "ms\n";
    resultados << "Vertices: ";
    for (int v : vetorVertices) resultados << v << " ";
    resultados << "\nArestas: ";
    for (int a : vetorArestas) resultados << a << " ";
    resultados << "\nEmparelhamentos: ";
    for (int e : emparelhamentosTotais) resultados << e << " ";
    resultados << "\nTempos: ";
    for (int t : temposProcessamento) resultados << t << " ";
    resultados << "\n";

    cout << "Tempo total: " << accumulate(temposProcessamento.begin(), temposProcessamento.end(), 0) / 1000.0 << "s\n";
    return 0;
}