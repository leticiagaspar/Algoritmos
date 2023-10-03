#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct conexao{
    int T;
    unsigned C;
}Conexao;

typedef struct{
    Conexao *vetor;
    int tam;
}Hashtable;

int funcaoHash(int chave, int tam){
    return chave % tam;
}
int HoarePartition(Conexao *vet, int l, int r){
    int pivo = (l + r + (l+r)/2)/3;
    int i = l, j = r-1;
    Conexao Temp;
    Temp = vet[l];
    vet[l] = vet[pivo];
    vet[pivo] = Temp;
    while(i <= j){
        while(i < r && vet[i].T <= vet[l].T){
            i++;
        }
        while(j > l && vet[j].T > vet[l].T){
            j--;
        }
        if(i < j){
            Temp = vet[i];
            vet[i] = vet[j];
            vet[j] = Temp;
        }
    }
    Temp = vet[l];
    vet[l] = vet[j];
    vet[j] = Temp;
    return j;
}

void quickSort(Conexao *vet, int l, int r){
    int s;
    if(r-l > 1){
        s = HoarePartition(vet, l , r);
        quickSort(vet, l, s);
        quickSort(vet, s+1, r);
    }
}

int binarySearch(Conexao *vetor, int timestamp, int tam){ 
    int l = 0, r = tam-1, m;
    while(l<=r){// enquanto o inicio for menor ou igual que o fim, pois se o inicio for maior que o fim, deve-se retornar -1
        m = (l+r)/2;
        if(timestamp == vetor[m].T){
            return m;
        }else if(timestamp < vetor[m].T){
            r = m-1;
        }else{
            l = m+1;
        }
    }
    return -1;
}

void buscaCliente(Hashtable *tabela, int timestamp, int tam){
    int posicao = funcaoHash(timestamp, tam);// posicao na tabela
    int posVetor = binarySearch(tabela[posicao].vetor, timestamp, tabela[posicao].tam);
    if(posVetor != -1){
        
        printf("%u %d\n", tabela[posicao].vetor[posVetor].C, posVetor);
        
    }
    else{
        printf("-1 -1\n");
    }
}
void hashing(Hashtable *tabela, int timestamp, unsigned cliente, int M){
    int posicao = funcaoHash(timestamp, M);
    if(tabela[posicao].tam == 0){
        tabela[posicao].vetor = (Conexao*) malloc(sizeof(Conexao));
    }else{
    tabela[posicao].vetor = (Conexao*) realloc(tabela[posicao].vetor, (tabela[posicao].tam + 1) * sizeof(Conexao));
        if(tabela[posicao].vetor == NULL){
            exit(1);
        }
    }
    tabela[posicao].vetor[tabela[posicao].tam].T = timestamp;
    tabela[posicao].vetor[tabela[posicao].tam].C = cliente;
    tabela[posicao].tam++;
    printf("%d %d\n", posicao, tabela[posicao].tam);
}
Hashtable *rehashing(Hashtable *tabela, int M){
    int NovoTam = 2*M + 1;
    Hashtable *tabelaNova = (Hashtable*) calloc(NovoTam, sizeof(Hashtable));
    if(tabelaNova == NULL){
        exit(1);
    }
    for(int i = 0; i < M; i++){
        tabelaNova[i].vetor = NULL;
        tabelaNova[i].tam = 0;
    }
    for(int i = 0; i < M; i++){
        for(int j = 0; j < tabela[i].tam; j++){
            int posicao = funcaoHash(tabela[i].vetor[j].T, NovoTam);
            if(tabelaNova[posicao].tam == 0){
                tabelaNova[posicao].vetor = (Conexao*) malloc(sizeof(Conexao));
            }else{
                tabelaNova[posicao].vetor = (Conexao*) realloc(tabelaNova[posicao].vetor, (tabelaNova[posicao].tam + 1) * sizeof(Conexao));
                if(tabelaNova[posicao].vetor == NULL){
                    exit(1);
                }
            }
            tabelaNova[posicao].vetor[tabelaNova[posicao].tam].T = tabela[i].vetor[j].T;
            tabelaNova[posicao].vetor[tabelaNova[posicao].tam].C = tabela[i].vetor[j].C;
            tabelaNova[posicao].tam++;
            quickSort(tabelaNova[posicao].vetor, 0, tabelaNova[posicao].tam);
        }
        free(tabela[i].vetor);
    }
    free(tabela);
    tabela = tabelaNova;
    tabelaNova = NULL;
    return tabela;
}

int main(){
    int M, timestamp;
    unsigned cliente;
    float Lmax;
    char evento[4];
    int elementos = 0;
    float FatorCarga;
    scanf("%d %f", &M, &Lmax);
    Hashtable *tabela;
    tabela = (Hashtable*) calloc(M, sizeof(Hashtable));
    if(tabela == NULL){
        exit(1);
    }
    for(int i = 0; i < M; i++){
        tabela[i].tam = 0;
        tabela[i].vetor = NULL;
    }
    while(scanf(" %s", evento) && strcmp(evento, "END") != 0){
        if(strcmp(evento, "NEW") == 0){
            scanf("%d %u", &timestamp, &cliente);
            FatorCarga = (float) elementos/M;
            if(FatorCarga > Lmax){
                tabela = rehashing(tabela, M);
                M = 2*M + 1;
            }
            hashing(tabela, timestamp, cliente, M);
            elementos++;
        }
        if(strcmp(evento, "QRY") == 0){
            scanf("%d", &timestamp);
            buscaCliente(tabela, timestamp, M);
        }
    }
    return 0;
}
