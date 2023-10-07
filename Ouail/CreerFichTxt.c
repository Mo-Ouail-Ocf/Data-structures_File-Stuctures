#include <stdio.h>
#include <stdbool.h>
#include <string.h>
int main(){
    struct Tenreg { 
        char nom[20];
        char num[15];
    }e;
    char nom_fich[20];
    printf("Entez le nom du fichier texte a construire : ");
    scanf(" %s",nom_fich);
    strcat(nom_fich,".txt");
    FILE *f = fopen(nom_fich,"w+");
    if(f==NULL){
        printf("Erreur d'ouverture de fichier");
        return 1;
    }
    bool ct= true;
    while(ct){
        printf("Entrez un nom et un numero de telephone (0 0 pour quitter le programme): ");
        scanf(" %s %s",e.nom,e.num);
        if(e.nom[0]=='0'){
            ct=false;
            break;
        }
        fprintf(f,"nom : %s , numero : %s\n",e.nom,e.num);
    }
    fseek(f,sizeof(e),SEEK_SET);
    fscanf(f,"nom:  %s , numero :  %s",e.nom,e.num);
    printf("%s %s",e.nom,e.num);
}