#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct carta{
    int valor;
    char naipe;
    struct carta *prox;
}Carta;

typedef struct fila{
    Carta *head;
    Carta *tail;
    int quantCartas;
    int saida;
}Jogador;

typedef struct pilha{
    Carta *head2;
    int quantCartas;
}Cuscuz;

Carta *enfileiraCarta(Carta *carta, int valor, char naipe, int *quantCartas){
    Carta *novo = (Carta*) malloc(sizeof(Carta));
    novo->valor = valor;
    novo->naipe = naipe;
    novo->prox = NULL;
    carta->prox = novo;
    (*quantCartas)++;
    return carta->prox;
}

Carta desenfileiraCarta(Carta** head, Carta** tail, int *quantCartas){
    Carta cartaRemovida;
    Carta *remover = (*head)->prox;
    (*head)->prox = remover->prox;
    remover->prox = NULL;
    cartaRemovida = (*remover);
    free(remover);
    if((*head)->prox == NULL){
        *tail = *head;
    }
    (*quantCartas)--;
    return cartaRemovida;
}

Carta *empilhaCuscuz(Carta *cuscuz, int valor, char naipe, int *quantCartas){
    Carta *novo = (Carta*) malloc(sizeof(Carta));
    novo->valor = valor;
    novo->naipe = naipe;
    novo->prox = cuscuz->prox;
    cuscuz->prox = novo;
    (*quantCartas)++;
    return cuscuz;
}
Carta desempilhaCuscuz(Carta **cuscuz, int *quantCartas){
    Carta *aux, CartaDesempilhada;
    aux = (*cuscuz)->prox;
    (*cuscuz)->prox = aux->prox;
    aux->prox = NULL;
    CartaDesempilhada = (*aux);
    free(aux);
    (*quantCartas)--;
    return CartaDesempilhada;//retorna a carta removida para add ao jogador ganhador da rodada
}

int compararValor(int *ganhador, int empate, Jogador *jogadores, int N){// verifica se os valores das cartas sao iguais
    for(int i = 0; i < N; i++){
        int e = jogadores[i].saida;
        if(e == 0 && *ganhador == -1){
            *ganhador = i;
        }
        if(e == 0 && jogadores[i].head->prox->valor > jogadores[*ganhador].head->prox->valor){// verifica se o valor da carta de um jogador eh maior que o do outro
            *ganhador = i;
        }
    }
    for(int i = 0; i < N; i++){
        if(jogadores[i].saida == 0 && i != *ganhador && jogadores[i].head->prox->valor == jogadores[*ganhador].head->prox->valor){
            empate = 1;
        }
    }
    return empate;
}

int vencedor(Jogador *jogadores, int N){
    int ganhador = -1, empate = 0;
    empate = compararValor(&ganhador, empate, jogadores, N);
    if(empate == 1){ // se houve empate dos valores, vai comparar os naipes
        for(int i = 0; i < N; i++){
            if((jogadores[i].saida == 0) && (i != ganhador) && jogadores[i].head->prox->valor == jogadores[ganhador].head->prox->valor){ // compara os naipes se os valores forem iguais
                if(jogadores[i].head->prox->naipe == 'P' &&  jogadores[ganhador].head->prox->naipe != 'P'){   // C < E < O < P
                    ganhador = i;
                    empate = 0;
                }
                else if(jogadores[i].head->prox->naipe == 'O' && jogadores[ganhador].head->prox->naipe != 'P' &&  jogadores[ganhador].head->prox->naipe != 'O'){
                    ganhador = i;
                    empate = 0;
                }
                else if(jogadores[i].head->prox->naipe == 'E' && jogadores[ganhador].head->prox->naipe != 'P'  && jogadores[ganhador].head->prox->naipe != 'O' &&  jogadores[ganhador].head->prox->naipe != 'E'){
                    ganhador = i;
                    empate = 0;
                }
                else if(jogadores[i].head->prox->naipe == jogadores[ganhador].head->prox->naipe){
                    empate = 1;
                }
                else{
                    empate = 0;
                }
            }
        }
        for(int i = 0; i < N; i++){
            if(jogadores[i].saida == 0 && i != ganhador && jogadores[i].head->prox->valor == jogadores[ganhador].head->prox->valor && jogadores[i].head->prox->naipe == jogadores[ganhador].head->prox->naipe){
                empate = 1;
            }
        }
    }
    if(empate == 1){
        return -1;
    }
    else{
        return ganhador;
    }
}
void mostrarCuscuz(Carta *cuscuz){ // printar pilha
    printf("\n\nCUSCUZ\n\n");
    Carta *aux = cuscuz->prox;
    while(aux){
        printf("Carta: %d%c\n", aux->valor, aux->naipe);
        aux = aux->prox;
    }
}

void ganhadorDaPartida(Jogador *jogadores, int N, int Ganhador){
    for(int i = 0; i < N; i++){
        int e = jogadores[i].saida;
        if(e == 0 && (Ganhador == -1 || jogadores[i].quantCartas > jogadores[Ganhador].quantCartas)){
            Ganhador = i;
        }
        if(e == 0 && jogadores[i].quantCartas == jogadores[Ganhador].quantCartas && i != Ganhador){
            Ganhador = -1;
        }
    }
    printf("%d\n", Ganhador);
}

int main(){
    int N;//quantidade de jogadores
    int valor, TerminoRodada = 0, Rodada = 0, Ganhador = -1, jogadoresEliminados = 0;
    char naipe;
    Carta removidaJogadores, removidaCuscuz;
    char entrada[4];
    //printf("digite a quantidade de jogadores: ");
    scanf("%d", &N);
    Jogador *jogadores = (Jogador*) calloc(N, sizeof(Jogador));
    Cuscuz *cuscuz = (Cuscuz*) calloc(1, sizeof(Cuscuz));
    for(int i = 0; i < N; i++){//inicializando fila
        jogadores[i].head = (Carta*) malloc(sizeof(Carta));//sentinela
        jogadores[i].head->prox = NULL;
        jogadores[i].tail = jogadores[i].head;
        jogadores[i].quantCartas = 0;
        jogadores[i].saida = 0;// ver se o jogador for eliminado
    }
    //inicializando pilha
    cuscuz->head2 = (Carta*) malloc(sizeof(Carta));//sentinela
    cuscuz->head2->prox = NULL;
    cuscuz->quantCartas = 0;
    while(scanf(" %s", entrada) && strcmp(entrada, "END") != 0){
        if(strcmp(entrada, "DEA") == 0){
            for(int i = 0; i < N; i++){
                if(jogadores[i].saida == 0){
                    scanf("%d%c", &valor, &naipe);
                    jogadores[i].tail = enfileiraCarta(jogadores[i].tail, valor, naipe, &jogadores[i].quantCartas);// distribuindo as cartas aos jogadores (fila)
                }
            }
        }
        for(int i = 0; i < N; i++){
            Carta *aux = jogadores[i].head->prox;
            //printf("jogador %d: ", i);
            while(aux != NULL){
                //printf("%d%c ", aux->valor, aux->naipe);
                aux = aux->prox;
            }
            //printf("\n");
        }
        if(strcmp(entrada, "RND") == 0){
            while(TerminoRodada == 0){
                Ganhador = vencedor(jogadores, N);
                //printf("ganhador = %d\n", Ganhador);
                for(int i = 0; i < N; i++){
                    if(jogadores[i].saida == 0){//se no jogador i ainda n foi eliminado
                        removidaJogadores = desenfileiraCarta(&jogadores[i].head, &jogadores[i].tail, &jogadores[i].quantCartas); //pega as cartas dos jogadores e passa para o cuscuz(pilha)
                        cuscuz->head2 = empilhaCuscuz(cuscuz->head2, removidaJogadores.valor, removidaJogadores.naipe, &cuscuz->quantCartas);//empilha as cartas no cuscuz, adicionando no topo(head) por ordem de jogada
                    }
                }
                //mostrarCuscuz(cuscuz->head2);
                if(Ganhador != -1){ // quando n eh empate
                    int qntCartas = cuscuz->quantCartas;
                    printf("%d %d %d\n", Rodada, Ganhador, cuscuz->quantCartas);
                    for(int i = 0; i < qntCartas; i++){
                        removidaCuscuz = desempilhaCuscuz(&cuscuz->head2, &cuscuz->quantCartas);//pega as cartas do cuscuz e leva pra o vencedor da rodada
                        jogadores[Ganhador].tail = enfileiraCarta(jogadores[Ganhador].tail, removidaCuscuz.valor, removidaCuscuz.naipe, &jogadores[Ganhador].quantCartas);// aqui p jogador da rodada recebe as cartas
                    }
                    TerminoRodada = 1;
                    //mostrarCuscuz(cuscuz->head2);
                }
                for(int i = 0; i < N; i++){
                    if(jogadores[i].quantCartas == 0 && jogadores[i].saida == 0){// quando um jogador n possui mais cartas ele eh eliminado da partida
                        jogadores[i].saida = 1;
                        jogadoresEliminados++;
                    }
                }
                if(jogadoresEliminados == N){// se a quantidade de eliminados for igual ao numero de jogadores (todos jogadores sem carta ao msm tempo) 
                    printf("-1 -1 -1\n");
                    TerminoRodada = 1;//termina a rodada
                }
            }
            Rodada++;
            TerminoRodada = 0;
        }
    }
    ganhadorDaPartida(jogadores, N, Ganhador);
    return 0;
}