#include <stdio.h>
#include <stdlib.h>
#include <string.h>

union FloatData
{
    float Number;
    char Data[4];
}FData;



int main(int argc, char const *argv[])
{
    char AuxData[4];
    
    FData.Number = 0.0112454;
    memcpy(AuxData,FData.Data,4);
    printf("Number in buffer: %s, Number in Float: %.12f\n",FData.Data,FData.Number);

    FData.Data[0] = AuxData[0] + 100;
    FData.Data[1] = AuxData[1];
    FData.Data[2] = AuxData[2];
    FData.Data[3] = AuxData[3] + 400;
    printf("umber in buffer: %s, Number in Float: %.12f, Comparation with previous: %s\n",FData.Data,FData.Number,AuxData);

    return 0;
}
