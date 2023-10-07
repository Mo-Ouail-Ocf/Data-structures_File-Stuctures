#include <stdio.h>
#include <string.h>
int main(){
    struct Tenreg
    {
        char nom[20];
        char num[15];
    }e ={
        "ouail","220010"
    };
    
    FILE *f = fopen("test.txt","w+");
    fputc('c',f);
    fprintf(f,"\nnom :%s , numero:%s ",e.nom,e.num);
    
    fseek(f,2,SEEK_SET);
    fscanf(f,"nom:%s ,num:%s",&e.nom,&e.num);
    strcpy(e.nom,"sup");    
    printf("%s  %s",e.nom,e.num);
    
}