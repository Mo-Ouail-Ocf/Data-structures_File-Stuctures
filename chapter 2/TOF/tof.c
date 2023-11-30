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
            *filePtr=fopen(fileName,"rb+");
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
        fread(&entete, sizeof(struct entete), 1, file);
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

    void InitEntete(char* fileName) {
        //open file
        FILE* file;
        ouvrir(fileName, &file, 'A');

        struct entete header;
        header.nbBlocs = 0;
        header.nbInsertions = 0;
        fseek(file, 0, SEEK_SET);
        fwrite(&header, sizeof(struct entete), 1, file);

        //close file
        fermer(file);
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
            fseek(f,sizeof(struct entete)+(numBloc-1)*sizeof(struct tofBloc),SEEK_SET);
            fread(buff,sizeof(struct tofBloc),1,f);
            return true;
        }}
        return false;
    }

    bool ecrireDir(FILE*f,int numBloc,struct tofBloc*buff){
        if(f!=NULL || numBloc<=0){
        int nbBloc = entete(f,1);
        if(numBloc<=nbBloc){
            fseek(f,sizeof(struct entete)+(numBloc-1)*sizeof(struct tofBloc),SEEK_SET);
            fwrite(buff,sizeof(struct tofBloc),1,f);
            return true;
        }}
        return false;
    }
//!   end machine abtraite.

//!     general functions :
    void rechDichoBuff(struct tofBloc buff,char key[KEYLENGTH],bool*found ,int*posBloc){
        int inf=0,sup=buff.nbData;
        bool stop=false;
        *found=false;
        while(!(*found) && inf<=sup){
            *posBloc = (inf+sup)/2;
            if(strcmp(key,buff.tab[*posBloc].key)<0){
                sup=*posBloc-1;
            }else {
                if (strcmp(key,buff.tab[*posBloc].key)>0){
                    inf=*posBloc+1;
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
//!    end general functions.

//!    TOF functions :
    //? dichotomique research of a given key
    void RechDichoTOF(char* fileName,char key[KEYLENGTH], bool *found, int *blocNum, int *structNum) { 
        //open file
        FILE* file;
        ouvrir(fileName, &file, 'A');
        fseek(file, 0, SEEK_SET);

        //declare buff
        struct tofBloc buff;
        
        //initialization
        int blocInf = 1;
        int blocSup = entete(file, 1); //nombre blocs
        *found = false;
        bool stop = false;
        *structNum = 0;

        //research for blocs
        while((blocInf<=blocSup) && !(*found) && !stop) {
            *blocNum = (blocInf+blocSup)/2;
            lireDir(file, *blocNum, &buff);
/*             printf("\there blocNum=%d and buff.nb=%d\n",*blocNum,buff.nbData);
            for (int i = 0; i < buff.nbData; i++) {
                printf("%d) Key: %s\n", i + 1, buff.tab[i].key);
            } */
            if((strcmp(key, buff.tab[0].key) >= 0) && (strcmp(key, buff.tab[buff.nbData-1].key) <= 0)){
                //key is in bloc number blocNum
                rechDichoBuff(buff, key, found, structNum);
                /* printf("pos=%d",*structNum); */
                stop = true;
            } else {
                //key is not in this bloc
                if(strcmp(key, buff.tab[0].key) < 0) { 
                    blocSup = *blocNum -1;
                } else { //i.e strcmp(key, buff.tab[buff.nbData].key) > 0
                    blocInf = *blocNum +1;
                }
            }
        }
        if(blocInf>blocSup) {
            *blocNum = blocInf;
            *structNum = 0;
        }
        printf("\n\nWe found element of key %s in bloc number %d position %d", key, *blocNum, *structNum);
        //close file
        fermer(file);     
    }

    //? creation of a file with initialized values
    void ChargeInitTOF(char* fileName,int nbStructs) {
        //open file
        FILE* file;
        ouvrir(fileName, &file, 'N');
        fseek(file, 0, SEEK_SET);

        //declare buf
        struct tofBloc buff;
        buff.nbData = 0;

        int blocNum = 1;
        int structNum = 0;
        struct structTable blocElement;
        printf("Give a set of %d structures (just the key up to %d characters) in croissante order\n\n", nbStructs, KEYLENGTH);
        for(int i=0;i<nbStructs;i++) {
            printf("\n%d)Enter the key with the coresspondant data:\n",i+1);
            printf("key:");
            scanf("%s", (blocElement.key));
            printf("data:");
            scanf("%s", (blocElement.data));
            blocElement.isDeleted = false;

            if(structNum<=MAXBLOC*FILL_PRCNTG) {
                buff.tab[structNum] = blocElement;
                structNum++; 
            } else {
                buff.nbData = structNum;
                ecrireDir(file, blocNum, &buff);
                buff.tab[0] = blocElement;
                blocNum++;
                structNum = 1;
            }
        }
        buff.nbData = structNum;
/*         for (int i = 0; i < buff.nbData; i++) {
            printf("%d) Key: %s\n", i + 1, buff.tab[i].key);
        } */
        ecrireDir(file, blocNum, &buff);
        affEntete(file, 1, blocNum);
        affEntete(file, 2, nbStructs);

    // Print debug information
    /* printf("\nDebug: Final block content:\n"); */
    lireDir(file, blocNum, &buff);
/*     for (int i = 0; i < buff.nbData; i++) {
        printf("%d) Key: %s\n", i + 1, buff.tab[i].key);
    }  */

        //close file
        fermer(file);        
    }

    //? inserting an elemnt with reordering
    void InsertTOF(char* fileName, struct structTable element) {
        //find the element
        bool found;
        int blocNum;
        int structNum;
        RechDichoTOF(fileName, element.key, &found, &blocNum, &structNum);

        if(!found) { //insert in !found position
            //open file
            FILE* file;
            ouvrir(fileName, &file, 'A');
            fseek(file, 0, SEEK_SET);

            //declare buff
            struct tofBloc buff;
            buff.nbData = 0;

            bool continu = true;
            while(continu && blocNum<=entete(file,1)) {
                lireDir(file, blocNum, &buff);
                struct structTable temp = buff.tab[buff.nbData]; //save the last element
                int index = buff.nbData;
                while (index>structNum) //reordering elements in the bloc
                {
                    buff.tab[index] = buff.tab[index-1];
                    index--;
                }
                buff.tab[structNum] = element;
                if(buff.nbData<MAXBLOC) { //bloc not full
                    buff.nbData++;
                    buff.tab[buff.nbData] = temp;
                    ecrireDir(file, blocNum, &buff);
                    continu = false;
                } else { //bloc full
                    ecrireDir(file, blocNum, &buff); //save the re-ordered bloc
                    blocNum++; //advance to the next one
                    structNum = 1;
                    element = temp; //redo the same fo temp
                }
            }
            if(blocNum>entete(file,1)) { //we surpassed the number of blocs 
                buff.tab[0] = element;
                buff.nbData = 1;
                affEntete(file, 1, blocNum); //add a new bloc
                ecrireDir(file, blocNum, &buff);
            }
            affEntete(file, 2, entete(file, 2)+1); //increment the number of insersts

            //close file
            fermer(file); 
        }         
    }

    //? delete un elemnet by updating the isDeleted attribute 
    void DeleteLogTOF(char* fileName, char key[KEYLENGTH]) {
        //find the element
        bool found;
        int blocNum;
        int structNum;
        RechDichoTOF(fileName, key, &found, &blocNum, &structNum);

        if(found) {
            //open file
            FILE* file;
            ouvrir(fileName, &file, 'A');
            fseek(file, 0, SEEK_SET);

            //declare buff
            struct tofBloc buff;
            buff.nbData = 0;

            lireDir(file, blocNum, &buff);
            buff.tab[structNum].isDeleted = true;
            ecrireDir(file, blocNum, &buff);

            //close file
            fermer(file);
        }
    }

    //? print elements of the table
    void printTof(FILE*f){
        //we suppose that the file is already opened
        int posBloc,numBloc;
        struct tofBloc buff;
        if(f==NULL ||entete(f,1)==0){
            printf("The file is empty \n");
        } else  {
            for ( numBloc = 1; numBloc <= entete(f,1); numBloc++)
            {
                printf("---------- Bloc %d ----------\n\n",numBloc);
                lireDir(f,numBloc,&buff);
                
                for ( posBloc = 0; posBloc < buff.nbData;posBloc++)
                {
                    printf("Record %d :\n",posBloc);
                    printf("Cle : %s  , Data : %s ,Efface :%d \n",buff.tab[posBloc].key,buff.tab[posBloc].data,buff.tab[posBloc].isDeleted);
                    printf("\n");
                }
                printf("\n");
            }
        }
    }

//!   end TOF functions.

int main() {
    char fileName[] = "fileTOF.bin";
    InitEntete(fileName);

    struct structTable blocElement = {
        "3",
        false,
        "data"
    };
    bool found;
    int blocNum = -1;
    int structNum = -1;
    struct tofBloc buff;

/*     for(int i=0;i<6;i++) {
        blocElement.key[0] = i+1;
        InsertTOF(fileName, blocElement);
    } */

    printf("\n\ninitialize file:");
    ChargeInitTOF(fileName, 5);
    
    printf("\n\nresearch element");
    RechDichoTOF(fileName, blocElement.key, &found, &blocNum, &structNum);
    

    printf("\n\ninsert file:");
    strcpy(blocElement.key,"3");
    InsertTOF(fileName, blocElement);
    strcpy(blocElement.key,"0");
    InsertTOF(fileName, blocElement);
    strcpy(blocElement.key,"7");
    InsertTOF(fileName, blocElement);

    printf("\n\ndelete file:");
    strcpy(blocElement.key,"3");
    DeleteLogTOF(fileName, blocElement.key);

    printf("\n\nPrint file:");
    FILE *file;
    ouvrir(fileName, &file, 'A');
    printTof(file);
    fclose(file);

    return 0;
}
