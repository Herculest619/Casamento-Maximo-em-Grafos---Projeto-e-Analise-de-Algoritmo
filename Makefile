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

# Nomes dos executáveis
EXECUTABLES = programa_c programa_cpp

# Regra padrão (compila tudo)
all: $(EXECUTABLES)

# Regra para compilar o programa em C
programa_c: src/programa.c
	$(CC) $(CFLAGS) $< -o $@$(EXE_SUFFIX) $(LDFLAGS)

# Regra para compilar o programa em C++
programa_cpp: src/programa.cpp
	$(CXX) $(CXXFLAGS) $< -o $@$(EXE_SUFFIX) $(LDFLAGS)

# Regra para executar os programas
run: all
	./programa_c$(EXE_SUFFIX)
	./programa_cpp$(EXE_SUFFIX)

# Regra para limpar os arquivos gerados
clean:
ifeq ($(OS),Windows_NT)
	-del /Q $(EXECUTABLES)$(EXE_SUFFIX) *.o
else
	-rm -f $(EXECUTABLES)$(EXE_SUFFIX) *.o
endif

# Marca as regras como "phony" (não correspondem a arquivos)
.PHONY: all clean run