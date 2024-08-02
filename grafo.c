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
    int modoConsulta;    // Defino o tipo da consulta se 1 é que partem e 2 que chegam no no
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

/* Função para imprimir um grafo completo, suas arestas e nos e dados*/
// void grafo_imprime(Grafo grafo)
// {
//     if(grafo == NULL)
//     {
//         printf("KABUM!! Grafo esta destruido (vazio ou nulo).\n");
//         return;
//     }
//     for(int i = 0; i < grafo->numVertices; i++)
//     {
//         printf("No %d: ", i);
//         No* no = &grafo->vertice[i];
//         printf("dados = %d ", *(int*)no->dado);
//         Aresta* aresta = no->listaArestas;
//         while (aresta)
//         {
//             printf(" -> [Destino: %d, dados: %d] ", aresta->destino, *(int*)aresta->dado);
//             aresta = aresta->prox;
//         }
//         printf("\n");
//     }   
// }

// Remove um nó do grafo e as arestas incidentes nesse nó
// a identificação dos nós remanescentes é alterada, como se esse nó nunca tivesse existido
void grafo_remove_no(Grafo self, int no)
{
    if(self == NULL || no < 0 || no >= self->numVertices)
    {
        fprintf(stderr, "Erro: Grafo ou nó são nulos\n");
        return;
    }

    // Remoção de todas as arestas que chegam e partem do nó
    for(int i = 0; i < self->numVertices; i++)
    {
        Aresta *arestaAnterior = NULL;
        Aresta *arestaAtual = self->vertice[i].listaArestas;

        while (arestaAtual != NULL)
        {
            if(arestaAtual->destino == no || i == no)
            {
                if(arestaAnterior == NULL)
                {
                    self->vertice[i].listaArestas = arestaAtual->prox;
                }
                else
                {
                    arestaAnterior->prox = arestaAtual->prox;
                }
                
                free(arestaAtual->dado);
                Aresta* proximaArestaAuxiliar = arestaAtual->prox;
                free(arestaAtual);
                arestaAtual = proximaArestaAuxiliar;
            }
            else
            {
                arestaAnterior = arestaAtual;
                arestaAtual = arestaAtual->prox;
            }
        }
    }

    free(self->vertice[no].dado);

    for(int i = no; i < self->numVertices - 1; i++)
    {
        self->vertice[i] = self->vertice[i + 1];
    }

    if(self->numVertices > 1)
    {
        No* temp = (No*)realloc(self->vertice, (self->numVertices - 1) * sizeof(No));
        if (temp == NULL)
        {
            fprintf(stderr, "Erro ao realocar memória para os nós\n");
            exit(EXIT_FAILURE);
        }
        self->vertice = temp;
    }
    else
    {
        free(self->vertice);
        self->vertice = NULL;
    }

    self->numVertices--;
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
void remove_aresta(No *no, int destino) 
{
    Aresta *anterior = NULL;
    Aresta *atual = no->listaArestas;

    while (atual != NULL && atual->destino != destino) 
    {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) 
    {
        // A aresta não existe, nada para remover
        return;
    }

    if (anterior == NULL) 
    {
        // A aresta a ser removida é a primeira da lista
        no->listaArestas = atual->prox;
    } else 
    {
        // A aresta a ser removida está no meio ou fim da lista
        anterior->prox = atual->prox;
    }

    free(atual->dado);
    free(atual);
}

// Função auxiliar pra criar a aresta
void cria_aresta(No *no, int destino, void *pdado, int tam_aresta) 
{
    Aresta *novaAresta = (Aresta *)malloc(sizeof(Aresta));
    if (novaAresta == NULL) 
    {
        fprintf(stderr, "Erro ao alocar memória para a nova aresta\n");
        exit(EXIT_FAILURE);
    }
    novaAresta->dado = malloc(tam_aresta);
    if (novaAresta->dado == NULL) 
    {
        fprintf(stderr, "Erro ao alocar memória para o dado da nova aresta\n");
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
    // Verificação dos parâmetros recebidos
    if (self == NULL || origem < 0 || destino < 0 || origem >= self->numVertices || destino >= self->numVertices) 
    {
        printf("Erro: parâmetros inválidos na função altera_valor_aresta\n");
        return;
    }

    // Procurar a aresta na lista de arestas do nó de origem
    Aresta *anterior = NULL;
    Aresta *atual = self->vertice[origem].listaArestas;

    while (atual != NULL && atual->destino != destino) 
    {
        anterior = atual;
        atual = atual->prox;
    }

    // Se pdado é NULL, a aresta deve ser removida
    if (pdado == NULL) 
    {
        if (atual != NULL) 
        {
            remove_aresta(&self->vertice[origem], destino);
        }
        return;
    }

    // Se a aresta já existe, alterar o valor
    if (atual != NULL) {
        memcpy(atual->dado, pdado, self->tam_aresta);
    } else {
        // Se a aresta não existe, criar uma nova aresta
        cria_aresta(&self->vertice[origem], destino, pdado, self->tam_aresta);
    }
}

// coloca em pdado (se não for NULL) o valor associado à aresta, se existir
// retorna true se a aresta entre os nós origem e destino existir, e false se não existir
bool grafo_valor_aresta(Grafo self, int origem, int destino, void *pdado) 
{
    // Verificação dos parâmetros recebidos
    if (self == NULL || origem < 0 || destino < 0 || origem >= self->numVertices || destino >= self->numVertices) 
    {
        printf("Erro: parâmetros inválidos na função grafo_valor_aresta\n");
        printf("Detalhes: self=%p, origem=%d, destino=%d, numVertices=%d\n", self, origem, destino, self->numVertices);
        return false; 
    }

    Aresta *atual = self->vertice[origem].listaArestas;
    while (atual != NULL) 
    {
        if (atual->destino == destino) 
        {
            // Coloca em pdado (se não for NULL) o valor associado à aresta, se existir
            if (pdado != NULL) 
            {
                memcpy(pdado, atual->dado, self->tam_aresta);
            }
            // Aresta encontrada    
            return true;    
        }
        atual = atual->prox;
    }
    // Aresta não encontrada
    return false;
}

// inicia uma consulta a arestas que partem do nó origem
// as próximas chamadas a 'grafo_proxima_aresta' devem retornar os valores correspondentes
// à cada aresta que parte desse nó
void grafo_arestas_que_partem(Grafo self, int origem)
{
    if(self == NULL || origem < 0 || origem >= self->numVertices)
    {
        return;
    }
    self->arestaAtual = self->vertice[origem].listaArestas;
    self->modoConsulta = 1; // tipo 1 repectivo ao no que parte
    self->noConsulta = origem;
}

// inicia uma consulta a arestas que chegam ao nó destino
// as próximas chamadas a 'grafo_proxima_aresta' devem retornar os valores correspondentes
// à cada aresta que chega nesse nó
void grafo_arestas_que_chegam(Grafo self, int destino)
{
    if(self == NULL || destino < 0 || destino >= self->numVertices)
    {
        return;
    }

    self->arestaAtual = NULL;
    self->modoConsulta = 2; // tipo 2 repectivo ao no que chega
    self->noConsulta = destino;

    // Encontro a aresta que chega no destino 
    for(int i = 0; i < self->numVertices; i++)
    {
        Aresta *aresta = self->vertice[i].listaArestas;
        while(aresta != NULL)
        {
            if(aresta->destino == destino)
            {
                self->arestaAtual = aresta;
                return;
            }
            aresta = aresta->prox;
        }
    }
}

// Funções auxiliares para diminuir o tamanho e melhorar
// a legibilidade da função grafo_proxima_aresta
bool proxima_aresta_que_parte(Grafo self, int *vizinho, void *pdado)
{
    if(self->arestaAtual == NULL) 
    {
        return false;
    }
    if(vizinho != NULL)
    { 
        *vizinho = self->arestaAtual->destino;
    }
    if(pdado != NULL)
    {
        memcpy(pdado, self->arestaAtual->dado, self->tam_aresta);
    }
    
    self->arestaAtual = self->arestaAtual->prox;

    return true;
}

bool proxima_aresta_que_chega(Grafo self, int *vizinho, void *pdado) 
{
    // Percorre a lista de arestas atual procurando a próxima aresta que chega ao nó de destino
    while (self->arestaAtual != NULL) 
    {
        if (self->arestaAtual->destino == self->noConsulta) 
        {
            if (vizinho != NULL) 
            {
                *vizinho = self->noConsulta;
            }
            if (pdado != NULL) 
            {
                memcpy(pdado, self->arestaAtual->dado, self->tam_aresta);
            }
            // Atualiza arestaAtual para a próxima aresta na lista
            self->arestaAtual = self->arestaAtual->prox;
            return true; 
        }
        self->arestaAtual = self->arestaAtual->prox;
    }

    for (int i = 0; i < self->numVertices; i++) 
    {
        Aresta* aresta = self->vertice[i].listaArestas;
        while (aresta != NULL) 
        {
            if (aresta->destino == self->noConsulta) 
            {
                self->arestaAtual = aresta->prox; // Corrigir para pular a atual
                if (vizinho != NULL) 
                {
                    *vizinho = i;
                }
                if (pdado != NULL) 
                {
                    memcpy(pdado, aresta->dado, self->tam_aresta);
                }
                return true; 
            }
            aresta = aresta->prox;
        }
    }
    return false; 
}

bool grafo_proxima_aresta(Grafo self, int *vizinho, void *pdado)
{
    if(self == NULL || self->arestaAtual == NULL)
    {
        return false;
    }

    if(self->modoConsulta == 1)
    {
        return proxima_aresta_que_parte(self, vizinho, pdado);
    }else if(self->modoConsulta == 2)
    {
        return proxima_aresta_que_chega(self, vizinho, pdado);
    }

    return false;
}

// Função auxiliar para verificação do ciclo
bool grafo_tem_ciclo_aux(Grafo self, int v, bool *visitado, bool *pilhaRec)
{
    Aresta *aresta = self->vertice[v].listaArestas;
    while(aresta != NULL)
    {
        int destino = aresta->destino;

        // Se o desitno não foi visitado chamo a função recursivamente
        if(!visitado[destino] && grafo_tem_ciclo_aux(self, destino, visitado, pilhaRec))
        {
            return true;
        }else if(pilhaRec[destino])
        {
            return true;
        }
        aresta = aresta->prox;
    }
    
    // Removo o vertice da pilha de recursão
    pilhaRec[v] = false;
    return false;
}

// Retorna true se o grafo é cíclico, false caso contrário
bool grafo_tem_ciclo(Grafo self) {
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
