#include "grafo.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>


int main(){
    // Teste criando o meu grafo
    Grafo grafo = grafo_cria(sizeof(int), sizeof(int));

    int dado1 = 20;
    int dado2 = 30;
    int dado3 = 40;

    int no1 = grafo_insere_no(grafo, &dado1);
    int no2 = grafo_insere_no(grafo, &dado2);
    int no3 = grafo_insere_no(grafo, &dado3);
    
    printf("Grafo apos inserir os nos:\n");
    grafo_imprime(grafo);


    grafo_destroi(grafo);
    grafo = NULL;
    
    printf("Grafo apos destruicao: \n");
    grafo_imprime(grafo);
    return 0;
}