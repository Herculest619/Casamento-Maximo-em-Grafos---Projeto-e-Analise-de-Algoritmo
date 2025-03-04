# Compilador para C
CC = gcc
# Compilador para C++
CXX = g++

# Opções de compilação para C
CFLAGS = -std=c11 -O2 -Wall
# Opções de compilação para C++
CXXFLAGS = -std=c++11 -O2 -Wall

# Flags de linking
LDFLAGS = 

# Sufixo para executáveis no Windows
EXE_SUFFIX := 
ifeq ($(OS),Windows_NT)
    EXE_SUFFIX := .exe
endif

# Nomes dos executáveis com caminho para a pasta bin
EXECUTABLES = bin/GeradorDeInstancias bin/AlgoritmoDeBlossom

# Regra padrão (compila tudo)
all: bin $(EXECUTABLES)

# Criação da pasta bin
bin:
ifeq ($(OS),Windows_NT)
	@if not exist bin mkdir bin
else
	mkdir -p bin
endif

# Regra para compilar o GeradorDeInstancias
bin/GeradorDeInstancias: src/GeradorDeInstancias.cpp
	$(CXX) $(CXXFLAGS) $< -o $@$(EXE_SUFFIX) $(LDFLAGS)

# Regra para compilar o AlgoritmoDeBlossom
bin/AlgoritmoDeBlossom: src/AlgoritmoDeBlossom.cpp
	$(CXX) $(CXXFLAGS) $< -o $@$(EXE_SUFFIX) $(LDFLAGS)

# Regra para executar os programas
run: all
	./bin/GeradorDeInstancias$(EXE_SUFFIX)
	./bin/AlgoritmoDeBlossom$(EXE_SUFFIX)

# Regra para limpar os arquivos gerados
clean:
ifeq ($(OS),Windows_NT)
	-del /Q bin/$(EXECUTABLES)$(EXE_SUFFIX) *.o
else
	-rm -f bin/$(EXECUTABLES)$(EXE_SUFFIX) *.o
endif

# Marca as regras como "phony" (não correspondem a arquivos)
.PHONY: all clean run bin
