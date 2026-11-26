#include "grafo.h"
#include <stdio.h>

Grafo *criarGrafo(int V){
    if(V<=0) return NULL; //não faz sentido criar gráfo sem vértices 

    Grafo *g = (Grafo*)malloc(sizeof(Grafo)); //aloca memória para o grafo
    if(g == NULL) return NULL;
    g->V = V; //define num de vértices do grafo
    g->A = 0; //num arrestas inicalmente do grafo é 0
    g->lista = (No**)malloc(V * sizeof(No*)); //aloca memória p lista de adjacÊncia
    if(g->lista == NULL){ //erro ao alocar
        free(g); 
        return NULL;
    }
    for(int i=0; i<V; i++){ //cada vértice da lista de adjacÊncia aponta p NULL pq ñ tem arrestas inicialmente
        g->lista[i]=NULL;
    }
    return g;
}

void addArresta(Grafo *g, int origem, int destino, int peso, int direcionado){
    if(g==NULL || origem<0 || origem>=g->V || destino<0 || destino >=g->V) return; //verifica se o gráfico é válido

    No *novo = (No*)malloc(sizeof(No));
    if(novo==NULL) return;
    novo->destino = destino;
    novo->peso = peso;
    novo->prox = NULL;

    //Inserção ordenado por destino (crescente)
    //insere o novo no INICIO da lista de adjacência
    if(g->lista[origem]==NULL || destino < g->lista[origem]->destino){ //lista (cabeça) vazia ou destino menor que a cabeça da lista
        novo->prox = g->lista[origem]; //novo aponta p cabeça da lista
        g->lista[origem] = novo; //novo agr é a cabeça da lista
    }

    //Buscar onde inserir corretamente
    else{
        No *ant = NULL;
        No *atual = g->lista[origem]; //cabeça da lista
        
        //percorre a lista até encontra o destino correto (destino > atual->destino)
        while(atual!=NULL && destino > atual->destino){
            ant = atual; //anterior passa a ser o atual
            atual = atual->prox; //avança para o próximo nó
        }

        //se o destino já existe não irá inserir novsamente
        if(atual!=NULL && destino == atual->destino){
            free(novo);
            return;
        }

        //insere o novo nó entre ant e atual
        ant->prox = novo;
        novo->prox = atual;
    }

    //verifica se é ou ñ direcionado
    if(direcionado==0){ //não direcionado, terá tmb arresta (destino->origem), além dá q já tem (origem->destino)
        addArresta(g, destino, origem, peso, 1); //direcionado=1 p ñ ter loop infinito
        g->A++; //incrementa numero de arrestas do grafo ñ direcionado
    }
    if(direcionado==1) g->A++; //incrementa numero de arrestas do grafo direcionado
}

Grafo *lerGrafo(const char *nomeArq, int direcionado){
    //*nomeArq -> endereço de memória do primeiro caractere do arquivo .txt
    FILE *arq = fopen(nomeArq, "r");
    if(arq==NULL){
        printf("Erro ao abrir o arquivo %s\n", nomeArq);
        return NULL;
    } 

    int V, A; //variaveis para guardar o num de V e A vindas do arquivo
    if(fscanf(arq, "%d %d", &V, &A) != 2){ //lê o dois primeiro inteiros do arq e vê se foi lido 2 valores, se ñ...
        printf("Erro ao ler o número de vértices e arestas do arquivo");
        fclose(arq);
        return NULL;
    }

    Grafo *g = criarGrafo(V); //cria grafo com num de Vertices lido do arqivo
    if(g==NULL){
        printf("Erro ao criar o grafo");
        fclose(arq);
        return NULL;
    }
    for(int i=0; i<A; i++){
        int origem, destino, peso; //variáveis para armazenar cada valor por linha do arq
        if(fscanf(arq, "%d %d %d", &origem, &destino, &peso) != 3){ //verifica s leu 3 valores na linha(arresta), se ñ...
            printf("Erro ao ler as arestas do arquivo");
            fclose(arq);
            liberarGrafo(g);
            return NULL;
        }
        addArresta(g, origem, destino, peso, direcionado);//insere a arresta na lista ordenada conforme o grafo, se é ou ñ direcionado
    }
    fclose(arq);
    return g; //devolve o grafo criado e preenchido com os valores do arquivo
}

void imprimirGrafo(Grafo *g){
    if(g==NULL){
        printf("Grafo vazio\n");
        return;
    }

    printf("Número de vértices do grafo: %d\n", g->V);
    printf("Número de arestas do grafo: %d\n", g->A);

    printf("======== GRAFO (lista adjacência) ========\n");
    for(int i=0; i<g->V; i++){//percorre ccada vértice da lista de adjacência
        printf("Vértice %d: ", i);
        No *atual = g->lista[i]; //cabeça da lista de adj de vertice i
        while(atual!=NULL){//percorre a lista
            printf(" -> (destino: %d, peso: %d)", atual->destino, atual->peso);
            atual = atual->prox; //avança para o prox nó
        }
        printf("\n");
    }
}

void liberarGrafo(Grafo *g){
    if(g==NULL) return;

    for(int i=0; i<g->V; i++){ //percorre cada vértice da lista de adj
        No *atual = g->lista[i]; //cabeça da lista do vértice i
        while(atual!=NULL){ //percorre a lista de adj
            No *temp = atual; //guarda o nó atual
            atual = atual->prox; //avança o nó
            free(temp); //libera o nó atual que foi guardado
        }
    }
    free(g->lista); //libera a lista de adjac~encia
    free(g); //libera o grafo
}