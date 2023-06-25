#include <stdio.h>
#include<malloc.h>

typedef struct Wezel{
    int key;
    struct Wezel *parent;
    int rank;
} Wezel, *WskWezel;

Wezel makeSet(int k){
    WskWezel x = (WskWezel)malloc(sizeof(Wezel));
    x->key=k;
    x->parent=x;
    x->rank=0;
    return *x;
}

WskWezel findSet(WskWezel x){
    if(x!=x->parent) 
        x->parent=findSet(x->parent);
    return x->parent;
}

void unio(WskWezel x, WskWezel y){
    if(x->rank < y->rank) 
        x->parent=y;
    else{
        y->parent=x;
        if(x->rank==y->rank)
            x->rank=x->rank+1;
    }
}

void printNode(WskWezel x){
    printf("-> %d ", x->key);
    if(x->parent!=x)
        printNode(x->parent);
}

int main(){
    Wezel Z[10];
    for(int i=0; i<10; i++){
        Z[i]=makeSet(i);
    }

    unio(findSet(Z),    findSet(Z+1));
    unio(findSet(Z+2),  findSet(Z+3));
    unio(findSet(Z+1),  findSet(Z+2));
    unio(findSet(Z+5),  findSet(Z+6));
    unio(findSet(Z+7),  findSet(Z+8));
    unio(findSet(Z+3),  findSet(Z+5));
    unio(findSet(Z),    findSet(Z+7));
    
    int nr=6;
    for (nr=0; nr<10; nr++){
    printf("%d par: %d\n", Z[nr].key, Z[nr].parent->key);

    printf("Sciezka %d: \n\t",nr);
    printNode(Z+nr);
    printf("\n");
    }
    
    return 0;
}