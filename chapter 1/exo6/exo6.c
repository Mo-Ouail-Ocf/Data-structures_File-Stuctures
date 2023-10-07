#include <stdio.h>
#include <string.h>

#define MAX_VILLE_LENGTH 21
#define DATE_LENGTH 11 
#define RECORD_SIZE (MAX_VILLE_LENGTH + DATE_LENGTH + sizeof(int))

struct TemperatureRecord {
    char city[MAX_VILLE_LENGTH];
    char date[11];
    int temperature;
};

/* void createRecord(*FILE file, e) {
    s
} */

int main(){
    FILE* file = fopen("FileResault.bin","rb+");

    if (file==NULL){
        printf("Failure of opening the file");
        return 1;
    }

    struct TemperatureRecord dataTable[]={
    {
        "chelghoum laid","07/10/2023",30
    }
    };

    return 0;
}
