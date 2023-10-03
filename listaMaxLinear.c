#include <stdio.h>
#include <stdlib.h>

//programacao dinamica
void PrintarIndices(int *ind, int m){
    int count = 0;
    for(int i = 0; i < m; i++){
        if(count == m - 1){// o ultimo n pode ter espaço, erro de apresentacao
            printf("%d", ind[i]);
        }
        else{
            printf("%d ", ind[i]);
        }
        count++;
    }
    printf("\n\n");
}
int main(){
    int n, m;
    while(scanf("%d %d", &n, &m) != EOF){
        int *Variaveis  = (int*)calloc(n, sizeof(int));
        int *Constantes = (int*)calloc(m, sizeof(int));
        int matriz[m + 1][n + 1];
        int *ind = (int*)calloc(m, sizeof(int));//indices
        int temp = m;
        for(int i = 0; i < n; i++){
            scanf("%d", &Variaveis[i]);
        }
        for(int i = 0; i < m; i++){
            scanf("%d", &Constantes[i]);
        }
        for(int i = 0; i < m + 1; i++){
            for(int j = 0; j < n + 1; j++){
                int mult = Variaveis[j-1]*Constantes[i-1];//multiplica o antecessor da variavel e o antecessor da constante
                int VDSE = matriz[i-1][j-1]; // valor que esta na digonal superior a esquerda
                int Vesquerda = matriz[i][j-1];// valor a esquerda
                if(i == 0 || j == 0 || j < i){
                    matriz[i][j] = 9999999;
                }
                else{
                    if(VDSE == 9999999 && Vesquerda == 9999999){
                        matriz[i][j] = mult;
                    }
                    else if(VDSE == 9999999){
                        if(Vesquerda > mult){
                            matriz[i][j] = Vesquerda;
                        }
                        else{
                            matriz[i][j] = mult;
                        }
                    }
                    else if(Vesquerda == 9999999){
                        matriz[i][j] = VDSE + mult; 
                    }
                    else{
                        if(Vesquerda > VDSE + mult){
                            matriz[i][j] = Vesquerda;
                        }
                        else{
                            matriz[i][j] = VDSE + mult;
                        }
                    }
                }
            }
        }
        for(int i = n; i >= 0; i--){
            if(temp != 0  && matriz[temp][i-1] != matriz[temp][i]){
                ind[temp-1] = i - 1;// pegar os indices que levaram ate o valor final da matriz de programacao dinamica 
                temp--;
            }
        }
        printf("%d\n", matriz[m][n]);// valor final
        PrintarIndices(ind, m);
    }
    return 0;
}