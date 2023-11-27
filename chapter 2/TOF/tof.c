#include"tof.h"
#include<string.h>
#include<time.h>

//!    machine abtraite :
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

    int entete(FILE*file,int dep){
        if(file!=NULL){
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

        }}
    }

    bool affEntete(FILE*f,int attrNumb,int dataIn){
        if(f!=NULL){
            switch (attrNumb)
            {
            case 1:
                fseek(f,0,SEEK_SET);
                fwrite(&dataIn,sizeof(dataIn),1,f);
                return true;
                break;
            case 2:
                fseek(f,sizeof(int),SEEK_SET);
                 fwrite(&dataIn,sizeof(dataIn),1,f);
                return true;
            default:
                return true;
                break;
            }
        }
        return false;
    }

    bool lireDir(FILE*f,int numBloc,struct tofBloc*buff){
        if(f!=NULL){
        int nbBloc = entete(f,1);
        if(numBloc<=nbBloc){
            fseek(f,sizeof(entete)+(numBloc-1)*sizeof(struct tofBloc),SEEK_SET);
            fread(buff,sizeof(struct tofBloc),1,f);
            return true;
        }}
        return false;
    
    }

    bool ecrireDir(FILE*f,int numBloc,struct tofBloc*buff){
        if(f!=NULL){
        int nbBloc = entete(f,1);
        if(numBloc<=nbBloc){
            fseek(f,sizeof(entete)+(numBloc-1)*sizeof(struct tofBloc),SEEK_SET);
            fwrite(buff,sizeof(struct tofBloc),1,f);
            return true;
        }}
        return false;
    }

//!     general functions :
    void rechDichoBuff(struct tofBloc buff,char key[KEYLENGTH],bool*found ,int*posBloc){
        int inf=0,sup=buff.nbData;
        bool stop=false;
        *found=false;
        while(!found && inf<=sup){
            *posBloc = (inf+sup)/2;
            if(strcmp(key,buff.tab[*posBloc].key)<0){
                inf=*posBloc+1;
            }else {
                if (strcmp(key,buff.tab[*posBloc].key)>0){
                    sup=*posBloc-1;
                } 
                else {
                    *found=true;
                }
            }
        }
        if(inf>sup){
            *posBloc=inf;
        }
    }


