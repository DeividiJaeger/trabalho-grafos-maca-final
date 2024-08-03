#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "grafo.h"
#include "fila.h"

// Função auxiliar para imprimir a ordem topológica
void imprimir_ordem_topologica(Fila ordem) {
    printf("Ordem Topológica: ");
    while (!fila_vazia(ordem)) {
        int vertice;
        fila_remove(ordem, &vertice);
        printf("%d ", vertice);
    }
    printf("\n");
}

int main() {
    // Cria o grafo
    Grafo g = grafo_cria(sizeof(int), sizeof(int));
   
    // Testa inserção e valor dos nós
    int valor0 = 10, valor1 = 20, valor2 = 30, valor3 = 40;
    int novo_no;

    // Inserir nós no grafo
    printf("Inserindo nós...\n");
    novo_no = grafo_insere_no(g, &valor0);
    printf("Nó 0 inserido com valor %d\n", valor0);

    novo_no = grafo_insere_no(g, &valor1);
    printf("Nó 1 inserido com valor %d\n", valor1);

    novo_no = grafo_insere_no(g, &valor2);
    printf("Nó 2 inserido com valor %d\n", valor2);

    novo_no = grafo_insere_no(g, &valor3);
    printf("Nó 3 inserido com valor %d\n", valor3);

    // Verifica valores dos nós
    printf("Valores dos nós:\n");
    for (int i = 0; i < grafo_nnos(g); i++) {
        int valor;
        grafo_valor_no(g, i, &valor);
        printf("Nó %d tem valor %d\n", i, valor);
    }

    // Testa inserção e alteração de arestas
    int aresta_valor = 1;
    printf("Inserindo arestas...\n");
    grafo_altera_valor_aresta(g, 0, 1, &aresta_valor); // Aresta de 0 para 1
    grafo_altera_valor_aresta(g, 1, 2, &aresta_valor); // Aresta de 1 para 2
    grafo_altera_valor_aresta(g, 2, 3, &aresta_valor); // Aresta de 2 para 3
    grafo_altera_valor_aresta(g, 3, 0, &aresta_valor); // Aresta de 3 para 0

    // Testa grafo_arestas_que_partem
    printf("Arestas que partem do nó 1:\n");
    grafo_arestas_que_partem(g, 1);
    int vizinho;
    while (grafo_proxima_aresta(g, &vizinho, NULL)) {
        printf("Aresta de 1 para %d\n", vizinho);
    }

    // Testa grafo_arestas_que_chegam
    printf("Arestas que chegam ao nó 2:\n");
    grafo_arestas_que_chegam(g, 2);
    while (grafo_proxima_aresta(g, &vizinho, NULL)) {
        printf("Aresta de %d para 2\n", vizinho);
    }

    // Testa grafo_tem_ciclo
    if (grafo_tem_ciclo(g)) {
        printf("O grafo tem ciclo\n");
    } else {
        printf("O grafo não tem ciclo\n");
    }

    // Testa grafo_ordem_topologica
    Fila ordem_topologica = grafo_ordem_topologica(g);
    if (ordem_topologica == NULL) {
        fprintf(stderr, "Erro ao calcular a ordem topológica\n");
    } else {
        imprimir_ordem_topologica(ordem_topologica);
        fila_destroi(ordem_topologica);
    }

    // Testa remoção de nó
    printf("Removendo nó 2...\n");
    grafo_remove_no(g, 2);

    // Verifica se o nó foi removido e imprime o grafo restante
    printf("Valores dos nós após a remoção do nó 2:\n");
    for (int i = 0; i < grafo_nnos(g); i++) {
        int valor;
        grafo_valor_no(g, i, &valor);
        printf("Nó %d tem valor %d\n", i, valor);
    }

    // Testa se a aresta foi removida corretamente
    printf("Arestas que partem do nó 1 após remoção de nó 2:\n");
    grafo_arestas_que_partem(g, 1);
    while (grafo_proxima_aresta(g, &vizinho, NULL)) {
        printf("Aresta de 1 para %d\n", vizinho);
    }

    // Destrói o grafo
    grafo_destroi(g);

    return 0;
}
