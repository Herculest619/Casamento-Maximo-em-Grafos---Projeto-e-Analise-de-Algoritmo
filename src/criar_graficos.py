import os
import matplotlib.pyplot as plt

# Verifica o diretório de trabalho atual
print("Diretório de trabalho atual:", os.getcwd())

# Caminho para o arquivo no diretório atual
caminho_arquivo = os.path.join(os.getcwd(), 'resultados.txt')
print("Caminho do arquivo:", caminho_arquivo)

# Função para ler os dados do arquivo resultados.txt
def ler_dados_arquivo(nome_arquivo):
    # Verifica se o arquivo existe
    if not os.path.exists(nome_arquivo):
        raise FileNotFoundError(f"Arquivo '{nome_arquivo}' não encontrado. Verifique o caminho e o nome do arquivo.")

    with open(nome_arquivo, 'r') as arquivo:
        linhas = arquivo.readlines()

    # Extrair os dados
    tempos = list(map(int, linhas[4].split()[1:]))  # Linha "Tempos: ..."
    vertices = list(map(int, linhas[1].split()[1:]))  # Linha "Vertices: ..."
    arestas = list(map(int, linhas[2].split()[1:]))  # Linha "Arestas: ..."
    emparelhamentos = list(map(int, linhas[3].split()[1:]))  # Linha "Emparelhamentos: ..."

    return tempos, vertices, arestas, emparelhamentos

try:
    # Ler os dados do arquivo
    tempos, vertices, arestas, emparelhamentos = ler_dados_arquivo(caminho_arquivo)

    # Preparar os dados para os gráficos
    data = []
    for i in range(len(tempos)):
        densidade = arestas[i] / vertices[i] if vertices[i] > 0 else 0  # Evitar divisão por zero
        produto = vertices[i] * arestas[i]
        data.append({
            'tempo': tempos[i],
            'vertices': vertices[i],
            'arestas': arestas[i],
            'densidade': densidade,
            'produto': produto,
            'emparelhamentos': emparelhamentos[i]
        })

    # Ordenar dados por tempo para melhor visualização
    data.sort(key=lambda x: x['tempo'])

    # Extrair dados para os gráficos
    tempos_plot = [d['tempo'] for d in data]
    vertices_plot = [d['vertices'] for d in data]
    arestas_plot = [d['arestas'] for d in data]
    densidade_plot = [d['densidade'] for d in data]
    produto_plot = [d['produto'] for d in data]

    # Criar os gráficos
    plt.figure(figsize=(15, 10))  # Ajuste o tamanho da figura

    # Gráfico 1: Número de Vértices vs Tempo (Tempo no eixo Y)
    plt.subplot(2, 2, 1)
    plt.scatter(vertices_plot, tempos_plot, color='blue')
    plt.title('Número de Vértices vs Tempo (ms)')
    plt.xlabel('Número de Vértices')
    plt.ylabel('Tempo (ms)')

    # Gráfico 2: Número de Arestas vs Tempo (Tempo no eixo Y)
    plt.subplot(2, 2, 2)
    plt.scatter(arestas_plot, tempos_plot, color='green')
    plt.title('Número de Arestas vs Tempo (ms)')
    plt.xlabel('Número de Arestas')
    plt.ylabel('Tempo (ms)')

    # Gráfico 3: Densidade vs Tempo (Tempo no eixo Y)
    plt.subplot(2, 2, 3)
    plt.scatter(densidade_plot, tempos_plot, color='red')
    plt.title('Razão Arestas/Vértices (Densidade) vs Tempo (ms)')
    plt.xlabel('Densidade')
    plt.ylabel('Tempo (ms)')

    # Gráfico 4: Produto vs Tempo (Tempo no eixo Y)
    plt.subplot(2, 2, 4)
    plt.scatter(produto_plot, tempos_plot, color='purple')
    plt.title('Produto (Vértices × Arestas) vs Tempo (ms)')
    plt.xlabel('Produto (Vértices × Arestas)')
    plt.ylabel('Tempo (ms)')

    # Ajustar layout e exibir os gráficos
    plt.tight_layout()
    plt.show()

except FileNotFoundError as e:
    print(e)