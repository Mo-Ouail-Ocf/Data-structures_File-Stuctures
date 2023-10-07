#include <stdio.h>

int main() {
    //open file on write
    FILE *file = fopen("Test.txt", "w");

    //test if null
    if (file == NULL) {
        printf("error");
        return 1;
    }

    //write on file
    fprintf(file, "This is a line written to the file.");

    //close file
    printf("done writing\n\n");
    fclose(file);

    //open file on write
    file = fopen("Test.txt", "r");

    //read file
    //method1
    char buffer[100];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s\n", buffer);
    }
    //method2
    int character;
    while ((character = fgetc(file)) != EOF) {
        printf("%c", character);
    }
    printf("\n");

    //close file
    printf("done reading\n");
    fclose(file);
}

