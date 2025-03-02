CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall
LDFLAGS = 
EXE_SUFFIX :=
ifeq ($(OS),Windows_NT)
    EXE_SUFFIX := .exe
endif

all: GeradorDeInstancias AlgoritmoDeBlossom

GeradorDeInstancias: GeradorDeInstancias.cpp
	$(CXX) $(CXXFLAGS) $< -o $@$(EXE_SUFFIX) $(LDFLAGS)

AlgoritmoDeBlossom: AlgoritmoDeBlossom.cpp
	$(CXX) $(CXXFLAGS) $< -o $@$(EXE_SUFFIX) $(LDFLAGS)

run: all
	./GeradorDeInstancias$(EXE_SUFFIX)
	./AlgoritmoDeBlossom$(EXE_SUFFIX)

clean:
ifeq ($(OS),Windows_NT)
	-del /Q GeradorDeInstancias$(EXE_SUFFIX) AlgoritmoDeBlossom$(EXE_SUFFIX) *.o
else
	-rm -f GeradorDeInstancias$(EXE_SUFFIX) AlgoritmoDeBlossom$(EXE_SUFFIX) *.o
endif

.PHONY: all clean run