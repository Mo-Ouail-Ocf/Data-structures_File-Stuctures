#include <stdio.h> 
#include <stdbool.h> //for bool
#include <ctype.h> //for isalpha and isspace
#include <string.h> //for string manipulation

const int MAX_WORD_LENGHT = 45;
const int MAX_LINE_LENGHT = 1024;

//read the input denying spaces and new lines(only characters)
bool ReadWord(char word[MAX_WORD_LENGHT]) {
    int i = 0;
    char  c;
    bool isNull = false;
    //skip leading spaces
    while (((c = getchar()) != EOF) && isspace(c));

    // Read characters until a space(or "enter") is encountered
    while (i < MAX_WORD_LENGHT - 1 && (c != EOF) && (c != '\n') && !isspace(c)) {
        word[i++] = c;
        c = getchar();
    }
    word[i] = '\0'; //end word
    if (i == 0) isNull = true;  
    return isNull;
}

//encrypt a text file using a given key
void EncryptTextFile(FILE* inputFile, FILE* outputFile, char key[MAX_WORD_LENGHT]) {
    char resLine[MAX_LINE_LENGHT]; //store each line
    int i = 0; //index for file
    while (fgets(resLine, MAX_LINE_LENGHT, inputFile) != NULL) { //get each line
        for (i =0; i<strlen(resLine);i++) { //get each character in line
            if (resLine[i] != '\n') {
                resLine[i] += key[i % strlen(key)]; //add coresponding char with key char
            }
        }
        fputs(resLine, outputFile); //store resault in another file
    }
}

//decrypt a text file using a given key (same just chnage + with -, maybe someday i will fuse the two functions)
void DecryptTextFile(FILE* inputFile, FILE* outputFile, char key[MAX_WORD_LENGHT]) {
    char resLine[MAX_LINE_LENGHT]; //store each line
    int i = 0; //index for file
    while (fgets(resLine, MAX_LINE_LENGHT, inputFile) != NULL) { //get each line
        for (i =0; i<strlen(resLine);i++) { //get each character in line
            if (resLine[i] != '\n') {
                resLine[i] -= key[i % strlen(key)]; //add coresponding char with key char
            }
        }
        fputs(resLine, outputFile); //store resault in another file
    }
}

//Process(encrypt or decrypt depending on entery) a binary file using a given key
void ProcessBinaryFile(FILE* inputFile, FILE* outputFile, char key[MAX_WORD_LENGHT]) {
    char resLine[MAX_LINE_LENGHT]; //store each line
    int bytesRead = 0;
    int keyLenght = strlen(key);
    int i = 0; //index for file
    while ((bytesRead = fread(resLine, sizeof(char), keyLenght, inputFile)) > 0) { //get each line
        for (i =0; i<bytesRead;i++) { //get each character in line
            if (resLine[i] != '\n') {
                resLine[i] = resLine[i] ^ key[i % keyLenght]; //add coresponding char with key char
            }
        }
        fwrite(resLine, sizeof(char), bytesRead, outputFile); //store resault in another file
    }
}

int main() {
    //?TEST TEXT/BINARY FILE (ON CHANGE r ET w PAR rb ET wb)
    //open files in Binary
    FILE* inputFile = fopen("Test3.txt","rb");
    FILE* outputFile = fopen("EncryptedTest3.txt","wb"); //resault file

    char key[MAX_WORD_LENGHT];
    //read key
    printf("Enter the Key Code : ");
    ReadWord(key);

    ProcessBinaryFile(inputFile,outputFile,key);

    fclose(inputFile);
    fclose(outputFile);

    //open files in Binary
    inputFile = fopen("EncryptedTest3.txt","rb");
    outputFile = fopen("DecryptedTest3.txt","wb"); //resault file

    ProcessBinaryFile(inputFile,outputFile,key); 

    fclose(inputFile);
    fclose(outputFile);
}