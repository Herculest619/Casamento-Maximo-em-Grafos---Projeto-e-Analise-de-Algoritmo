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
#include <queue> // Para a busca de caminhos aumentantes
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
vector<string> listarArquivosDeGrafos(const string& diretorio) {
    vector<string> arquivos; // Vetor para armazenar os arquivos .txt
    DIR *dir; // Ponteiro para o diretório
    struct dirent *entrada; // Estrutura para armazenar as entradas do diretório

    if ((dir = opendir(diretorio.c_str())) != nullptr) {
        while ((entrada = readdir(dir)) != nullptr) {
            string nomeArquivo = entrada->d_name;
            if (nomeArquivo.find("grafo_") != string::npos && nomeArquivo.find(".txt") != string::npos) {
                arquivos.push_back(diretorio + "/" + nomeArquivo);
            }
        }
        closedir(dir);
    } else {
        cerr << "Erro: Não foi possível abrir o diretório " << diretorio << endl;
    }
    return arquivos;
}

class AlgoritmoDeBlossom{
public:
    // Lê o grafo a partir do arquivo e salva na estrutura Grafo
    Grafo lerGrafo(const string& nomeArquivo) {
        Grafo grafo;
        ifstream arquivo(nomeArquivo);
        string linha;

        if (!arquivo.is_open()) {
            cerr << "Erro: Arquivo " << nomeArquivo << " não encontrado." << endl;
            return grafo;
        }

        // Lê o número de vértices
        if (!getline(arquivo, linha)) {
            cerr << "Erro: Arquivo vazio ou inválido (" << nomeArquivo << ")" << endl;
            return grafo;
        }
        size_t pos = linha.find("Vertices: ");
        if (pos == string::npos) {
            cerr << "Erro: Formato inválido em " << nomeArquivo << endl;
            return grafo;
        }
        grafo.vertices = stoi(linha.substr(pos + 10));

        // Inicializa a lista de adjacência
        grafo.listaAdjacencia.resize(grafo.vertices);
        grafo.arestas = 0;

        // Lê as arestas
        if (!getline(arquivo, linha) || linha.find("Arestas:") == string::npos) {
            cerr << "Erro: Seção de arestas faltante em " << nomeArquivo << endl;
            return grafo;
        }

        // Lê as arestas e preenche a lista de adjacência
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

    // Implementação do algoritmo de Edmonds (Blossom) para emparelhamento máximo
    vector<int> aplicarBlossom(const Grafo &grafo) {
        int n = grafo.vertices;
        vector<vector<int>> g = grafo.listaAdjacencia;
        vector<int> emparelhamento(n, -1), pai(n), base(n); // emparelhamento: resultado; pai: predecessores; base: base do blossom
        vector<bool> usado(n), flor(n); // usado: vértices visitados; flor: vértices contraídos no blossom

        // Função lambda para calcular o ancestral comum mínimo (ACM) entre dois vértices
        // ACM: vértice mais próximo do início de um caminho
        auto calcularACM = [&](int a, int b) -> int { //auto: tipo de retorno é inferido pelo compilador
            vector<bool> usadoLocal(n, false);
            while (true) {
                a = base[a];
                usadoLocal[a] = true;
                if (emparelhamento[a] == -1) break;
                a = pai[emparelhamento[a]];
            }
            while (true) {
                b = base[b];
                if (usadoLocal[b])
                    return b;
                b = pai[emparelhamento[b]];
            }
        };

        // Função lambda para marcar os caminhos (usada na contração de blossoms)
        // contração de blossom: redução de um ciclo ímpar para um vértice
        auto marcarCaminho = [&](int v, int b, int x, queue<int>& fila) {
            while (base[v] != b) {
                flor[base[v]] = flor[base[emparelhamento[v]]] = true;
                pai[v] = x;
                x = emparelhamento[v];
                if (!usado[x]) {
                    usado[x] = true;
                    fila.push(x);
                }
                v = pai[emparelhamento[v]];
            }
        };

        // Função lambda para buscar um caminho aumentante a partir de um vértice s
        // Caminho aumentante é um caminho que começa e termina em vértices não emparelhados
        auto buscarCaminho = [&](int s) -> int {
            usado.assign(n, false);
            pai.assign(n, -1);
            for (int i = 0; i < n; i++) {
                base[i] = i;
            }
            queue<int> fila;
            fila.push(s);
            usado[s] = true;
            while (!fila.empty()) {
                int v = fila.front();
                fila.pop();
                for (int u : g[v]) {
                    if (base[v] == base[u] || emparelhamento[v] == u)
                        continue;
                    if (u == s || (emparelhamento[u] != -1 && pai[emparelhamento[u]] != -1)) {
                        int atual = calcularACM(v, u);
                        flor.assign(n, false);
                        marcarCaminho(v, atual, u, fila);
                        marcarCaminho(u, atual, v, fila);
                        for (int i = 0; i < n; i++) {
                            if (flor[base[i]]) {
                                base[i] = atual;
                                if (!usado[i]) {
                                    usado[i] = true;
                                    fila.push(i);
                                }
                            }
                        }
                    } else if (pai[u] == -1) {
                        pai[u] = v;
                        if (emparelhamento[u] == -1)
                            return u;
                        usado[emparelhamento[u]] = true;
                        fila.push(emparelhamento[u]);
                    }
                }
            }
            return -1;
        };

        // Para cada vértice não emparelhado, tenta encontrar um caminho aumentante
        for (int i = 0; i < n; i++) {
            if (emparelhamento[i] == -1) {
                int v = buscarCaminho(i);
                if (v != -1) {
                    // Quando um caminho aumentante é encontrado, atualiza o emparelhamento
                    int atual = v;
                    while (atual != -1) {
                        int pred = pai[atual];
                        int proximo = emparelhamento[pred];
                        emparelhamento[atual] = pred;
                        emparelhamento[pred] = atual;
                        atual = proximo;
                    }
                }
            }
        }
        return emparelhamento;
    }

    // Função que chama o Blossom, gera a saída e preenche o contador de emparelhamentos
    string obterEmparelhamentoMaximo(const Grafo& grafo, int instancia, int &contadorEmparelhamentos) {
        vector<int> emparelhamento = aplicarBlossom(grafo);
        int contagem = 0;
        string resultado = "Grafo " + to_string(instancia) + ":\n";
        resultado += "Vértices: " + to_string(grafo.vertices) + "\n";
        resultado += "Arestas: " + to_string(grafo.arestas) + "\n";
        resultado += "Emparelhamentos:\n";
        // Imprime cada aresta do emparelhamento apenas uma vez
        for (int i = 0; i < grafo.vertices; i++) {
            if (emparelhamento[i] != -1 && i < emparelhamento[i]) {
                resultado += to_string(i) + " - " + to_string(emparelhamento[i]) + "\n";
                contagem++;
            }
        }
        resultado += "Total de emparelhamentos: " + to_string(contagem) + "\n\n";
        contadorEmparelhamentos = contagem;
        return resultado;
    }
};

int main() {
    AlgoritmoDeBlossom algoritmo;
    const string pastaGrafos = "Grafos";
    const string pastaEmparelhamentos = "Emparelhamentos";

    // Lista os arquivos de grafos
    vector<string> arquivosDeGrafos = listarArquivosDeGrafos(pastaGrafos);
    int totalGrafos = arquivosDeGrafos.size();

    if (totalGrafos == 0) {
        cerr << "Nenhum arquivo de grafo encontrado na pasta " << pastaGrafos << endl;
        return 1;
    }

    // Cria o diretório para os emparelhamentos (se não existir)
    #ifdef _WIN32
    _mkdir(pastaEmparelhamentos.c_str());
    #else 
    mkdir(pastaEmparelhamentos.c_str(), 0777);
    #endif

    vector<int> temposProcessamento;
    vector<int> emparelhamentosTotais;
    vector<int> vetorVertices;
    vector<int> vetorArestas;

    // Processa cada grafo
    for (size_t i = 0; i < arquivosDeGrafos.size(); i++) {
        string caminhoGrafo = arquivosDeGrafos[i];
        Grafo grafo = algoritmo.lerGrafo(caminhoGrafo);

        // Extrai o índice a partir do nome do arquivo (ex.: "grafo_3.txt")
        size_t inicio = caminhoGrafo.find_last_of("_") + 1;
        size_t fim = caminhoGrafo.find_last_of(".");
        int indice = stoi(caminhoGrafo.substr(inicio, fim - inicio));

        clock_t tempoInicio = clock();
        int contadorEmparelhamentos = 0;
        string resultado = algoritmo.obterEmparelhamentoMaximo(grafo, indice, contadorEmparelhamentos); // Executa o algoritmo de Blossom
        clock_t tempoFim = clock();
        double tempoProcessamento = double(tempoFim - tempoInicio) / CLOCKS_PER_SEC;

        // Salva o resultado do emparelhamento em um arquivo
        string caminhoEmparelhamento = pastaEmparelhamentos + "/emparelhamento_" + to_string(indice) + ".txt";
        ofstream arquivo(caminhoEmparelhamento);
        arquivo << resultado;
        arquivo.close();

        cout << "Processado grafo " << indice << " (" << (i+1) << "/" << totalGrafos << ") "
             << "Vertices: " << grafo.vertices << " Arestas: " << grafo.arestas << " "
             << "Emparelhamentos: " << contadorEmparelhamentos << " "
             << "Tempo: " << tempoProcessamento << "s\n";

        temposProcessamento.push_back(static_cast<int>(tempoProcessamento * 1000));
        emparelhamentosTotais.push_back(contadorEmparelhamentos);
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
