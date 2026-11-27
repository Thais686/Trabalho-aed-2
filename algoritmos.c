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

void testarAlgoritmos(Grafo *g, const char *nome, int direcionado) {
    if (g == NULL)
        return;
    printf("\n%s\n", nome);
    if (direcionado) {
        int *ordem = malloc(g->V * sizeof(int));
        if (ordem == NULL)
            return;
        if (ordenacaoTopologica(g, ordem)) {
            printf("Ordenacao topologica: ");
            
            for (int i = 0; i < g->V; i++)
                printf("%d ", ordem[i]);

            printf("\n");
        } else {
            printf("Nao existe ordenacao topologica.\n");
        }
        printf("Possui ciclo direcionado: %s\n",
               temCicloDirecionado(g) ? "Sim" : "Nao");

        free(ordem);
    } else {
        printf("Possui ciclo nao direcionado: %s\n",
               temCicloNaoDirecionado(g) ? "Sim" : "Nao");
    }
}

// FORTEMENTE CONEXO 

/* DFS usada na primeira etapa do algoritmo de Kosaraju */
static void dfsKosaraju1(Grafo *g, int v, int visitado[], int pilha[], int *topo) {
    visitado[v] = 1;

    for (No *p = g->lista[v]; p != NULL; p = p->prox) {
        int u = p->destino;

        if (!visitado[u]) {
            dfsKosaraju1(g, u, visitado, pilha, topo);
        }
    }

    pilha[(*topo)++] = v;
}


/* DFS usada na segunda etapa */
static void dfsKosaraju2(Grafo *g, int v, int visitado[]) {
    visitado[v] = 1;

    printf("%d ", v);

    for (No *p = g->lista[v]; p != NULL; p = p->prox) {
        int u = p->destino;

        if (!visitado[u]) {
            dfsKosaraju2(g, u, visitado);
        }
    }
}


/* Cria o grafo transposto */
static Grafo *transporGrafo(Grafo *g) {
    Grafo *transposto = criarGrafo(g->V);

    if (transposto == NULL)
        return NULL;

    for (int v = 0; v < g->V; v++) {

        for (No *p = g->lista[v]; p != NULL; p = p->prox) {

            addArresta(
                transposto,
                p->destino,
                v,
                p->peso,
                1
            );
        }
    }

    return transposto;
}


/* Algoritmo de Kosaraju */
void componentesFortementeConexos(Grafo *g, int direcionado) {

    if (g == NULL)
        return;
        
     if (!direcionado) {
        printf("\n=== COMPONENTES FORTEMENTE CONEXAS ===\n");
        printf("Nao e possivel aplicar CFC em grafo nao direcionado.\n");
        return;
    }

    int *visitado = calloc(g->V, sizeof(int));
    int *pilha = malloc(g->V * sizeof(int));

    if (visitado == NULL || pilha == NULL) {
        free(visitado);
        free(pilha);
        return;
    }

    int topo = 0;

    /* Primeira DFS */
    for (int i = 0; i < g->V; i++) {

        if (!visitado[i]) {
            dfsKosaraju1(g, i, visitado, pilha, &topo);
        }
    }

    /* Cria o grafo transposto */
    Grafo *transposto = transporGrafo(g);

    if (transposto == NULL) {
        free(visitado);
        free(pilha);
        return;
    }

    /* Zera vetor de visitados */
    for (int i = 0; i < g->V; i++) {
        visitado[i] = 0;
    }

    printf("\n=== COMPONENTES FORTEMENTE CONEXAS ===\n");

    int componente = 1;

    /* Segunda DFS, na ordem inversa da primeira */
    for (int i = topo - 1; i >= 0; i--) {

        int v = pilha[i];

        if (!visitado[v]) {

            printf("Componente %d: ", componente);

            dfsKosaraju2(transposto, v, visitado);

            printf("\n");

            componente++;
        }
    }

    free(visitado);
    free(pilha);
    liberarGrafo(transposto);
}

void testarGrafoGrande() {

    int V = 5000;

    clock_t inicio, fim;

    inicio = clock();

    // Cria o grafo
    Grafo *g = criarGrafo(V);

    if (g == NULL) {
        printf("Erro ao criar grafo!\n");
        return;
    }

    // Adiciona as arestas
    for (int i = 0; i < V; i++) {
        addArresta(g, i, (i + 1) % V, 1, 1);
        addArresta(g, i, (i + 2) % V, 1, 1);
        addArresta(g, i, (i + 3) % V, 1, 1);
    }
    
    // testar nos algoritmos 
    
     testarAlgoritmos(g, "Grafo 5", 1);
     componentesFortementeConexos(g, 1);

    fim = clock();

    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("\n===== TESTE DE GRAFO GRANDE =====\n");
    printf("Vertices: %d\n", g->V);
    printf("Arestas: %d\n", g->A);
    printf("Tempo de: %.6f segundos\n", tempo);

    liberarGrafo(g);
}

