#include <stdio.h>

int main(){
    //concatene 02 fich txt , retourne resultats dans fich3.txt
    FILE*f1,*f2,*f3;
    f1= fopen("fich1.txt","r");
    f2=fopen("fich2.txt","r");
    if (f1==NULL || f2==NULL){
        printf("Erreur d'ouverture des fichiers a concatener");
        return 1;
    }
    f3=fopen("fich3.txt","w+");
    if(f3==NULL){
        printf("Erreur d'ouverture de fichier f3 resultat de concatenation");
        return 1;
    }
    
    //lire f1 ligne par ligne , et inserer dans f3 , puis faire le meme pour f2
    char ligne[400];
    FILE*f=f1;
    do
    {
       if(fgets(ligne,402,f)){
        fputs(ligne,f3);
       }
       if(feof(f1)){
        //inserer caract saut de ligne
        fputc('\n',f3);
        f=f2;
       }
    } while (!feof(f));
    
    
    fclose(f1);
    fclose(f2);
    fclose(f3);
    return 0;
}