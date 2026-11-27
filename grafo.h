#ifndef GRAFO_H
#define GRAFO_H

typedef struct No { 
int destino;        //diz quem é o vizinho alcançado
int peso;           //valor da arresta
struct No *prox;   //ponteiro para nó
} No; 

typedef struct { 
int V;          //num vértices
int A;          //num arrestas
No **lista;     //lista de adjacência
} Grafo; 

Grafo *criarGrafo(int V);
void addArresta(Grafo *g, int origem, int destino, int peso, int direcionado);
Grafo *lerGrafo(const char *nomeArq, int direcionado);
void imprimirGrafo(Grafo *g);
void liberarGrafo(Grafo *g);

#endif
