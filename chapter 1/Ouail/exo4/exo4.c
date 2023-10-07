#include <stdio.h>
//calculer le nb de ligne
int main(){
    FILE*f = fopen("fich.txt","r");
    if (f==NULL){
        printf("Erreur lors de l'ouverture du fichier ");
        return 1;
    }
    int nb_ligne=0;
    char c;

    if (getc(f)==EOF){
        //fichier vide
        printf("Le fichier est vide , il contient aucune ligne");
        return 0;
    }
    else {
        nb_ligne++;
        rewind(f);
        do {
            c=getc(f);
            if (c=='\n')
                nb_ligne++;
        }
        while (c!=EOF);
        
    }
    printf("Le nombre de ligne du fichier est : %d",nb_ligne);
    return 0;
}