#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct arvore{
    unsigned valor;
    struct arvore *left;
    struct arvore *right;
}Tree;

Tree *RotateLeft(Tree *root){
    Tree *NewRootR = root->right;
    Tree *NewRootLeft = NewRootR->left;
    NewRootR->left = root;
    root->right = NewRootLeft;
    return NewRootR;
}
Tree *RotateRight(Tree *root){
    Tree *NewRootL = root->left;
    Tree *NewRootRight = NewRootL->right;
    NewRootL->right = root;
    root->left = NewRootRight;
    return NewRootL;
}

Tree *BST_search(Tree *root, unsigned valor, int *prf, int *rotacao){ // vai retornar o ponteiro para o no com valor procurado
    if(root == NULL){
        printf("%d\n", *prf);
        *rotacao = 0;
        return NULL;
    }
    else if(valor == root->valor){
        printf("%d\n", *prf);
        return root;
    }
    else if(valor < root->valor){
        (*prf)++;
        root->left = BST_search(root->left, valor, prf, rotacao);
        if(*rotacao == 1){
            root = RotateRight(root);
        }
        *prf = 0;
        return root;
    }
    else{
        (*prf)++;
        root->right = BST_search(root->right, valor, prf, rotacao);
        if(*rotacao == 1){
            root = RotateLeft(root);
        }
        *prf = 0;
        return root;
    }
}

Tree *list_insert(Tree *root, unsigned valor, int *prf, int *rotacao){// vai retornar a nova raiz apos a inserção
    Tree *New;
    if(root == NULL){
        New = (Tree*) malloc(sizeof(Tree));
        if(New == NULL){
            exit(1);
        }
        New->valor = valor;
        New->left =  NULL;
        New->right = NULL;
        printf("%d\n", *prf);
        return New;
    }
    else if(valor < root->valor){
        (*prf)++;//profundidade
        root->left = list_insert(root->left, valor, prf, rotacao);
        root = RotateRight(root);
        *prf = 0;
        return root;
    }
    else if(valor > root->valor){
        (*prf)++;
        root->right = list_insert(root->right, valor, prf, rotacao);
        root = RotateLeft(root);
        *prf = 0;
        return root;
    }
    else{
        BST_search(root, valor, prf, rotacao);//caso o valor ja esteja inserido na arvore
    }
}

Tree *BST_delete_min(Tree *root, int *valor){
    Tree *raiz, *l;
    int Val;
    if(root->left == NULL){
        raiz = root->right;
        Val = root->valor;
        free(root);
        *valor = Val;
        return raiz;
    }
    else{ 
        l = BST_delete_min(root->left, valor);
        root->left = l;
        return root;
    }
}

Tree *BST_delete(Tree *root, int valor, int *prf, int *rotacao, int *DEL, int *DELFATHER){
    if(root == NULL){
        printf("%d\n", *prf);
        *rotacao = 0;
        return NULL;
    }
    else if(valor < root->valor){
        (*prf)++;
        root->left = BST_delete(root->left, valor, prf, rotacao, DEL, DELFATHER);
        if(root->left == NULL || root->left->valor == *DEL){
            *DELFATHER = root->valor;
        }
        else if(*rotacao == 1 && root->left->valor == *DELFATHER){
            root = RotateRight(root);
        }
        *prf = 0;
        return root;
    }
    else if(valor> root->valor){
        (*prf)++;
        root->right = BST_delete(root->right, valor, prf, rotacao, DEL, DELFATHER);
        if(root->right == NULL || root->right->valor == *DEL){
            *DELFATHER = root->valor;
        }
        else if(*rotacao == 1 && root->right->valor == *DELFATHER){
            root = RotateLeft(root);
        }
        *prf = 0;
        return root;
    }
    else{// valor ser igual ao valor da raiz
        printf("%d\n", *prf);
        if(root->left == NULL){
            Tree *r;
            r = root->right;
            free(root);
            if(r != NULL){
                *DEL = r->valor;
            }
            return r;
        }
        else if(root->right == NULL){
            Tree *l;
            l = root->left;
            free(root);
            if(l != NULL){
                *DEL = l->valor;
            }
            return l;
        }
        else{
            root->right = BST_delete_min(root->right, &valor);
            root->valor = valor;
            *DEL = root->valor;
            return root;
        }
    }
}

int main(){
    char entrada[4];
    unsigned valorNo;
    int D = 0, rotacao;
    int del, delfather;
    Tree *root = NULL;
    while(scanf("%s", entrada) && strcmp(entrada, "END") != 0){
        rotacao = 1;
        if(strcmp(entrada, "INS") == 0){
            scanf("%u", &valorNo);
            root = list_insert(root, valorNo, &D, &rotacao);
        }
        else if(strcmp(entrada, "FND") == 0){
            scanf("%u", &valorNo);
            root = BST_search(root, valorNo, &D, &rotacao);
        }
        else if(strcmp(entrada, "DEL") == 0){
            scanf("%u", &valorNo);
            root = BST_delete(root, valorNo, &D, &rotacao, &del, &delfather);
        }
    }
}