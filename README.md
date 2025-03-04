## Problema do Casamento Máximo em Grafos

## Para rodar o código:
```bash
make run
```

## O problema

Dado um grafo G = (V, E), determinar o maior subconjunto de aresta, E′ ⊆ E, que formam um casamento. Ou seja, nenhum aresta de E′ compartilham vértices em comum;


## Estrutura de arquivos

```bash
projeto/
│
├── bin/
│   ├── GeradorDeInstancias      (ou GeradorDeInstancias.exe no Windows)
│   └── AlgoritmoDeBlossom       (ou AlgoritmoDeBlossom.exe no Windows)
│
├── src/
│   ├── GeradorDeInstancias.cpp
│   └── AlgoritmoDeBlossom.cpp
│
└── Makefile
```