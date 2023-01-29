#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct 
{
    float SensData[512][18];
    float RecentData[18];

    char CH0_Name[200];
    char CH1_Name[200];
    char CH2_Name[200];
    char CH3_Name[200];
    char CH4_Name[200];
    char CH5_Name[200];

    char CardName[200];
    FILE *RecFile;
    FILE * EvFile;

}TestStruct;

FILE *Ptr;
FILE *Ptr2;

void writeRec(TestStruct* Teste)
{
    char AuxLine[128] = "";
    char AuxDigit[32];

    for (int i = 0; i<18; i++){
    gcvt(Teste->SensData[0][i], 6, AuxDigit);
    strcat(AuxLine,AuxDigit);
    strcat(AuxLine,",");
    }
    strcat(AuxLine,"\n");
    fputs(AuxLine,Ptr);

}


void writeEvent(TestStruct* Teste)
{
    char AuxLine[128] = "";
    char AuxDigit[32];

    for (int j = 0; j< 128;j++){
    sprintf(AuxLine," ");
    for (int i = 0; i<18; i++){
    gcvt(Teste->SensData[j][i], 6, AuxDigit);
    strcat(AuxLine,AuxDigit);
    strcat(AuxLine,",");
    }
    strcat(AuxLine,"\n");
    fputs(AuxLine,Ptr2);
    }
}


void writeHeader(TestStruct *Teste)
{

    char AuxLine[12024];
    strcat(AuxLine,Teste->CH0_Name);
    strcat(AuxLine,",");
    strcat(AuxLine,Teste->CH1_Name);
    strcat(AuxLine,",");
    strcat(AuxLine,Teste->CH2_Name);
    strcat(AuxLine,",");
    strcat(AuxLine,Teste->CH3_Name);
    strcat(AuxLine,",");
    strcat(AuxLine,Teste->CH4_Name);
    strcat(AuxLine,",");
    strcat(AuxLine,Teste->CH5_Name);
    strcat(AuxLine,"\n");
    fputs(AuxLine,Ptr);
    fputs(AuxLine,Ptr2);

}

int main(int argc, char const *argv[])
{
    
    
    TestStruct Teste;
    


    memccpy(Teste.CH0_Name,"TesteCH",20,20);
    memccpy(Teste.CH1_Name,"TesteCH",20,20);
    memccpy(Teste.CH2_Name,"TesteCH",20,20);
    memccpy(Teste.CH3_Name,"TesteCH",20,20);
    memccpy(Teste.CH4_Name,"TesteCH",20,20);
    memccpy(Teste.CH5_Name,"TesteCH",20,20);

    memccpy(Teste.CardName,"CardName",20,20);
    strcat(Teste.CardName,".txt");

    Ptr = fopen(Teste.CardName,"w");
    Teste.RecFile = Ptr;

    memccpy(Teste.CardName,"CardName_Ev",20,20);
    strcat(Teste.CardName,".txt");
    Ptr2 = fopen(Teste.CardName,"w");
    Teste.EvFile = Ptr2;

    writeHeader(&Teste);
    writeRec(&Teste);
    writeEvent(&Teste);

    return 0;
}
