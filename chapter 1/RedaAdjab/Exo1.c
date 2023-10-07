#include <stdio.h>

int main() {
    //open file on write
    FILE *file = fopen("Test.txt", "r");

    //1) read file by characters using a char
    int character;
    while ((character = fgetc(file)) != EOF) {
        printf("%c", character);
    }
    printf("\n");

    //rewind character indicator
    rewind(file);
    printf("\n\n");

    //2) read file by characters using a char
    while (!feof(file)) {
        printf("%c", fgetc(file));
    }
    printf("\n");

    //close file
    printf("done reading\n");
    fclose(file);

    return 0;
}