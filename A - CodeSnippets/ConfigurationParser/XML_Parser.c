#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define GENERAL 1
#define ALLOWED_ID 2
#define CARD 3
#define CH_CONFIG 4
#define CH 5
#define OPERATIONS 6

typedef struct
{
    uint32_t FileSize;
}RecordsConfig;

typedef struct
{
    char* Name;
    float Upper_Limits;
    float Lower_Limits;
    float Frequency;
    float Gain;
    float Time_Constant;
}Operations;

typedef struct
{
    char* NickName;
    uint8_t Id;

    float gain;
    float bias;
    float offset;

    Operations* Ops;
    RecordsConfig* Rec;
}Channels;

typedef struct
{
    char* Name;
    uint8_t Id;
    uint8_t *Perm_Id;

    uint8_t CH_Num;

    Channels* CHs;
}Card;

Card SAC;
uint8_t Mode;
void parse_line(char* line)
{
    

    if(strstr(line,"<general>"))
        Mode = GENERAL;
    if(strstr(line,"<allowed_id>"))
        Mode = ALLOWED_ID;
    if(strstr(line,"<allowed_id>"))
        Mode = ALLOWED_ID;
    if(strstr(line,"<card>"))
        Mode = CARD;
    if(strstr(line,"<CH_Config>"))
        Mode = CH_CONFIG;
    if(strstr(line,"<CH>"))
        Mode = CH;
    if(strstr(line,"<operations>"))
        Mode = OPERATIONS;

    char *p1,*p2,*aux;

    switch (Mode)
    {
    case GENERAL:
        if((p1 = strstr(line,"<name>")) && (p2 = strstr(p1,"</name>")))
        {
            aux = p2 - p1 - 6, p1 + 6;
            SAC.Name = (char *)malloc(8*strlen(aux));
            memccpy(SAC.Name,aux,1024,strlen(aux));
        }
        break;
    
    default:
        break;
    }

}

int main(int argc, char const *argv[])
{
    FILE *fptr;

    fptr = fopen("SAC_ConfigFile.xml","r");
    char c[1024];
    char *aux;
    while (fgets(c,1024,fptr) != NULL)
    {
        parse_line(c);
        printf("%s",c);
    }
    printf("%s\n",SAC.Name);
    return 0;
}
