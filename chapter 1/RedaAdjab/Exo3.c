#include <stdio.h>
#include <ctype.h> //for isalpha and isspace
#include <stdbool.h>

const int MAX_WORD_LENGHT = 45;

//read the input denying spaces and new lines;
bool LireMot(char word[MAX_WORD_LENGHT]) {
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

int main() {
    //read the word
    printf("Enter the word : ");
    char word[MAX_WORD_LENGHT];
    int wordCounter = 0;
    LireMot(word);

    //test if null
    if (word[0] != '\0') {
        //open read
        FILE* file = fopen("Test3.txt", "r");
    
        //go in file
        int i = 0;
        while (!feof(file)) {
            if (fgetc(file) == word[i]) {
                i++;
                if (word[i] == '\0') { //found word, do another search
                    wordCounter ++;
                    i=0;
                }
            }
            else {
                i=0; //do another search
            }
        }
    }
    printf("The number of times the word '%s' is repeated is: %d", word,wordCounter);
    return 0;
}