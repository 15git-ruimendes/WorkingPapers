#include "1-ConfigurationParser.h"

int startNewRecordFile(GeneralCard* Card)
{
	FRESULT fresult;
	/*
	 * Close current record file
	 */
	if (Card->SAC.RecordFile != NULL)
	{
	fresult = f_close(Card->SAC.RecordFile);
	if (fresult != FR_OK)
		Error_Handler();

	}

	/*
	 * Check folder size and open new file
	 */


	if (Card->SAC.RecordFileCount == Card->SAC.MaxRecordFolder)
	{
		DIR dp;
		uint32_t Oldest = 0xFFFFFFFF;
		uint32_t Last = 0;
		fresult = f_opendir(&dp,Card->SAC.RecordFolderName);
		if (fresult != FR_OK)
			Error_Handler();

		FILINFO fno,*fnres;
		for (uint8_t i = 0;i< Card->SAC.MaxRecordFolder;i++)
		{
			fresult = f_readdir(&dp,&fno);
			if (fresult != FR_OK)
				Error_Handler();
			Last = (uint16_t)((fno.fdate) << 16 | fno.ftime);
			if (Last < Oldest)
			{
				Oldest = Last;
				fnres = &fno;
			}
		}
		char AuxPath[128];
		strcpy(AuxPath,Card->SAC.RecordFolderName);
		strcat(AuxPath,fnres->altname);
		fresult = f_unlink(AuxPath);
		if (fresult != FR_OK)
			Error_Handler();
	}

	Card->SAC.RecordFileCount++;


	char timestr[128],datestr[128],aux[128];
	HAL_RTC_GetTime(Card->sRTC, &Card->Time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(Card->sRTC, &Card->Date, RTC_FORMAT_BIN);
	sprintf((char*)timestr,"%02d_%02d_%02d.csv",Card->Time.Hours, Card->Time.Minutes, Card->Time.Seconds);
	sprintf((char*)datestr,"%02d_%02d_%2d_",Card->Date.Date, Card->Date.Month, 2000 + Card->Date.Year);

	memcpy(Card->SAC.RecordFileName,Card->SAC.RecordFolderName,256);
	strcat(Card->SAC.RecordFileName,"/");
	strcat(Card->SAC.RecordFileName,datestr);
	strcat(Card->SAC.RecordFileName,timestr);
	fresult = f_open(Card->SAC.RecordFile,Card->SAC.RecordFileName,FA_WRITE | FA_CREATE_ALWAYS);
	if (fresult != FR_OK)
		Error_Handler();


	/*
	 * Try on headers
	 */
	for (uint8_t i = 0; i < 18; i++)
	{
		if (Card->SAC.Sensors[i].ActiveFlag)
		{
			strcat(aux, Card->SAC.Sensors[i].ChannelNickname);
			if (i < 17) strcat(aux, ",");
		}
	}
	fresult = f_puts(aux,Card->SAC.RecordFile);
	if (fresult != FR_OK)
		Error_Handler();

	/*
	 * return successful
	 */
	return 1;
}


int recordFile(GeneralCard* Card)
{
	FRESULT fresult;
	if (f_size(Card->SAC.RecordFile) > Card->SAC.MaxRecordFile)
		startNewRecordFile(Card);

	char aux[128],aux_data[64];
	for (uint8_t i = 0; i < 6; i++)
	{
		if (Card->SAC.Sensors[i].ActiveFlag)
		{
			sprintf(aux_data,"%.6f,%.6f,%.6f",
					Card->SAC.Sensors[i].ABS_Value,
					Card->SAC.Sensors[i].RMS_Value,
					Card->SAC.Sensors[i].FiltValue);
			strcat(aux, aux_data);
			if (i < 5) strcat(aux, ",");
		}
	}
	fresult = f_puts(aux,Card->SAC.RecordFile);
	if (fresult != FR_OK)
		Error_Handler();
	return 1;
}

int saveEvent(GeneralCard* Card,uint8_t CH,uint8_t Flag)
{
	return 1;
}

int CreateFoldersStartHeaders(GeneralCard* Card)
{
	/*
	 * Get RTC time
	 */

	HAL_RTC_GetTime(Card->sRTC, &Card->Time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(Card->sRTC, &Card->Date, RTC_FORMAT_BIN);

	/*
	 * Create Event and Record Directory
	 */
	FRESULT Res;
	Res = f_mkdir((TCHAR*)Card->SAC.RecordFolderName);
	if (Res != FR_OK || Res != FR_EXIST)
		Error_Handler();

	Res = f_mkdir((TCHAR*)Card->SAC.EventFolderName);
	if (Res != FR_OK || Res != FR_EXIST)
		Error_Handler();

	startNewRecordFile(Card);

	return 1;

}


int ReadConfigFile(RawConfig* Config)
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
    return LineCount;
}

int Parser(GeneralCard* SAC_Card, RawConfig* Config)
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
    SAC_Card->SuperTime = 1/SAC_Card->SuperFSW;
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
    for (uint8_t i = 0,j = 14; i < 6; i++,j+=6)
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
    SAC_Card->ConfiguredFlag = 1;
    return 1;
}
