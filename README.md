# Problema do Casamento Máximo em Grafos

## Para rodar o código:
```bash
make run
```

## O problema

Dado um grafo G = (V, E), determinar o maior subconjunto de aresta, E′ ⊆ E, que formam um casamento.<br>Ou seja, nenhum aresta de E′ compartilham vértices em comum;


<img src="imagens/casamento1.png" alt="Exemplo de Grafo" style="width: 35%; height: auto;">

<img src="imagens/casamento2.png" alt="Exemplo de Grafo" style="width: 35%; height: auto;">


## Estrutura de arquivos

```bash
projeto/
│
├── bin/
│   ├── GeradorDeInstancias
│   └── AlgoritmoDeBlossom
│
├── src/
│   ├── GeradorDeInstancias.cpp
│   └── AlgoritmoDeBlossom.cpp
│
├── Grafos/
│   ├── grafos.txt
│   ├── subgrafos.txt
│   └── resultados.txt
│
└── Makefile
```