#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include "algoritmos.h"

int main(){
    Grafo *g1 = lerGrafo("grafo1.txt", 0);
    Grafo *g2 = lerGrafo("grafo2.txt", 1);
    Grafo *g3 = lerGrafo("grafo3.txt", 1);
    Grafo *g4 = lerGrafo("grafo4.txt", 1);

    imprimirGrafo(g1);
    imprimirGrafo(g2);
    imprimirGrafo(g3);
    imprimirGrafo(g4);

   

    testarAlgoritmos(g1, "Grafo 1", 0);
    testarAlgoritmos(g2, "Grafo 2", 1);
    testarAlgoritmos(g3, "Grafo 3", 1);
    testarAlgoritmos(g4, "Grafo 4", 1);
    
 
componentesFortementeConexos(g1, 0);
componentesFortementeConexos(g2, 1);
componentesFortementeConexos(g3, 1);
componentesFortementeConexos(g4, 1);
    
    
    liberarGrafo(g1);
    liberarGrafo(g2);
    liberarGrafo(g3);
    liberarGrafo(g4);
    
    
    testarGrafoGrande();

    return 0;
}
