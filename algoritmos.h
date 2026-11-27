#ifndef ALGORITMOS_H
#define ALGORITMOS_H
#include "grafo.h"

int ordenacaoTopologica(Grafo *g, int *ordem);
int temCicloDirecionado(Grafo *g);
int temCicloNaoDirecionado(Grafo *g);
void testarAlgoritmos(Grafo *g, const char *nome, int direcionado);

#endif
