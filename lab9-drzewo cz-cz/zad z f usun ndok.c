#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define RIGHT 1
#define LEFT 0
#define RED 1        /* stala oznaczajaca kolor wezla */
#define BLACK 0      /* stala oznaczajaca kolor wezla */
#define SZER_EKR 80  /* szerokosc ekranu */
#define IL_POZ   5   /* ilosc poziomow drzewa, ktore beda wydrukowane   */
                     /* gwiazdka bedzie sygnalizowac istnienie nizszych */
                     /* poziomow                                        */

typedef struct Node{
    int key;
    struct Node *left, *right, *parent;
    int color;
}Node, *NodePtr;

NodePtr nil; 

void nilInit(void){
/* funkcja inicjujaca nil; musi byc wywolana przed budowaniem drzewa */
  nil= (NodePtr) malloc(sizeof(Node));
  nil->parent = NULL;
  nil->color = BLACK;
  nil->left = nil->right = NULL;  
}

NodePtr create(int liczba, int col){
    NodePtr nd=(NodePtr)malloc(sizeof(Node));
    nd->key  = liczba;
    nd->color=col;
    nd->left=nd->right=nd->parent=nil;
    return nd;
}

NodePtr rotate(NodePtr nd, int dir){
    int whichSon;
    if(nd->parent->left==nd) whichSon=LEFT;
    else whichSon=RIGHT;

    if(dir==LEFT){
        NodePtr temp=nd->right;
        nd->right->parent=nd->parent;
        nd->right=nd->right->left;
        if(nd->right!=nil)
            nd->right->parent=nd;
        nd->parent=temp;
        nd->parent->left=nd;
        if(nd->parent->parent!=nil)
            if(whichSon==RIGHT)
                nd->parent->parent->right=nd->parent;
            else nd->parent->parent->left=nd->parent;
    }
    else{
        NodePtr temp=nd->left;
        nd->left->parent=nd->parent;
        nd->left=nd->left->right;
        if(nd->left!=nil)
            nd->left->parent=nd;
        nd->parent=temp;
        nd->parent->right=nd;
        if(nd->parent->parent!=nil)
            if(whichSon==RIGHT)
                nd->parent->parent->right=nd->parent;
            else nd->parent->parent->left=nd->parent;

    }
    return nd;
}

NodePtr repair(NodePtr x){
    if(x->parent->parent!=nil){
        //jeśli ojciec to prawy syn
        if(x->parent->parent->right==x->parent){
            //przypadek 1
            if(x->parent->parent->left->color==RED){
                x->parent->color=BLACK;
                x->parent->parent->color=RED;
                x->parent->parent->left->color=BLACK;
                x=x->parent->parent;
                if(x->parent==nil){
                    x->color=BLACK;
                    return x;
                }
                if(x->parent->color==RED) repair(x);
            }
            //przypadek 2
            
            else{
                if(x->parent->left==x){
                x=rotate(x->parent,RIGHT);
                }
                //przypadek 3
                if(x->parent->parent!=nil){
                    if(x->parent->parent->left->color==BLACK && x->parent->right==x){
                        NodePtr temp = rotate(x->parent->parent, LEFT);
                        x->parent->color=BLACK;
                        x->parent->left->color=RED;
                        if(temp->parent->parent==nil)
                            return temp->parent;

                    }
                }
            }
        }
        //jeśli ojciec to lewy syn
        else{
            //przypadek 1
            if(x->parent->parent->right->color==RED){
                x->parent->color=BLACK;
                x->parent->parent->color=RED;
                x->parent->parent->right->color=BLACK;
                x=x->parent->parent;
                if(x->parent==nil){
                    x->color=BLACK;
                    return x;
                }
                if(x->parent->color==RED) repair(x);
            }

            //przypadek 2
            if(x->parent->right==x){
                x=rotate(x->parent,LEFT);
            }

            //przypadek 3
            if(x->parent->parent!=nil){
                if(x->parent->parent->right->color==BLACK && x->parent->left==x){
                NodePtr temp=rotate(x->parent->parent, RIGHT);
                x->parent->color=BLACK;
                x->parent->right->color=RED;
                if(temp->parent->parent==nil)
                    return temp->parent;

            }
            }
        }

    }
    while(x->parent!=nil){
        x=x->parent;
    }
    return x;
}

NodePtr insert(NodePtr root, int x){
    bool inserted=false;
    if(root==nil){
        root=create(x, BLACK);
        inserted=true;
    }
    NodePtr temp = root;
    while(!inserted){
        if(x>temp->key){
            if(temp->right==nil){
                NodePtr new = create(x, RED);
                temp->right = new;
                new->parent = temp;
                if(new->parent->color==RED){
                    root = repair(new);
                }
                inserted=true;
            }
            else
                temp = temp->right;
        }
        else if(x<temp->key){
            if(temp->left==nil){
                NodePtr new=create(x, RED);
                temp->left=new;
                new->parent=temp;
                if(new->parent->color==RED){
                    root = repair(new);
                }
                inserted=true;
            }
            else
                temp=temp->left;
        }
        else{
            //ignoruje, jeśli jest już wstawione
            inserted=true;
        }
    }
    return root;
}

/*void print(NodePtr x){
    if(x != NULL){
        print(x->left);
        if(x->parent != NULL)
            printf(" %d(%d)", x->key, x->count);
        else
            printf(" { %d(%d) }", x->key, x->count);

        print(x->right);
    }
}*/

char wydruk[IL_POZ+1][SZER_EKR];

void drukujost(NodePtr w, int l, int p,int poziom){
       int srodek = (l+p)/2;
       if (w==nil)   return; 
       wydruk[poziom][srodek]='*';
}

void drukujwew(NodePtr w, int l, int p,int poziom){
       int srodek = (l+p)/2;
       int i,dl;
       char s[19];
       if (w==nil)    {//printf("kupa"); 
       return;}
       if (w->color==BLACK)
       {//printf("dupa");
         dl=sprintf(s,"%d",w->key);}
       else{//printf("dupa2");
	 //	        dl=sprintf(s,"\e[31m%+d\e[0m",w->klucz);
       dl=sprintf(s,"%+d",w->key);}
       for (i=0;i<dl;i++)
         wydruk[poziom][srodek-dl/2+i]=s[i];
       if (++poziom<IL_POZ){
         drukujwew(w->left,l,srodek,poziom) ;
         drukujwew(w->right,srodek+1,p,poziom) ;
       }
       else {
         drukujost(w->left,l,srodek,poziom) ;
         drukujost(w->right,srodek+1,p,poziom) ;
       }
}

void drukuj(NodePtr w){
  int j,i;
  for (i=0;i<=IL_POZ;i++)
    for (j=0;j<SZER_EKR;j++)
      wydruk[i][j] = ' ';
  drukujwew(w,0,SZER_EKR,0);
  for (i=0;i<=IL_POZ;i++){
      for (j=0;j<SZER_EKR;j++)
        putchar(wydruk[i][j]);
      printf("\n");
  }
}

int max(int l, int r){
    if(r==0 || l>r) return l;
    return r;
}

int maxDens(NodePtr x){
    if((x->left==nil && x->right==nil) || x==nil) 
        return 0;
    return max(maxDens(x->left),maxDens(x->right))+1;
}

int min(int l, int r){
    if(l==-1) return r;
    if(r==-1) return l;
    if(l>r) return r;
    return l;
}

int minDens(NodePtr x){
    if(x->left==nil && x->right==nil){
        return 0;
    }
    if(x==nil){
        return -1;
    }
    return min(minDens(x->left),minDens(x->right))+1;
}

NodePtr search(NodePtr root, int x){
    NodePtr found=root;
    while(found!=nil && found->key!=x){
        if(x<found->key) found=found->left;
        else found=found->right;
    }

    return found;
}

NodePtr minimum(NodePtr x){
    while(x->left!=nil) x=x->left;
    return x;
}

NodePtr delete(NodePtr root, NodePtr toDelete){
    if(toDelete->left==nil && toDelete->right==nil){
        if(toDelete==root) root=nil;
        else{
            if(toDelete==toDelete->parent->left) toDelete->parent->left=nil;
            else toDelete->parent->right=nil;
        }
    }else{
        if(toDelete->left!=nil && toDelete->right!=nil){
            NodePtr y=minimum(toDelete->right);
            toDelete->key=y->key;
            delete(root, y);
        }
        else{
            if(toDelete->left!=nil){
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
    nilInit();
    NodePtr root=nil;
    int choice=1;
    while(choice!=0){
        printf("Co chcesz zrobić?\n");
        printf("\t0 - WYJDZ\n");
        printf("\t1 - WSTAW\n");
        printf("\t2 - USUN\n");
        printf("\t3 - DRUKUJ\n: ");
        printf("\t4 - max gleb\n: ");
        printf("\t5 - min gleb\n: ");
        printf("\t6 - wstaw 0:999\n: ");
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
            printf("Co usunac?\n: ");
            int toDelete;
            scanf("%d", &toDelete);
            NodePtr nodeToDelete=search(root, toDelete);
            if(nodeToDelete!=NULL) {
                //if(nodeToDelete->count==1) root=delete(root, nodeToDelete);
                //else (nodeToDelete->count)--;
                root=delete(root, nodeToDelete); //zamiast powyższego
            }
            else printf("Nie ma takiego wezla\n");
            break;
        case 3:
            //print(root);
            drukuj(root);
            break;
        case 4:
            printf("Max glebokosc: %d\n",maxDens(root));
            break;
        case 5:
            printf("Min glebokosc: %d\n", minDens(root));
            break;
        case 6:
            for(int i=0; i<1000; i++){
                root=insert(root, i);
            }
            break;
        }
    }


    free(root);
    return 0;
}