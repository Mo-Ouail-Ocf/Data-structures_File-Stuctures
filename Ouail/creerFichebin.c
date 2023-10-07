#include <stdio.h>
#include <string.h>
/* construir fich binaire contenant nb d'enregs <nom,tlph>*/

int main(){

struct Tenreg {
    char nom[20];
    char tlph[15];
} e;

char nomFich[30];
printf("Entrez le nom du fichier binaire a construire : ");
scanf(" %s",nomFich);
strcat(nomFich,".bin");


FILE *f = fopen(nomFich,"w+");


if (f==NULL){
    printf("erreur d'ouverture de fichier \n");
    return 1;
}

printf("entez un nom et un numero de telephone (0 0 pour quitter le programme)");
scanf(" %s %s",e.nom,e.tlph);
while(e.nom[0]!='0'){
    //ecriture du e dans le fichier
    fwrite(&e,sizeof(e),1,f);
    printf("entez un nom et un numero de telephone (0 0 pour quitter le programme)");
    scanf(" %s %s",e.nom,e.tlph);
}
//afficher le contenu d'un fichier binaire
fseek(f,0,SEEK_SET);
int nb_Elmt_Lu = fread(&e,sizeof(e),1,f);
if (nb_Elmt_Lu==1){
    printf(" Nom : %s --- Numero : %s \n",e.nom,e.tlph);
}
while(!feof(f)){
    int nb_Elmt_Lu = fread(&e,sizeof(e),1,f);
    if (nb_Elmt_Lu==1){
        printf(" Nom : %s --- Numero : %s \n",e.nom,e.tlph);
    }
}
//accees directe a l'enregistrement 
int num_enreg;
printf("entrez le numero d'enregistrement a afficher : ");
scanf("%d",&num_enreg);
fseek(f,(num_enreg-1)*sizeof(e),SEEK_SET);
nb_Elmt_Lu = fread(&e,sizeof(e),1,f);
if (nb_Elmt_Lu==1){
    printf(" nom : %s ---- numero : %s",e.nom,e.tlph);
}
return 0;
}