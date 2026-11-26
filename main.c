#include "grafo.h"
#include <stdio.h>

int main(){
    Grafo *g1 = lerGrafo("grafo1.txt", 0);
    Grafo *g2 = lerGrafo("grafo2.txt", 1);

    imprimirGrafo(g1);
    imprimirGrafo(g2);

    liberarGrafo(g1);
    liberarGrafo(g2);


    return 0;
}