#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node{
    int key, count;
    struct Node *left, *right, *parent;
}Node, *NodePtr;

NodePtr create(int liczba){
    NodePtr nd=(NodePtr)malloc(sizeof(Node));
    nd->key  = liczba;
    nd->count  = 1;
    nd->left=nd->right=nd->parent=NULL;
    return nd;
}

NodePtr insert(NodePtr root, int x){
    bool inserted=false;
    if(root==NULL){
        root=create(x);
        inserted=true;
    }
    NodePtr temp = root;
    while(!inserted){
        if(x>temp->key){
            if(temp->right==NULL){
                NodePtr new = create(x);
                temp->right = new;
                new->parent = temp;
                inserted=true;
            }
            else
                temp = temp->right;
        }
        else if(x<temp->key){
            if(temp->left==NULL){
                NodePtr new=create(x);
                temp->left=new;
                new->parent=temp;
                inserted=true;
            }
            else
                temp=temp->left;
        }
        else{
            (temp->count)++;
            inserted=true;
        }
    }
    return root;
}

void print(NodePtr x){
    if(x != NULL){
        print(x->left);
        if(x->parent != NULL)
            printf(" %d(%d)", x->key, x->count);
        else
            printf(" { %d(%d) }", x->key, x->count);

        print(x->right);
    }
}

NodePtr search(NodePtr root, int x){
    NodePtr found=root;
    while(found!=NULL && found->key!=x){
        if(x<found->key) found=found->left;
        else found=found->right;
    }

    return found;
}

NodePtr minimum(NodePtr x){
    while(x->left!=NULL) x=x->left;
    return x;
}

NodePtr delete(NodePtr root, NodePtr toDelete){
    if(toDelete->left==NULL && toDelete->right==NULL){
        if(toDelete==root) root=NULL;
        else{
            if(toDelete==toDelete->parent->left) toDelete->parent->left=NULL;
            else toDelete->parent->right=NULL;
        }
    }else{
        if(toDelete->left!=NULL && toDelete->right!=NULL){
            NodePtr y=minimum(toDelete->right);
            toDelete->key=y->key;
            toDelete->count=y->count;
            delete(root, y);
        }
        else{
            if(toDelete->left!=NULL){
                toDelete->left->parent=toDelete->parent;
                if(toDelete==root){
                    root=toDelete->left;
                }else{
                    if(toDelete==toDelete->parent->left) toDelete->parent->left=toDelete->left;
                    else toDelete->parent->right=toDelete->left;
                }
            }else{
                toDelete->right->parent=toDelete->parent;
                if(toDelete==root) root=toDelete->right;
                else{
                    if(toDelete==toDelete->parent->left) toDelete->parent->left=toDelete->right;
                    else toDelete->parent->right=toDelete->right;
                }
            }
        }
    }
    return root;
}

int main(){
    NodePtr root=NULL;
    int choice=1;
    while(choice!=0){
        printf("Co chcesz zrobić?\n");
        printf("\t0 - WYJDZ\n");
        printf("\t1 - WSTAW\n");
        printf("\t2 - SZUKAJ\n");
        printf("\t3 - USUN\n");
        printf("\t4 - DRUKUJ\n: ");
        scanf("%d",&choice);

        switch (choice)
        {
        case 1:
            printf("Co wstawić?\n: ");
            int toInsert;
            scanf("%d", &toInsert);
            root=insert(root,toInsert);
            break;
        case 2:
            printf("Podaj element do szukania\n: ");
            int toSearch;
            scanf("%d", &toSearch);
            NodePtr searched=search(root,toSearch);
            if(searched==NULL) printf("Nie znaleziono\n");
            else{
                printf("Znaleziono element %d. Ilosc wystapien: %d\n", searched->key, searched->count);
            }
            break;
        case 3:
            printf("Co usunac?\n: ");
            int toDelete;
            scanf("%d", &toDelete);
            NodePtr nodeToDelete=search(root, toDelete);
            if(nodeToDelete!=NULL) {
                if(nodeToDelete->count==1) root=delete(root, nodeToDelete);
                else (nodeToDelete->count)--;
            }
            else printf("Nie ma takiego wezla\n");
            break;
        case 4:
            print(root);
            break;
        
        }
    }
    free(root);
    return 0;
}