#include "ConfigurationParser.h"

void ReadConfigFile(RawConfig* Config)
{
	FIL fil;
	FRESULT fresult;

	fresult = f_open(&fil,"SAC1-ConfigurationFile.csv",FA_READ);
	if (fresult != FR_OK)
		Error_Handler();

    int LineCount = 1;
    char LineBuffer[1024];
    while (f_gets(LineBuffer,1024,&fil) != NULL)
    {
        strcpy(Config->Labels[LineCount-1],strtok(LineBuffer,","));
        strcpy(Config->Values[LineCount-1],strtok(NULL,","));
        LineCount++;
        //HAL_UART_Transmit(&huart6,(uint8_t *)buffer,26,1000);
    }


    f_close(&fil);
    return;
}

void Parser(GeneralCard* SAC_Card, RawConfig* Config)
{
    // Set Name
    memcpy(SAC_Card->Card_Name,Config->Values[0],128);

    // CAN Id
    SAC_Card->Card_Id = atoi(Config->Values[1]);

    // Accept Master Comms
    if (strstr(Config->Values[2],"FALSE") != NULL);
    else
    {
        SAC_Card->ACH[0] = atoi(Config->Values[2]);
    }
    // Accept Communication Card Tx
    if (strstr(Config->Values[3],"FALSE") != NULL);
    else
    {
        SAC_Card->ACH[1] = atoi(Config->Values[3]);
    }
    
    // Supervisor frequency
    SAC_Card->SuperFSW = atof(Config->Values[4]);
    // Number of channels
    SAC_Card->SAC.NO_ActiveSensors = atoi(Config->Values[5]);

    SAC_Card->SAC.RecordTime = atof(Config->Values[6]);
    SAC_Card->SAC.EventTime = atof(Config->Values[7]);

    SAC_Card->SAC.MaxRecordFolder = atoi(Config->Values[8]);
    SAC_Card->SAC.MaxEventFolder = atoi(Config->Values[9]);

    SAC_Card->SAC.MaxRecordFile = atoi(Config->Values[10]);
    SAC_Card->SAC.MaxEventFile = atoi(Config->Values[11]);

    memcpy(SAC_Card->SAC.RecordFolderName,Config->Values[12],512);
    memcpy(SAC_Card->SAC.EventFolderName,Config->Values[13],512);

    // CH configs
    for (int i = 0,j = 14; i < 6; i++,j+=6)
    {
    if (strstr(Config->Values[j],"TRUE") != NULL) SAC_Card->SAC.Sensors[i].ActiveFlag = 1;
    else SAC_Card->SAC.Sensors[i].ActiveFlag = 0;
    if (SAC_Card->SAC.Sensors[i].ActiveFlag)
    {
        memcpy(SAC_Card->SAC.Sensors[i].ChannelNickname,Config->Values[j+1],512);
        SAC_Card->SAC.Sensors[i].Gain = atof(Config->Values[j+2]);
        SAC_Card->SAC.Sensors[i].Bias = atof(Config->Values[j+3]);
        SAC_Card->SAC.Sensors[i].Offset = atof(Config->Values[j+4]);

        if (strstr(Config->Values[j+5],"TRUE") != NULL)
        {
            SAC_Card->SAC.Sensors[i].ABS_Flag = 1;
            SAC_Card->SAC.Sensors[i].ABSUpLimit = atof(Config->Values[j+6]);
            SAC_Card->SAC.Sensors[i].ABSLowLimit = atof(Config->Values[j+7]);
        }
        else SAC_Card->SAC.Sensors[i].ABS_Flag = 0;

        if (strstr(Config->Values[j+8],"TRUE") != NULL)
        {
            SAC_Card->SAC.Sensors[i].FilterFlag = 1;
            SAC_Card->SAC.Sensors[i].FiltGain = atof(Config->Values[j+9]);
            SAC_Card->SAC.Sensors[i].FiltTC = atof(Config->Values[j+10]);
            SAC_Card->SAC.Sensors[i].FiltUpLimit = atof(Config->Values[j+11]);
            SAC_Card->SAC.Sensors[i].FiltLowLimit = atof(Config->Values[j+12]);
        }
        else SAC_Card->SAC.Sensors[i].FilterFlag = 0;

        if (strstr(Config->Values[j+13],"TRUE") != NULL)
        {
            SAC_Card->SAC.Sensors[i].RMS_Flag = 1;
            SAC_Card->SAC.Sensors[i].RMSUpLimit = atof(Config->Values[j+14]);
            SAC_Card->SAC.Sensors[i].RMSLowLimit = atof(Config->Values[j+15]);
        }
        else SAC_Card->SAC.Sensors[i].RMS_Flag = 0;
    }
    }
}
