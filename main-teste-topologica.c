#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"
#include "fila.h"

int main() {
    // Cria um grafo com tamanho de nó e aresta arbitrários (por exemplo, tamanho de int)
    Grafo grafo = grafo_cria(sizeof(int), sizeof(int));

    // Verifica se a criação do grafo foi bem-sucedida
    if (grafo == NULL) {
        fprintf(stderr, "Erro ao criar o grafo.\n");
        return EXIT_FAILURE;
    }

    // Adiciona vértices ao grafo
    int dado1 = 1, dado2 = 2, dado3 = 3, dado4 = 4, dado5 = 5, dado6 = 6;
    grafo_insere_no(grafo, &dado1); // Vértice 0
    grafo_insere_no(grafo, &dado2); // Vértice 1
    grafo_insere_no(grafo, &dado3); // Vértice 2
    grafo_insere_no(grafo, &dado4); // Vértice 3
    grafo_insere_no(grafo, &dado5); // Vértice 4
    grafo_insere_no(grafo, &dado6); // Vértice 5

    // Adiciona arestas ao grafo
    grafo_altera_valor_aresta(grafo, 5, 2, &dado1); // 5 -> 2
    grafo_altera_valor_aresta(grafo, 5, 0, &dado2); // 5 -> 0
    grafo_altera_valor_aresta(grafo, 4, 0, &dado3); // 4 -> 0
    grafo_altera_valor_aresta(grafo, 4, 1, &dado4); // 4 -> 1
    grafo_altera_valor_aresta(grafo, 2, 3, &dado5); // 2 -> 3
    grafo_altera_valor_aresta(grafo, 3, 1, &dado6); // 3 -> 1

    // Verifica se o grafo é cíclico
    if (grafo_tem_ciclo(grafo)) {
        printf("O grafo possui um ciclo. Ordem topológica não é possível.\n");
        return EXIT_FAILURE;
    }

    // Calcula a ordem topológica
    Fila ordem_topologica = grafo_ordem_topologica(grafo);

    // Verifica se a ordem topológica é válida
    if (fila_vazia(ordem_topologica)) {
        printf("Erro: a ordem topológica retornada é vazia.\n");
        return EXIT_FAILURE;
    }

    // Imprime a ordem topológica
    printf("Ordem Topológica: ");
    while (!fila_vazia(ordem_topologica)) {
        int vertice;
        fila_remove(ordem_topologica, &vertice);
        printf("%d ", vertice);
    }
    printf("\n");

    // Limpa a memória
    fila_destroi(ordem_topologica);
    grafo_destroi(grafo);

    return 0;
}
