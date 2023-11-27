#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>


//? declarations donnees
    #define MAXBLOC 100
    #define KEYLENGTH 20
    #define DATALENGTH 50
    

    struct entete {
        int nbBlocs;
        int nbInsertions;
    };

    struct structTable
    {
        char key[KEYLENGTH] ;
        bool isDeleted ;
        char data[DATALENGTH];
    };
    

    struct tofBloc {
        struct structTable tab [MAXBLOC];
        int nbData ; //nb data inserted
    };

//?functions declarations
    //! machine abstraite
    bool ouvrir(char*fileName , FILE**filePtr ,char mode);
    void fermer(FILE*file);
    int entete(FILE*file,int dep);
    bool lireDir(FILE*f,int numBloc,struct tofBloc*buff);
    bool ecrireDir(FILE*f,int numBloc,struct tofBloc*buff);

    //!general functions
    void rechDichoBuff(struct tofBloc buff,char key[KEYLENGTH],bool*found ,int*posBloc);