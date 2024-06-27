// #include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definindo a estrutura de uma aresta
typedef struct Aresta 
{
    int destino;
    void* dado;
    struct Aresta* prox;
} Aresta;

// Definindo Estrutura de um dado (No)
typedef struct No 
{
    void* dado;
    Aresta* listaArestas;
} No;

// Definindo a Estrutura do próprio Grafo
typedef struct Grafo 
{
    int numVertices;
    int tamNo;
    int tamArestas;
    No* vertices;
} Grafo;

