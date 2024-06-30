// #include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"

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
typedef struct _grafo 
{
    int numVertices;
    int tam_no;
    int tam_aresta;
    No* vertice;
} _grafo;

//  Função para criar um grafo vazio
Grafo grafo_cria(int tam_no, int tam_aresta)
{
    Grafo grafo = (Grafo)malloc(sizeof(_grafo));
    if(grafo == NULL)
    {
        fprintf(stderr, "erro ao alocar memoria para o grafo\n");
        exit(EXIT_FAILURE);
    }
    grafo->numVertices = 0;
    grafo->tam_no = tam_no;
    grafo->tam_aresta = tam_aresta;
    grafo->vertice = NULL;
    return grafo;
}

//  Função para destruir o Grafo
void grafo_destroi(Grafo self)
{
    // Se grafo null então retorna  
    if(self == NULL)
    {
        return;
    }
    // Senão...
    // Libera memória para cada nó
    for(int i = 0; i < self->numVertices; i++)
    {
        Aresta* aresta = self->vertice[i].listaArestas;
        while (aresta)
        {
            Aresta* proximo = aresta->prox;
            free(aresta->dado);
            free(aresta);
            aresta = proximo;
        }
        free(self->vertice[i].dado);
    }
    // Libero a memoria do array de nós
    free(self->vertice);

    //Libero a memoria da estrutura do grafo
    free(self);
}

// Insere um nó no grafo, com o dado apontado por pdado
// retorna o número do novo nó
int grafo_insere_no(Grafo self, void *pdado)
{
    //Verifica os argumetos recebidos
    if(self == NULL || pdado == NULL){
        fprintf(stderr, "deu ruim \n");
        return -1; // <- Retorna o valor de erro.
    }

    // Aqui vou realocar espaço para o novo no 
    self->vertice = (No*)realloc(self->vertice, (self->numVertices + 1) * sizeof(No));
    if(self->vertice == NULL)
    {
        fprintf(stderr, "Erro ao realocar memoria para o bendito no\n");
        exit(EXIT_FAILURE);
    }

    // Aloco memoria para um novo no
    No* novoNo = &self->vertice[self->numVertices];
    novoNo->dado = malloc(self->tam_no);
    if(novoNo->dado == NULL){
        fprintf(stderr, "Erro ao alocar memoria para os dados od novo nozin\n");
        exit(EXIT_FAILURE);
    }

    // Copio os dados para o novo no
    memcpy(novoNo->dado, pdado, self->tam_no);
    
    // Inicializo a list de arestas do novo nó
    novoNo->listaArestas = NULL;

    //Atualizo o numero de vertices
    int indice_novo_no = self->numVertices;
    self->numVertices++;

    return indice_novo_no;

}

/* Função para imprimir um grafo completo, suas arestas e nos e dados*/
void grafo_imprime(Grafo grafo)
{
    if(grafo == NULL)
    {
        printf("KABUM!! Grafo esta destruido (vazio ou nulo).\n");
        return;
    }

    for(int i = 0; i < grafo->numVertices; i++)
    {
        printf("No %d: ", i);
        No* no = &grafo->vertice[i];
        printf("dados = %d ", *(int*)no->dado);
        Aresta* aresta = no->listaArestas;
        while (aresta)
        {
            printf(" -> [Destino: %d, dados: %d] ", aresta->destino, *(int*)aresta->dado);
            aresta = aresta->prox;
        }
        printf("\n");
        
    }
    
}