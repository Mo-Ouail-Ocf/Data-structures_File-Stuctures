#include <stdio.h>
#include <string.h>
//compte nb occurence

int main(){
    FILE*f = fopen("fich.txt","r");
    if(f==NULL){
        printf("erreur de lecture d'ouverture de fichier f");
        return 0;
    }
    char mot[100];
    printf("Entrez le mot a chercher dans le fichier : ");
    scanf("%s",mot);
    //buffer de mot
    char Buff_mot[100];
    int nb_occ = 0; //nb d'occ
    while(fscanf(f," %s",Buff_mot)==1){
        if (strcmp(Buff_mot,mot)==0){
            nb_occ++;
        }
    }
    printf("Le nombre d'occurence de %s dans le fichier est : %d",mot,nb_occ);
}