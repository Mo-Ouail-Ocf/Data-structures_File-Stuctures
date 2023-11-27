#include<stdio.h>
#include<stdbool.h>

//machine abtraite :
//? declaration du fichier 
    #define MAXBLOC 100
    #define KEYLENGTH 20
    #define DATALENGTH 50
    

    struct entete {
        int nbBlocs;
        int nbInsertions;
    };

    struct enregBloc
    {
        char key[KEYLENGTH] ;
        bool isDeleted ;
        char data[DATALENGTH];
    };
    

    struct tBloc {
        struct enregBloc tab [MAXBLOC];
        int nbData ;
    };

    struct tBloc buffer;

    //functions to use :
    bool ouvrir(char*fileName , FILE**filePtr ,char mode){
        switch (mode)
        {
        case 'N':
            *filePtr =fopen(fileName,"wb+");
            return true;
            break;
        case 'A':
            *filePtr=fopen(fileName,"wb");
            return true;
            break;

        default:
            return false;
            break;
        }
    }

    void fermer(FILE*file){
        fclose(file);
    }

    /*
        fread()
        fwrite()
    */

    int entete(FILE*file,int dep){
        fseek(file,0,SEEK_SET);
        struct entete entete;
        fread(&entete, sizeof(entete), 1, file);
        switch (dep)
        {
        case 1:
            return entete.nbBlocs;
            break;
        
        case 2:
            return entete.nbInsertions;
            break;

        }
    }
void main(){

}