#include <stdio.h>
#include <string.h>

#define MAX_VILLE_LENGTH 21
#define DATE_LENGTH  11

struct TemperatureRecord {
    char city[MAX_VILLE_LENGTH];
    char date[DATE_LENGTH];
    int temperature;
};

int main(){
    FILE* file = fopen("FileResult.bin","rb+");

    if (file==NULL){
        printf("Failure of opening the file");
        return 1;
    }

}
