#include <stdio.h>
#include <string.h>
#define LEFT 0
#define CORNER 1
#define TOP 2
#define SIZE 100

int length(char* w1){
    int i=0;
    for(;w1[i]!='\0';i++);
    return i;
}

int LCS(char w1[SIZE], char w2[SIZE], char dir[SIZE][SIZE], int counter[SIZE][SIZE]){
    int l1=length(w1);
    int l2=length(w2);
    for(int i=0; i<l1+1; i++) counter[i][0]=0;  //*((counter+kolumna*WIERSZ)+kolumna)
    for(int j=0; j<l2+1; j++) counter[0][j]=0;
    for(int i=1; i<l1+1; i++){
        for(int j=1; j<l2+1; j++){
            if(w1[i]==w2[j])
            {
                counter[i][j]=counter[i-1][j-1]+1;
                dir[i][j]='\\';
            }
            else if(counter[i-1][j]>=counter[i][j-1]){
                counter[i][j]=counter[i-1][j];
                dir[i][j]='|';
            }else{
                counter[i][j]=counter[i][j-1];
                dir[i][j]='-';
            }
        }
    }
    return counter[l1-1][l2-1];
}

void printLCS(char w[SIZE], char b[SIZE][SIZE], int i, int j){
    if(i==0 || j==0) return;
    if(b[i][j]=='\\'){
        printLCS(w, b, i-1, j-1);
        printf("%c",w[i]);
    } else if(b[i][j]=='|'){
        printLCS(w, b, i-1, j);
    }else{
        printLCS(w, b, i, j-1);
    }
}

int main(){
    char w1[SIZE], w2[SIZE], dir[SIZE][SIZE];
    int choice=1, counter[SIZE][SIZE];
    //Słowa indeksowane będą od 1
    *w1='0';
    *w2='0';
    while(choice){
        printf("M E N U\n");
        printf("0 - wyjdz\n");
        printf("1 - podaj ciagi\n: ");
        scanf("%d", &choice);
        if(choice){
            printf("Podaj ciag nr 1: ");
            scanf("%s", w1+1);
            printf("Podaj ciag nr 2: ");
            scanf("%s", w2+1);
            int lcs=LCS(w1, w2, dir, counter);
            printf("LCS = %d\n",lcs);
            printLCS(w1, dir, length(w1), length(w2));
            printf("\n");
        }
    }
    return 0;
}