#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct arestas{
    int val, tamanho, vertice2;
    struct arestas *prox;
}Aresta;

typedef struct graph{
    Aresta *head;
    Aresta *tail;
    Aresta *head2;
    Aresta *tail2;
    int tam2;
}Graph;

typedef struct heap{
    int vertice;
    int distancia;
}Heap;

typedef struct restaurante{
    int restaurant;
    int *ifood;
}Restaurante;

Heap *bubble_up(Heap *heap, int indice){
    int i = indice;
    Heap aux;
    while(i > 0 && heap[i].distancia <= heap[(i - 1)/2].distancia){
        aux = heap[i];
        heap[i] = heap[(i - 1)/2];
        heap[(i - 1)/2] = aux;
        i = (i - 1)/2;
    }
    return heap;
}

Heap *heapInsert(Heap *heap, int localidade, int distancia, int *heapsize, int *tamanhoAtual){
    if((*heapsize) == (*tamanhoAtual)){
        (*heapsize) = (*heapsize)*2;
        heap = (Heap*) realloc(heap, (*heapsize) * sizeof(Heap));
    }
    //printf("aquiii\n");
    heap[(*tamanhoAtual)].vertice = localidade;
    heap[(*tamanhoAtual)].distancia = distancia;
    heap = bubble_up(heap, (*tamanhoAtual));
    (*tamanhoAtual)++;
    return heap;
}

Heap *bubble_down(Heap *heap, int indice, int tamanhoAtual){
    int l = 2*indice + 1;//filho a esquerda
    int r = 2*indice + 2;//filho a direita
    int m = indice;
    Heap aux;
    if(l < tamanhoAtual && heap[l].distancia <= heap[m].distancia){
        m = l;
    }
    if(r < tamanhoAtual && heap[r].distancia <= heap[m].distancia){
        m = r;
    }
    if(m != indice){
        aux = heap[m];
        heap[m] = heap[indice];
        heap[indice] = aux;
        return bubble_down(heap, m, tamanhoAtual);
    }
    return heap;
}

int heap_extract(Heap *heap, int *distancia, int *tamanhoAtual){
    Heap r = heap[0];
    Heap aux;
    if((*tamanhoAtual) > 0){
        aux = heap[0];
        heap[0] = heap[(*tamanhoAtual) - 1];
        heap[(*tamanhoAtual) - 1] = aux;
        (*tamanhoAtual)--;
        heap = bubble_down(heap, 0, (*tamanhoAtual));
    }
    *distancia = r.distancia;
    return r.vertice;
}

Heap *heapUpdate(Heap *heap, int distancia, int valor, int tamanhoAtual, int *n){
    for(int i = 0; i < tamanhoAtual; i++){// heapUpdate
        if(heap[i].vertice == valor){
            heap[i].distancia = distancia;
            heap = bubble_up(heap, i);
            (*n) = 1;
        }
    }   
    return heap;
}

int *dikjstra(Graph *graph, int X, int Y){
    int *distancia = (int*) malloc(X*sizeof(int));
    Heap *heap = (Heap*) malloc(sizeof(Heap)); 
    Aresta *grafo = NULL;
    int heapsize = 1;
    int tamanhoAtual = 0;
    int n, u, dist, valor, weight;
    for(int i = 0; i < X; i++){
        distancia[i] = -1;
    }
    distancia[Y] = 0;
    heap = heapInsert(heap, Y, 0, &heapsize, &tamanhoAtual);
    
    for(int i = 0; i < X; i++){
        u = heap_extract(heap, &dist, &tamanhoAtual);
        grafo = graph[u].head;
        while(grafo != NULL){
            valor = grafo->val;
            weight = grafo->tamanho;
            if(dist + weight < distancia[valor] || distancia[valor] == -1){
                distancia[valor] = dist + weight;
                n = 0;
                heap = heapUpdate(heap, distancia[valor], valor, tamanhoAtual, &n);
                if(n == 0){
                    heap = heapInsert(heap, valor, distancia[valor], &heapsize, &tamanhoAtual);
                }
                
            }
            grafo = grafo->prox;
        }
    }
    return distancia;
}

Aresta *inserirfila(Graph *fila, int num, int num2, int L){ // inserindo as listas de arestas
    Aresta *novo = (Aresta*) malloc(sizeof(Aresta));
    if(novo == NULL){
        printf("erro ao alocar memoria\n");
        exit(1);
    }
    else{
        novo->val = num;
        novo->tamanho = L;
        novo->prox = NULL;
        if(fila[num2].head == NULL){
            fila[num2].head = novo;
            fila[num2].tail = novo;
        }
        else{
            fila[num2].tail->prox = novo;
            fila[num2].tail = novo;
        }
    }
    return fila[num2].head;
}

//Teste com sentinela
Aresta *InserePedidos(Aresta *list, int num, int Localidade, int *qntPedidos){ //inserindo na lista
    Aresta *novo = (Aresta*) malloc(sizeof(Aresta));
    novo->prox = NULL;
    novo->val = num;
    novo->vertice2 = Localidade;
    list->prox = novo;
    (*qntPedidos)++;
    return list->prox;
}

Aresta *list_delete(Aresta *list){
    Aresta *remover = list->prox;
    list->prox = remover->prox;
    free(remover);
    return list;
}

Aresta *CancelaPedidos(Aresta *prim, Aresta **fim, int *qntPedidos){
    prim = list_delete(prim);
    if(prim->prox == NULL){
        *fim = prim;
    }
    (*qntPedidos)--;
    return prim;
}


int main(){
    int localidade, vias, lojas, entregadores;
    int A, B, L;
    char entrada[4];
    int ID, Z, D;
    int localRestaurante;
    int distancia;
    Aresta *graf = NULL;
    Aresta *localidade1 = NULL;
    scanf("%d %d %d %d", &localidade, &vias, &lojas, &entregadores);
    Graph *graph = (Graph*) calloc(localidade, sizeof(Graph));
    int *localEntrega = (int*) calloc(entregadores, sizeof(int));
    Restaurante *restaurante = (Restaurante*) calloc(lojas, sizeof(Restaurante));
    Graph pedidos;
    Aresta *aux;
    for(int i = 0; i < localidade; i++){
        graph[i].head = NULL;
        graph[i].tail = NULL;
    }
    //inicializar fila de pedidos com sentinela
    pedidos.tam2 = 0;
    pedidos.head2 = (Aresta*) malloc(sizeof(Aresta));
    pedidos.head2->prox = NULL;
    pedidos.tail2 = pedidos.head2;

    for(int i = 0; i < vias; i++){
        scanf("%d %d %d", &A, &B, &L);
        graph[A].head = inserirfila(graph, B, A, L);
        graph[B].head = inserirfila(graph, A, B, L);
    }

    int Y[lojas];
    for(int i = 0; i < lojas; i++){
        scanf("%d", &Y[i]);
        restaurante[i].restaurant = Y[i];
        restaurante[i].ifood = dikjstra(graph, localidade, Y[i]);
    }
    for(int i = 0; i < entregadores; i++){
        localEntrega[i] = Y[0];
    }
    while(scanf(" %s", entrada) && strcmp(entrada, "END") != 0){
        if(strcmp(entrada, "NEW") == 0){
            scanf("%d %d", &ID, &Z);
            pedidos.tail2 = InserePedidos(pedidos.tail2, ID, Z, &pedidos.tam2);
            printf("+%d %d\n", ID, pedidos.tam2);
        }
        else if(strcmp(entrada, "CEL") == 0){
            scanf("%d", &ID);
            //pedidos.head2 = CancelaPedidos(pedidos.head2, &pedidos.tail2, &pedidos.tam2, ID);
            Aresta *aux = pedidos.head2;
            while(aux->prox != NULL && aux->prox->val != ID){
                aux = aux->prox;
            }
            if(aux->prox != NULL){
                aux = CancelaPedidos(aux, &pedidos.tail2, &pedidos.tam2);;
            }
            printf("-%d %d\n", ID, pedidos.tam2);
        }
        else if(strcmp(entrada, "DLV") == 0){
            scanf("%d", &D);
            for(int i = 0; i < lojas; i++){
                int r = restaurante[i].ifood[localEntrega[D]];
                int t = restaurante[i].ifood[pedidos.head2->prox->vertice2];
                if(i == 0 || distancia > (r + t)){
                    distancia = r + t;
                    localRestaurante = restaurante[i].restaurant;
                }
            }
            int valor = pedidos.head2->prox->val;
            int entrega = pedidos.head2->prox->vertice2;
            printf("%d %d %d %d %d\n", valor, localEntrega[D], localRestaurante, entrega, distancia);
            localEntrega[D] = pedidos.head2->prox->vertice2;
            pedidos.head2 = CancelaPedidos(pedidos.head2, &pedidos.tail2, &pedidos.tam2);
        }
    }
    return 0;
}