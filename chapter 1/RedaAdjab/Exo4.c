#include <stdio.h>

int main() {
    //open
    FILE* file = fopen("Test3.txt","r");

    int lineCounter = 0;
    if (!fgetc(file) == '\0') { //file not empty
        lineCounter = 1;
        //read characters
        while (!feof(file)) {
            if(fgetc(file) == '\n') {
                lineCounter ++;
            }
        }
    }
    printf("The number of lines in %d", lineCounter);

    return 0;
}