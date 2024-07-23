#include "grafo.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>


int main(){

    // Teste criando o meu grafo
    Grafo grafo = grafo_cria(sizeof(int), sizeof(int));

    // Dados para ser inserido no grafo
    int dado1 = 20;
    int dado2 = 30;
    int dado3 = 40;

    // Teste inserção de no no grafo
    int no1 = grafo_insere_no(grafo, &dado1);
    int no2 = grafo_insere_no(grafo, &dado2);
    int no3 = grafo_insere_no(grafo, &dado3);
    
    printf("Grafo apos inserir os nos:\n");
    grafo_imprime(grafo);

    // printf("Grafo apos remover 1 no\n");
    // grafo_remove_no(grafo, no2);

    // Teste alterar valor do nó no grafo
    int valorNovoValor = 99;
    grafo_altera_valor_no(grafo,no2,&valorNovoValor);
    printf("Grafo apos alterar o valor do 2 para 99 no\n");
    grafo_imprime(grafo);

    // Teste Remoção de no do grafo
    grafo_remove_no(grafo, no2);
    printf("Grafo apos remocao do segundo dado: \n");
    grafo_imprime(grafo);

    // Teste função que capta o numero de nos no grafo
    int x = grafo_nnos(grafo);
    printf("Numero de nos no grafo %d\n", x);



   // Teste destroi grafo 
    grafo_destroi(grafo);
    grafo = NULL;
    grafo_imprime(grafo);
    
    return 0;
}