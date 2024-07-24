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

//  Função para destruir o Grafo
void grafo_destroi(Grafo self)
{
    // Se grafo null então retorna  
    if(self == NULL)
    {
        return;
    }
    /* Senão...
    Libera memória para cada nó
    */
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
// retorna o índice do novo nó
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

// Remove um nó do grafo e as arestas incidentes nesse nó
// a identificação dos nós remanescentes é alterada, como se esse nó nunca tivesse existido
void grafo_remove_no(Grafo self, int no)
{
    //1º Verifica se o grafo e o indice do no a ser removido são válidos
    if(self == NULL || no < 0 || no >= self->numVertices)
    {
        fprintf(stderr, "Erro: Arvore ou no são nulos\n");
        return;
    }

    //2º Remoção de todas arestas do no
    for(int i = 0; i < self->numVertices; i++)
    {
        //Pego a aresta anterior
        Aresta *arestaAnterior = NULL;
        //Pego a aresta atual na lista encadeada
        Aresta *arestaAtual = self->vertice[i].listaArestas;

        while (arestaAtual != NULL)
        {
            /* Verifico se a aresta atual tem o nó a ser removido
            como destino ou então esse nó atual ja é o nó pra ser removido */
           if(arestaAtual->destino == no || i == no)
           {
                /* Se minha anterior é nula então a gente ta na primeira aresta da lista logo ajusto o ponteiro p/ proxima aresta */
                if(arestaAnterior == NULL){
                    self->vertice[i].listaArestas = arestaAtual->prox;
                }else{
                    /* Senão ajusto o ponteiro proximo da aresta anteriror pra pular pra aresta atual */
                    arestaAnterior->prox = arestaAtual->prox;
                }
                // Libero o dado da aresta atual
                free(arestaAtual->dado);
                // Variavel auxiliar pegando o proximo aresta antes de liberar
                Aresta* proximaArestaAuxiliar = arestaAtual->prox;
                //Libero a aresta atual
                free(arestaAtual);
                //Reajusto a atual
                arestaAtual = proximaArestaAuxiliar;
           }else{
                /* Se a aresta atual não precisa ser removida então eu pulo para a proxima */
                arestaAnterior = arestaAtual;
                arestaAtual = arestaAtual->prox;
           }
        }
        
    }

    // 3º Liberar a memória dos dados do nó(indice) a ser removido
    free(self->vertice[no].dado);

    // 4º Desalocar os nos restantes e preencher o que ficar em branco pelo no removido deslocando para a esquerda
    for(int i = no; i < self->numVertices - 1; i++)
    {
        // copio o conteudo do no na posicao (i + 1) para a posição (i)
        self->vertice[i] = self->vertice[i + 1];
    }

    //5º Ajusto as arestas restantes iterando por tods os nos restantes
    for(int i = 0; i < self->numVertices - 1; i++)
    {
        Aresta* arestaAtual = self->vertice[i].listaArestas;
        while(arestaAtual != NULL)
        {
            /* Se o meu destino da aresta for maior que o no removido, então eu decremento o indice desse no */
            if(arestaAtual->destino > no)
            {
                arestaAtual->destino--;
            }
            arestaAtual = arestaAtual->prox;
        }
    }

    // 6º Reduzo o tamanho do arra dos nos e realoco a memoria 
    self->vertice = (No*)realloc(self->vertice, (self->numVertices - 1) * sizeof(No));
    if(self->vertice == NULL && self->numVertices > 1)
    {
        fprintf(stderr, "Erro ao realocar memoria para os nos\n");
        exit(EXIT_FAILURE);
    }

    // 7º Atualizo o contador de nos
    self->numVertices--;
}            

// altera o valor associado a um nó (copia o valor apontado por pdado para o nó)
void grafo_altera_valor_no(Grafo self, int no, void *pdado)
{
    // 1º Verifico os parâmetros recebidos
    if(self == NULL || no < 0 || no >= self->numVertices || pdado == NULL){
        fprintf(stderr, "erro grafo, no ou pdado nulos\n");
        exit(EXIT_FAILURE);
    }

    // 2º Se parametros validos então
    /*(destino)(origem)(tamanho em bytes)*/
    memcpy(self->vertice[no].dado, pdado, self->tam_no);
}

// coloca em pdado o valor associado a um nó
void grafo_valor_no(Grafo self, int no, void *pdado)
{
    if(self == NULL || no < 0 || no >= self->numVertices || pdado == NULL){
        printf("dados passados para o no sao invalidos\n");
        return;
    }
    // Copio os dados para o novo no
    memcpy(pdado, self->vertice[no].dado, self->tam_no);
}

// retorna o número de nós do grafo
int grafo_nnos(Grafo self){
    if(self == NULL){
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

    if (atual == NULL) {
        // A aresta não existe, nada para remover
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

// Função auxiliar pra criar a aresta
void cria_aresta(No *no, int destino, void *pdado, int tam_aresta) {
    Aresta *novaAresta = (Aresta *)malloc(sizeof(Aresta));
    if (novaAresta == NULL) {
        fprintf(stderr, "Erro ao alocar memória para a nova aresta\n");
        exit(EXIT_FAILURE);
    }
    novaAresta->dado = malloc(tam_aresta);
    if (novaAresta->dado == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o dado da nova aresta\n");
        free(novaAresta);
        exit(EXIT_FAILURE);
    }
    memcpy(novaAresta->dado, pdado, tam_aresta);
    novaAresta->destino = destino;
    novaAresta->prox = no->listaArestas;
    no->listaArestas = novaAresta;
}

// altera o valor da aresta que interliga o nó origem ao nó destino (copia de *pdado)
// caso a aresta não exista, deve ser criada
// caso pdado seja NULL, a aresta deve ser removida
void grafo_altera_valor_aresta(Grafo self, int origem, int destino, void *pdado) {
    // Verificação dos parâmetros recebidos
    if (self == NULL || origem < 0 || destino < 0 || origem >= self->numVertices || destino >= self->numVertices) {
        printf("Erro: parâmetros inválidos na função altera_valor_aresta\n");
        return;
    }

    // Procurar a aresta na lista de arestas do nó de origem
    Aresta *anterior = NULL;
    Aresta *atual = self->vertice[origem].listaArestas;

    while (atual != NULL && atual->destino != destino) {
        anterior = atual;
        atual = atual->prox;
    }

    // Se pdado é NULL, a aresta deve ser removida
    if (pdado == NULL) {
        if (atual != NULL) {
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
        printf("Erro: parâmetros inválidos na função altera_valor_aresta\n");
        return;
    }


    Aresta *atual = self->vertice[origem].listaArestas;
    while(atual != NULL){
        if(atual->destino == destino)
        {
            // coloca em pdado (se não for NULL) o valor associado à aresta, se existir
            if(pdado != NULL){
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
    if(self == NULL || origem < 0 || origem >= self->numVertices){
        printf("Erro: parametros invalidos linha 384\n");
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
    if(self == NULL || destino < 0 || destino >= self->numVertices){
        printf("Erro: parametros invalidos linha 399\n");
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