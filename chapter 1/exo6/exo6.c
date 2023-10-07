#include <stdio.h>
#include <string.h>

const int MAX_VILLE_LENGTH = 21;
const int DATE_LENGTH = 11;
const int RECORD_SIZE = (MAX_VILLE_LENGTH + DATE_LENGTH + sizeof(int));

struct TemperatureRecord {
    char city[MAX_VILLE_LENGTH];
    char date[DATE_LENGTH];
    int temperature;
};

int main(){
    FILE* file = fopen("test.bin","rb+");

    if (file==NULL){
        printf("Failure of opening the file");
        return 1;
    }

    struct TemperatureRecord dataTable[]={
    {
        "chelghoum laid","07/10/2023",30
    },
    {
        "chelghoum laid","06/10/2023",25
    },
    {
        "chelghoum laid","05/10/2023",20
    },
    {
        "chelghoum laid","04/10/2023",23
    }
    };
}
