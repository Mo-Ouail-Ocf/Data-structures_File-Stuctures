#include <stdio.h>

const int MAX_FILE_SIZE = 1024;

int main() {
    //open read
    FILE* file = fopen("Test.txt", "r");
    FILE* file2 = fopen("Test2.txt", "r");

    //read and copy
    int i = 0; //index
    char res[MAX_FILE_SIZE]; //contains all that charcters of both files
    while (i < MAX_FILE_SIZE-1 && (res[i] = fgetc(file)) != EOF) {
        i++;
    }
    while (i < MAX_FILE_SIZE-1 && (res[i] = fgetc(file2)) != EOF) {
        i++;
    }
    res[i] = '\0'; //i is now the nummber of total characters+EOF

    //close
    fclose(file);
    fclose(file2);

    //open write
    file = fopen("Test3.txt", "w");

    //write on file
    fwrite(res, sizeof(char), i, file); //write all charcters into another file

    //close
    fclose(file);

    return 0;
}
