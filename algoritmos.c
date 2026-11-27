#include <stdio.h>
#include <stdlib.h>
#include "algoritmos.h"

static int dfsTopologica(Grafo *g, int v, int estado[], int ordem[], int *pos) {
    estado[v] = 1;
    for (No *p = g->lista[v]; p != NULL; p = p->prox) {
        int u = p->destino;

        // Um vértice ainda em processamento indica um ciclo.
        if (estado[u] == 1)
            return 0;
        if (estado[u] == 0) {
            if (!dfsTopologica(g, u, estado, ordem, pos))
                return 0;
        }
    }
    estado[v] = 2;

    // Insere o vértice ao final da ordenação, em ordem inversa de conclusão.
    ordem[--(*pos)] = v;
    return 1;
}

int ordenacaoTopologica(Grafo *g, int *ordem) {
    if (g == NULL || ordem == NULL)
        return 0;
    int *estado = calloc(g->V, sizeof(int));
    if (estado == NULL)
        return 0;
    int pos = g->V;
    for (int i = 0; i < g->V; i++) {
        if (estado[i] == 0) {
            if (!dfsTopologica(g, i, estado, ordem, &pos)) {
                free(estado);
                return 0;
            }
        }
    }

    free(estado);
    return 1;
}

static int dfsCicloDirecionado(Grafo *g, int v, int estado[]) {
    estado[v] = 1;
    for (No *p = g->lista[v]; p != NULL; p = p->prox) {
        int u = p->destino;

        // Encontrar um vértice ativo significa que existe um ciclo.
        if (estado[u] == 1)
            return 1;
        if (estado[u] == 0) {
            if (dfsCicloDirecionado(g, u, estado))
                return 1;
        }
    }
    estado[v] = 2;
    return 0;
}

int temCicloDirecionado(Grafo *g) {
    if (g == NULL)
        return 0;
    int *estado = calloc(g->V, sizeof(int));
    if (estado == NULL)
        return 0;
    for (int i = 0; i < g->V; i++) {
        if (estado[i] == 0) {
            if (dfsCicloDirecionado(g, i, estado)) {
                free(estado);
                return 1;
            }
        }
    }

    free(estado);
    return 0;
}

static int dfsCicloNaoDirecionado(Grafo *g, int v, int pai, int visitado[]) {
    visitado[v] = 1;
    for (No *p = g->lista[v]; p != NULL; p = p->prox) {
        int u = p->destino;
        if (!visitado[u]) {
            if (dfsCicloNaoDirecionado(g, u, v, visitado))
                return 1;
        }
        // Ignora a aresta que retorna ao pai; as demais indicam ciclo.
        else if (u != pai) {
            return 1;
        }
    }

    return 0;
}

int temCicloNaoDirecionado(Grafo *g) {
    if (g == NULL)
        return 0;
    int *visitado = calloc(g->V, sizeof(int));
    if (visitado == NULL)
        return 0;
    for (int i = 0; i < g->V; i++) {
        if (!visitado[i]) {
            if (dfsCicloNaoDirecionado(g, i, -1, visitado)) {
                free(visitado);
                return 1;
            }
        }
    }

    free(visitado);
    return 0;
}
