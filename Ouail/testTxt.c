#include <stdio.h>

void main(){
    FILE*f = fopen("test.txt","r+");
    struct tEnreg{
        char nom[20];
        char prenom[20];
        int gp;
    }etd;
    struct tEnreg tab[20];
    int index=0;
    while(!feof(f)){
        if(fscanf(f,"%s%s%d",tab[index].nom,tab[index].prenom,&tab[index].gp)==3){
        printf("nom: %s prenom: %s  groupe: %d\n",tab[index].nom,tab[index].prenom,tab[index].gp);
        }
        index++;
    }
    printf("%d",index);
    
}