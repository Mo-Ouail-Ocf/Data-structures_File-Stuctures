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

//modify the temperature of a city at a given date
// returns true if the modification is done
bool ModifyTempAtGivenDate(const char *fileName){

    //read city and date
    char cityName[MAX_VILLE_LENGTH];
    char givenDate[DATE_LENGTH];
    printf("Enter the city name : ");
    fgets(cityName,sizeof(cityName),stdin);
        //remove '\n';
    if (strlen(cityName)>0 && cityName[strlen(cityName)-1]=='\n')
        cityName[strlen(cityName)-1]='\0';
    printf("\nEnter the date : ");
    scanf("%s",givenDate);

    //search for the record compatible with the city and the date
    FILE*file = fopen(fileName,"r+");
    bool foundRecord= false;
    struct TemperatureRecord currRecord;
    int nbRecordsRead = 0;
    while (fread(&currRecord,sizeof(currRecord),1,file)==1 && !foundRecord){
        nbRecordsRead++;
        if( strcmp(currRecord.city,cityName)==0 && strcmp(currRecord.date,givenDate)==0 ){
            foundRecord = true;
            //modify the temperature
            printf("\nThe actual temperature is : %d",currRecord.temperature);
            printf("\nEnter the new temperature  : ");
            scanf("%d",&currRecord.temperature);
            //return to the previous record of the binary file
           
            fseek(file,(nbRecordsRead-1)*sizeof(currRecord),SEEK_SET);
            if(fwrite(&currRecord,sizeof(currRecord),1,file)==1)
                printf("Success .");
        }
    }
    if (!foundRecord){
        printf("City name with the given date does not exist in the file");
        return false;
    }
    return true;
}

//trouver temperature minimale , moyenne et maximale pour une ville donee.

//returns true if we found city data in the file
bool MinMaxMoyTemp(const char*fileName,int *tempMin,int *tempMax,int *tempMoy){
    FILE*file = fopen(fileName,"r");

    //read city name
    char cityName[MAX_VILLE_LENGTH];
    printf("Enter the city name : ");
    fgets(cityName,sizeof(cityName),stdin);
        //remove '\n';
    if (strlen(cityName)>0 && cityName[strlen(cityName)-1]=='\n')
        cityName[strlen(cityName)-1]='\0';

    //initialising: 
    int nbCityRecords=0; //nb records with the city name
    int currTemp; //of the city record
    *tempMin=100;*tempMax=0;*tempMoy=0;

    //run through file and search for city records
    struct TemperatureRecord currRecord;
    while(fread(&currRecord,sizeof(currRecord),1,file)==1){
        if (strcmp(currRecord.city,cityName)==0){
            nbCityRecords++;
            currTemp = currRecord.temperature;
            if(currTemp<*tempMin)
                 *tempMin=currTemp;
            if(currTemp>*tempMax)
                 *tempMax=currTemp;
            *tempMoy+=currTemp;
        }
    }
    //testing if there was any record of the city
    if (nbCityRecords>0){
        *tempMoy/=nbCityRecords;
        return true;
    }
    printf("There is no record for %s",cityName);
    return false;
    
}
int main(){
    const char fileName[] = "FileResault.bin";
    FILE* file = fopen("FileResault.bin","rb+");

    if (file==NULL){
        printf("Failure of opening the file");
        return 1;
    }
    int min,max,moy;
    MinMaxMoyTemp(fileName,&min,&max,&moy);
    printf("%d %d %d",min,max,moy);

    return 0;
}
