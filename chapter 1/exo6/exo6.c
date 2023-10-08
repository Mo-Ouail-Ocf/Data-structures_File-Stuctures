#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_VILLE_LENGTH 21
#define DATE_LENGTH 11 
#define RECORD_SIZE (MAX_VILLE_LENGTH + DATE_LENGTH + sizeof(int))

struct TemperatureRecord {
    char city[MAX_VILLE_LENGTH];
    char date[DATE_LENGTH];
    int temperature;
};

struct TemperatureRecord readRecord() {
    struct TemperatureRecord temperatureRecord;
    printf("Enter city name: ");
    scanf("%[^\n]", temperatureRecord.city);

    printf("Enter date: ");
    scanf("%s", temperatureRecord.date);

    printf("Enter temperature: ");
    scanf("%d", temperatureRecord.temperature);

    return temperatureRecord;
}

void addRecord(FILE* file, struct TemperatureRecord* temperatureRecord) {
    // Move the file pointer to the end of the file
    fseek(file, 0, SEEK_END);
    
    bool foundRecord = false;
    struct TemperatureRecord currentRecord;
    while (fread(&currentRecord, RECORD_SIZE, 1, file) == 1) { //read one record from the file
        if ((strcmp(currentRecord.city, temperatureRecord->city) != 0) && (strcmp(currentRecord.date, temperatureRecord->date) != 0)) { //if city name match
            foundRecord = true;
        }
    }
    // Write the record to the file
    if(!foundRecord) fwrite(temperatureRecord, RECORD_SIZE, 1, file);
}

void deleteCityRecord(const char* fileName,const char* city) { //!must close file before using
    //open files
    FILE* originalFile = fopen(fileName, "rb"); 
    FILE* tempFile = fopen("TempFile.bin", "wb");
    
    //move to the start of the file
    fseek(originalFile, 0, SEEK_SET);

    //read the file record 
    struct TemperatureRecord currentRecord;
    while (fread(&currentRecord, RECORD_SIZE, 1, originalFile) == 1) { //read one record from the file
        if (strcmp(currentRecord.city, city) != 0) { //if city name match
            fwrite(&currentRecord, RECORD_SIZE, 1, tempFile);
        }
    }

    //close
    fclose(originalFile);
    fclose(tempFile);

    remove(fileName);// Remove the original file
    rename("TempFile.bin", fileName);
}

int main(){
    const char fileName[] = "FileResault.bin";
    FILE* file = fopen("FileResault.bin","rb+");

    if (file==NULL){
        printf("Failure of opening the file");
        return 1;
    }

    struct TemperatureRecord dataTable[]={
    {
        "chelghoum laid","13/10/2022",22
    },
    {
        "la montagne","05/04/2023",35
    },
    {
        "harrash","07/01/2023",30
    },
    {
        "bash jrah","30/06/2023",31
    },
    {
        "oud samar","20/11/2022",25
    },
    {
        "oud samar","20/11/2022",25
    },
    };

    for (int i = 0; i < 5; i++) {
        addRecord(file, &dataTable[i]);
    }
    fclose(file);    

    return 0;
}
