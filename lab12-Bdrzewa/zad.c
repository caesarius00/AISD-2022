// struktura wezla B-drzewa i przyklad zapisu i odczytu na plik
// budowanie B-drzewa o zadanej wysokosci i drukowanie
// Uwaga: w odróżnieniu od pseudokodu, tutaj tablice kluczy
// i "wskaźników" do synów są indeksowane od 0

#include <stdio.h>
#define T 3   // stopien B-drzewa

typedef struct{
  short count;        //ilosc kluczy (-1 oznacza wezel usuniety)
  short leaf;     // czy lisc
  int key[2*T-1];   // klucze (numerowane od pozyji 0)
  int child[2*T];     // "wskaźniki" do synow (pozycje w pliku: 0,1,2 ...)
  //int info[2*T-1];  // "wskazniki" do informacji skojarzonej z kluczem
                    // (pozycje w innym pliku); tutaj nie beda uzyte
} Wezel;


int rozmiarw = sizeof(Wezel); // rozmiar wezla w bajtach
FILE *drzewo;  // plik drzewa (zawierajacy wezly)

void zapisz(int i,Wezel *w){
  // zapisuje *w jako i-ty zapis w pliku drzewa
  fseek(drzewo,(long)i*rozmiarw,SEEK_SET);
  fwrite(w,rozmiarw,1,drzewo);
  //  printf("z%d ",i);
}

void odczytaj(int i,Wezel *w){
  // odczytuje i-ty zapis w pliku drzewa i wpisuje do *w
  fseek(drzewo,(long)i*rozmiarw,SEEK_SET);
  fread(w,rozmiarw,1,drzewo);
  //  printf("o%d ",i);
}

void usun(int i){
  // usuwa i-ty zapis w pliku drzewa
  // w tej wersji nie wykorzystujemy usunietych pozycji,
  // tylko zaznaczamy jako usuniete
  Wezel w;
  odczytaj(i,&w);
  w.count=-1;
  zapisz(i,&w);
}

int budujB(int g, int n){
  // buduje B-drzewo o wysokosci g, w kazdym wezle jest n kluczy
  // wynikiem jest pozycja korzenia w pliku - jest to ostatni 
  // zapis w pliku, co jest wazne dla dalszych zapisow do pliku
  // ktore trzeba robic zaczynajac od kolejnej pozycji
  static int klucz=0; // kolejny klucz
  static int pozycja=0; // wolna pozycja w pliku
  Wezel w;
  w.count=n;
  int i;
  if (g==0){  // lisc
    for (i=0;i<n;i++){
      w.child[i]= -1; // w lisc
      w.key[i]= klucz++;
    }
    w.child[n]= -1;
    w.leaf=1;
  } else {  // wezel wewnetrzny
    for (i=0;i<n;i++){
      w.child[i]= budujB(g-1,n);
      w.key[i]= klucz++;
    }
    w.child[n]= budujB(g-1,n);;
    w.leaf=0;
  }
  zapisz(pozycja++,&w);
  return pozycja-1;
}

/*void BTreeSplitChild(Wezel* x, int i, Wezel* y){
    //rozbija wezel y, ktory jest i-tym synem wezla x
    Wezel z;
    z.leaf=y->leaf;
    z.count=T-1;
    for(int j=0; i<T-1; j++){
        x->key[j] = y->key[j+T];
    }
    if(!y->leaf){
        for(int j=0; j<T; j++){
            z.child[j] = y->child[j+T];
        }
    }
    y->count = T-1;
    for(int j=x->count+1; j>i+1; j--){
        x->child[j+1] = x->child[j];
    }
    x->child[i+1]=&z;
    for(int j=x->count; i>i; j--){
        x->key[j+1] = x->key[j];
    }
    x->key[i] = y->key

}*/
void drukujB(int r, int p){
  // drukuje B-drzewo o korzeniu r (pozycja w pliku)
  // wydruk przesuniety o p w prawo
  Wezel w;
  int i,j;
  odczytaj(r,&w);
  if (w.leaf){
    for (i=0;i<p;i++) printf(" ");
    for (i=0;i<w.count;i++) printf("%d ",w.key[i]);
    printf("\n");
  } else {
    drukujB(w.child[w.count],p+4);
    for (i=w.count-1;i>=0;i--){
      for (j=0;j<p;j++) printf(" ");
      printf("%d\n",w.key[i]);
      drukujB(w.child[i],p+4);
    }
  }
}

int BTreeSearch(Wezel *x, int k){
    int i=0;
    while (i<x->count && k>x->key[i]) i++;
    if(i<=x->count && k==x->key[i])
        return i;
    if(x->leaf) return -1;
    else{
        odczytaj(x->child[i], x);
        return BTreeSearch(x, k);
    }
        
}

int main(){
    int i;
    double sp;
    drzewo = fopen("bdrzewo","w+");
    Wezel wezel;
    int root;

    root=budujB(2,2);
    printf("\n");
    drukujB(root,0);

    odczytaj(root, &wezel);
    printf("Czego szukamy?: ");
    int seek;
    scanf("%d", &seek);
    int pos = BTreeSearch(&wezel,seek);
    if(pos>-1){
        printf("\nZnaleziono %d na pozycji %d w wezle [ ", seek, pos);
        for(int i=0; i<wezel.count; i++)
            printf("%d ", wezel.key[i]);
        printf(" ]\n");
    }
    else{
        printf("Nie znaleziono\n");
    }
    fclose(drzewo);

    return 0;
}