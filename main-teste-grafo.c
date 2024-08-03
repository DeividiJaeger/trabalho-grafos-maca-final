#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"
#include "fila.h"

int main() {
    // Cria um grafo com espaço para 10 nós e 10 arestas
    Grafo g = grafo_cria(sizeof(int), sizeof(int));

    // Insere nós
    int valor = 10;
    grafo_insere_no(g, &valor);
    valor = 20;
    grafo_insere_no(g, &valor);
    valor = 30;
    grafo_insere_no(g, &valor);

    // Verifica o número de nós
    printf("Número de nós após inserção: %d\n", grafo_nnos(g));

    // Verifica os valores dos nós
    int val;
    grafo_valor_no(g, 0, &val);
    printf("Valor do nó 0: %d\n", val);
    grafo_valor_no(g, 1, &val);
    printf("Valor do nó 1: %d\n", val);
    grafo_valor_no(g, 2, &val);
    printf("Valor do nó 2: %d\n", val);

    // Altera valor do nó 0
    valor = 100;
    grafo_altera_valor_no(g, 0, &valor);
    grafo_valor_no(g, 0, &val);
    printf("Valor do nó 0: %d\n", val);

    // Remove nó 1
    grafo_remove_no(g, 1);

    // Verifica o número de nós após remoção
    printf("Número de nós após remoção: %d\n", grafo_nnos(g));

    // Verifica os valores dos nós após remoção
    grafo_valor_no(g, 0, &val);
    printf("Valor do nó 0: %d\n", val);
    grafo_valor_no(g, 1, &val); // No 1 agora deve ter o valor do nó que foi movido
    printf("Valor do nó 1: %d\n", val);

    // Destroi o grafo
    grafo_destroi(g);

    return 0;
}
