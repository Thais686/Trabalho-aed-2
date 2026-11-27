#ifndef ALGORITMOS_H
#define ALGORITMOS_H
#include "grafo.h"
#include <time.h>

int ordenacaoTopologica(Grafo *g, int *ordem);
int temCicloDirecionado(Grafo *g);
int temCicloNaoDirecionado(Grafo *g);
void testarAlgoritmos(Grafo *g, const char *nome, int direcionado);
void componentesFortementeConexos(Grafo *g, int direcionado);
void testarGrafoGrande();

#endif
