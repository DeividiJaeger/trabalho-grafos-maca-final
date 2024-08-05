#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fila.h"
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
    Aresta *arestaAtual; // Serve para reastrear o estado da iteração
    int modoConsulta;    // Defino o tipo da consulta se 1 é que partem e 2 que chegam no nÓ
    int noConsulta;      // Guardo o no atual da consulta aqui
} _grafo;

//  Função para criar um grafo vazio
Grafo grafo_cria(int tam_no, int tam_aresta)    
{
    /* O dado, tamaho no e tamanho aresta representa o tamanho do dado que eu quero armazenar no no
    pode então ter o sizeof de um int ou sizeof de uma struct tanto faz
    */
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

void grafo_destroi(Grafo self)
{
    if(self == NULL)
    {
        return;
    }
    
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

    free(self->vertice);
    free(self);
}

// Insere um nó no grafo, com o dado apontado por pdado
// retorna o índice do novo nó
int grafo_insere_no(Grafo self, void *pdado)
{
    if (self == NULL || pdado == NULL)
    {
        fprintf(stderr, "Erro: grafo ou dado nulos\n");
        return -1; // Retorna o valor de erro.
    }

    // Realoca memória para o novo nó
    No* temp = (No*)realloc(self->vertice, (self->numVertices + 1) * sizeof(No));
    if (temp == NULL)
    {
        fprintf(stderr, "Erro ao realocar memória para o nó\n");
        exit(EXIT_FAILURE);
    }
    self->vertice = temp;

    // Aloca memória para o novo nó
    No* novoNo = &self->vertice[self->numVertices];
    novoNo->dado = malloc(self->tam_no);
    if (novoNo->dado == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para os dados do novo nó\n");
        exit(EXIT_FAILURE);
    }

    // Copia os dados para o novo nó
    memcpy(novoNo->dado, pdado, self->tam_no);
    
    // Inicializa a lista de arestas do novo nó
    novoNo->listaArestas = NULL;

    // Atualiza o número de vértices
    int indice_novo_no = self->numVertices;
    self->numVertices++;

    return indice_novo_no;
}

// Remove um nó do grafo e as arestas incidentes nesse nó
// a identificação dos nós remanescentes é alterada, como se esse nó nunca tivesse existido
void grafo_remove_no(Grafo self, int no) 
{
    if (self == NULL || no < 0 || no >= self->numVertices) 
    {
        return;
    }

    // Remoção de todas as arestas que partem do nó
    Aresta *arestaAtual = self->vertice[no].listaArestas;
    while (arestaAtual != NULL) 
    {
        Aresta *proximaAresta = arestaAtual->prox;
        free(arestaAtual->dado);
        free(arestaAtual);
        arestaAtual = proximaAresta;
    }

    // Remoção de todas as arestas que chegam ao nó
    for (int i = 0; i < self->numVertices; i++) 
    {
        Aresta *arestaAnterior = NULL;
        arestaAtual = self->vertice[i].listaArestas;

        while (arestaAtual != NULL) 
        {
            if (arestaAtual->destino == no) 
            {
                if (arestaAnterior == NULL) 
                {
                    self->vertice[i].listaArestas = arestaAtual->prox;
                } else {
                    arestaAnterior->prox = arestaAtual->prox;
                }

                free(arestaAtual->dado);
                Aresta *proximaAresta = arestaAtual->prox;
                free(arestaAtual);
                arestaAtual = proximaAresta;
            } else {
                arestaAnterior = arestaAtual;
                arestaAtual = arestaAtual->prox;
            }
        }
    }

    // Libera a memória do nó removido
    free(self->vertice[no].dado);

    // Ajuste os nós restantes
    for (int i = no; i < self->numVertices - 1; i++) 
    {
        self->vertice[i] = self->vertice[i + 1];
    }

    self->numVertices--;

    if (self->numVertices > 0) 
    {
        No *temp = (No *)realloc(self->vertice, self->numVertices * sizeof(No));
        if (temp == NULL) 
        {
            exit(EXIT_FAILURE);
        }
        self->vertice = temp;
    } else {
        free(self->vertice);
        self->vertice = NULL;
    }

    // Ajustar os índices das arestas restantes
    for (int i = 0; i < self->numVertices; i++) 
    {
        arestaAtual = self->vertice[i].listaArestas;
        while (arestaAtual != NULL) 
        {
            if (arestaAtual->destino > no) 
            {
                arestaAtual->destino--;
            }
            arestaAtual = arestaAtual->prox;
        }
    }
}

// altera o valor associado a um nó (copia o valor apontado por pdado para o nó)
void grafo_altera_valor_no(Grafo self, int no, void *pdado)
{
    // 1º Verifico os parâmetros recebidos
    if(self == NULL || no < 0 || no >= self->numVertices || pdado == NULL)
    {
        exit(EXIT_FAILURE);
    }

    // 2º Se parametros validos então
    /*(destino)(origem)(tamanho em bytes)*/
    memcpy(self->vertice[no].dado, pdado, self->tam_no);
}

// coloca em pdado o valor associado a um nó
void grafo_valor_no(Grafo self, int no, void *pdado)
{
    if(self == NULL || no < 0 || no >= self->numVertices || pdado == NULL)
    {
        return;
    }
    // Copio os dados para o novo no
    memcpy(pdado, self->vertice[no].dado, self->tam_no);
}

// retorna o número de nós do grafo
int grafo_nnos(Grafo self)
{
    if(self == NULL)
    {
        printf("erro: Grafo nulo\n");
    }
    return self->numVertices;
}

// Arestas
// Função auxiliar para remover a aresta
void remove_aresta(No *no, int destino) {
    Aresta *anterior = NULL;
    Aresta *atual = no->listaArestas;

    while (atual != NULL && atual->destino != destino) {
        anterior = atual;
        atual = atual->prox;
    }
    
    // A aresta não existe, nada para remover
    if (atual == NULL) {
        return;
    }

    if (anterior == NULL) {
        // A aresta a ser removida é a primeira da lista
        no->listaArestas = atual->prox;
    } else {
        // A aresta a ser removida está no meio ou fim da lista
        anterior->prox = atual->prox;
    }

    free(atual->dado);
    free(atual);
}

// Função auxiliar para criar a aresta
void cria_aresta(No *no, int destino, void *pdado, int tam_aresta) 
{
    Aresta *novaAresta = (Aresta *)malloc(sizeof(Aresta));
    if (novaAresta == NULL) 
    {
        exit(EXIT_FAILURE);
    }
    novaAresta->dado = malloc(tam_aresta);
    if (novaAresta->dado == NULL) 
    {
        free(novaAresta);
        exit(EXIT_FAILURE);
    }
    memcpy(novaAresta->dado, pdado, tam_aresta);
    novaAresta->destino = destino;
    novaAresta->prox = no->listaArestas;
    no->listaArestas = novaAresta;
}

// Altera o valor da aresta que interliga o nó origem ao nó destino (copia de *pdado)
// Caso a aresta não exista, deve ser criada
// Caso pdado seja NULL, a aresta deve ser removida
void grafo_altera_valor_aresta(Grafo self, int origem, int destino, void *pdado) 
{
    if (self == NULL || origem < 0 || origem >= self->numVertices || destino < 0 || destino >= self->numVertices) {
        printf("Erro: parâmetro inválido.\n");
        return;
    }

    Aresta* aresta = self->vertice[origem].listaArestas;
    Aresta* anterior = NULL;

    while (aresta != NULL) 
    {
        if (aresta->destino == destino) 
        {
            if (pdado == NULL) 
            {
                // Remover a aresta
                if (anterior == NULL) 
                {
                    self->vertice[origem].listaArestas = aresta->prox;
                } else {
                    anterior->prox = aresta->prox;
                }
                free(aresta->dado);
                free(aresta);
                return;
            } else {
                // Atualizar o valor da aresta
                memcpy(aresta->dado, pdado, self->tam_aresta);
                return;
            }
        }
        anterior = aresta;
        aresta = aresta->prox;
    }

    if (pdado != NULL) {
        // Criar uma nova aresta
        Aresta* novaAresta = (Aresta*)malloc(sizeof(Aresta));
        novaAresta->destino = destino;
        novaAresta->dado = malloc(self->tam_aresta);
        memcpy(novaAresta->dado, pdado, self->tam_aresta);
        novaAresta->prox = self->vertice[origem].listaArestas;
        self->vertice[origem].listaArestas = novaAresta;
    }
}

// coloca em pdado (se não for NULL) o valor associado à aresta, se existir
// retorna true se a aresta entre os nós origem e destino existir, e false se não existir
bool grafo_valor_aresta(Grafo self, int origem, int destino, void *pdado) 
{
    if (self == NULL || origem < 0 || origem >= self->numVertices || destino < 0 || destino >= self->numVertices) {
        return false;
    }

    Aresta* aresta = self->vertice[origem].listaArestas;

    while (aresta != NULL) 
    {
        if (aresta->destino == destino) 
        {
            if (pdado != NULL) 
            {
                memcpy(pdado, aresta->dado, self->tam_aresta);
            }
            return true;
        }
        aresta = aresta->prox;
    }

    return false;
}

// inicia uma consulta a arestas que partem do nó origem
// as próximas chamadas a 'grafo_proxima_aresta' devem retornar os valores correspondentes
// à cada aresta que parte desse nó
void grafo_arestas_que_partem(Grafo self, int origem) {
    if (self == NULL || origem < 0 || origem >= self->numVertices) 
    {
        printf("Erro: parâmetro inválido.\n");
        return;
    }

    self->arestaAtual = self->vertice[origem].listaArestas;
    self->modoConsulta = 1;
    self->noConsulta = origem;
}

// inicia uma consulta a arestas que chegam ao nó destino
// as próximas chamadas a 'grafo_proxima_aresta' devem retornar os valores correspondentes
// à cada aresta que chega nesse nó
void grafo_arestas_que_chegam(Grafo self, int destino) 
{
    if (self == NULL || destino < 0 || destino >= self->numVertices) 
    {
        printf("Erro: parâmetro inválido.\n");
        return;
    }

    self->arestaAtual = NULL;
    self->modoConsulta = 2;
    self->noConsulta = destino;

    // Encontrar a primeira aresta que chega ao destino
    for (int i = 0; i < self->numVertices; i++) 
    {
        Aresta* aresta = self->vertice[i].listaArestas;
        while (aresta != NULL) 
        {
            if (aresta->destino == destino) 
            {
                self->arestaAtual = aresta;
                return;
            }
            aresta = aresta->prox;
        }
    }
}

bool grafo_proxima_aresta(Grafo self, int *vizinho, void *pdado) 
{
    if (self == NULL || self->arestaAtual == NULL) 
    {
        return false;
    }

    Aresta* arestaAtual = self->arestaAtual;

    if (self->modoConsulta == 1) 
    {
        // Consulta de arestas que partem do nó
        if (vizinho != NULL) 
        {
            *vizinho = arestaAtual->destino;
        }
        if (pdado != NULL) 
        {
            memcpy(pdado, arestaAtual->dado, self->tam_aresta);
        }
        self->arestaAtual = arestaAtual->prox;
        return true;
    } else if (self->modoConsulta == 2) 
    {
        // Consulta de arestas que chegam ao nó
        for (int i = 0; i < self->numVertices; i++) 
        {
            Aresta* aresta = self->vertice[i].listaArestas;
            while (aresta != NULL) 
            {
                if (aresta == self->arestaAtual) 
                {
                    if (vizinho != NULL) 
                    {
                        *vizinho = i;
                    }
                    if (pdado != NULL) 
                    {
                        memcpy(pdado, aresta->dado, self->tam_aresta);
                    }

                    // Encontrar a próxima aresta que chega ao nó de destino
                    self->arestaAtual = aresta->prox;
                    for (int j = i; j < self->numVertices; j++) 
                    {
                        Aresta* proxAresta = (j == i) ? aresta->prox : self->vertice[j].listaArestas;
                        while (proxAresta != NULL) 
                        {
                            if (proxAresta->destino == self->noConsulta) 
                            {
                                self->arestaAtual = proxAresta;
                                return true;
                            }
                            proxAresta = proxAresta->prox;
                        }
                    }
                    self->arestaAtual = NULL;
                    return true;
                }
                aresta = aresta->prox;
            }
        }
    }
    return false;
}

bool grafo_tem_ciclo_aux(Grafo self, int v, bool *visitado, bool *pilhaRec)
{
    if (self == NULL || v < 0 || v >= self->numVertices) 
    {
        return false;
    }

    pilhaRec[v] = true;
    visitado[v] = true;

    Aresta *aresta = self->vertice[v].listaArestas;
    while (aresta != NULL) 
    {
        int destino = aresta->destino;

        if (destino < 0 || destino >= self->numVertices) 
        {
            return false;
        }

        // Se o destino não foi visitado, chamo a função recursivamente
        if (!visitado[destino] && grafo_tem_ciclo_aux(self, destino, visitado, pilhaRec)) 
        {
            return true;
        } else if (pilhaRec[destino]) {
            return true;
        }
        aresta = aresta->prox;
    }

    // Removo o vértice da pilha de recursão
    pilhaRec[v] = false;
    return false;
}
bool grafo_tem_ciclo(Grafo self) 
{
    if (self == NULL) 
    {
        return false;
    }

    bool *visitado = (bool *)malloc(self->numVertices * sizeof(bool));
    bool *pilhaRec = (bool *)malloc(self->numVertices * sizeof(bool));
    
    if (visitado == NULL || pilhaRec == NULL) 
    {
        fprintf(stderr, "Erro ao alocar memória.\n");
        return false;
    }

    // Inicializar arrays
    for (int i = 0; i < self->numVertices; i++) 
    {
        visitado[i] = false;
        pilhaRec[i] = false;
    }

    // Verificar ciclos
    for (int i = 0; i < self->numVertices; i++) 
    {
        if (!visitado[i] && grafo_tem_ciclo_aux(self, i, visitado, pilhaRec)) 
        {
            free(visitado);
            free(pilhaRec);
            return true;
        }
    }

    free(visitado);
    free(pilhaRec);
    return false;
}

// Aqui percorre todas as arestas do grafo e conta o numero de vezes que cada no e destino de uma aresta
// Função para calcular os graus de entrada de cada vértice
// Definição da função calcular_grau_entrada
int* calcular_grau_entrada(Grafo self) 
{
    int numVertices = grafo_nnos(self);
    int* graus_entrada = calloc(numVertices, sizeof(int));
    for (int i = 0; i < numVertices; i++) 
    {
        grafo_arestas_que_partem(self, i);
        int vizinho;
        while (grafo_proxima_aresta(self, &vizinho, NULL)) 
        {
            graus_entrada[vizinho]++;
        }
    }
    return graus_entrada;
}

Fila grafo_ordem_topologica(Grafo self) 
{
    if (self == NULL) 
    {
        printf("Erro: grafo nulo\n");
        return fila_cria(sizeof(int)); // Retorna uma fila vazia
    }

    int numVertices = grafo_nnos(self);
    int* graus_entrada = calcular_grau_entrada(self);
    Fila fila = fila_cria(sizeof(int));
    Fila ordem_topologica = fila_cria(sizeof(int));

    for (int i = 0; i < numVertices; i++) 
    {
        if (graus_entrada[i] == 0) 
        {
            fila_insere(fila, &i);
        }
    }

    int vertices_processados = 0;
    while (!fila_vazia(fila)) 
    {
        int vertice;
        fila_remove(fila, &vertice);
        fila_insere(ordem_topologica, &vertice);
        vertices_processados++;
        grafo_arestas_que_partem(self, vertice);
        int vizinho;
        while (grafo_proxima_aresta(self, &vizinho, NULL)) 
        {
            graus_entrada[vizinho]--;
            if (graus_entrada[vizinho] == 0) 
            {
                fila_insere(fila, &vizinho);
            }
        }
    }

    free(graus_entrada);
    fila_destroi(fila);

    if (vertices_processados != numVertices) 
    {
        fila_destroi(ordem_topologica);
        return fila_cria(sizeof(int));
    }

    return ordem_topologica;
}
