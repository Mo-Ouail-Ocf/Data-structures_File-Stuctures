#include <stdio.h>
#include <string.h>
struct Record {
    char city[20];
    char date[10];
    int temp;
};

int main(){
    FILE*file = fopen("test.bin","rb+");

    if (file==NULL){
        printf("Failure of opening the file");
        return 1;
    }

    struct Record dataTable[]={
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
