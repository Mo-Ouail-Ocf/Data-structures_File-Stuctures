#include <stdio.h>

int main(){
    //affichage d'un fichier texte a l'ecran
    char ligne[400];
    //affichier le texte ligne par ligne apres la lecture de la ligne dans ligne

    FILE*f=fopen("fich.txt","r");
    
    if(f==NULL){
        printf("erreur d'ouverture du fichier en mode ""r");
        return 1;
    }

    while(fgets(ligne,402,f)){
        printf(ligne);
    }
    fclose(f);
    return 0;

}