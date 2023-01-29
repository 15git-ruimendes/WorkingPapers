#include "1-ConfigurationParser.h"

int startNewRecordFile(GeneralCard* Card)
{
	FRESULT fresult;
	/*
	 * Close current record file
	 */
	if (Card->TAS.RecordFile != NULL)
	{
	fresult = f_close(Card->TAS.RecordFile);
	if (fresult != FR_OK)
		Error_Handler();

	}

	/*
	 * Check folder size and open new file
	 */


	if (Card->TAS.RecordFileCount == Card->TAS.MaxRecordFolder)
	{
		DIR dp;
		uint32_t Oldest = 0xFFFFFFFF;
		uint32_t Last = 0;
		fresult = f_opendir(&dp,Card->TAS.RecordFolderName);
		if (fresult != FR_OK)
			Error_Handler();

		FILINFO fno,*fnres;
		for (uint8_t i = 0;i< Card->TAS.MaxRecordFolder;i++)
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
		strcpy(AuxPath,Card->TAS.RecordFolderName);
		strcat(AuxPath,fnres->altname);
		fresult = f_unlink(AuxPath);
		if (fresult != FR_OK)
			Error_Handler();
	}

	Card->TAS.RecordFileCount++;


	char timestr[128],datestr[128],aux[128];
	HAL_RTC_GetTime(Card->sRTC, &Card->Time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(Card->sRTC, &Card->Date, RTC_FORMAT_BIN);
	sprintf((char*)timestr,"%02d_%02d_%02d.csv",Card->Time.Hours, Card->Time.Minutes, Card->Time.Seconds);
	sprintf((char*)datestr,"%02d_%02d_%2d_",Card->Date.Date, Card->Date.Month, 2000 + Card->Date.Year);

	memcpy(Card->TAS.RecordFileName,Card->TAS.RecordFolderName,256);
	strcat(Card->TAS.RecordFileName,"/");
	strcat(Card->TAS.RecordFileName,datestr);
	strcat(Card->TAS.RecordFileName,timestr);
	fresult = f_open(Card->TAS.RecordFile,Card->TAS.RecordFileName,FA_WRITE | FA_CREATE_ALWAYS);
	if (fresult != FR_OK)
		Error_Handler();


	/*
	 * Try on headers
	 */
	for (uint8_t i = 0; i < 8; i++)
	{
		if (Card->TAS.PT100_SENS[i].ActiveFlag)
		{
			strcat(aux, Card->TAS.PT100_SENS[i].ChannelNickname);
		}
	}
	for (uint8_t i = 0; i < 8; i++)
	{
		if (Card->TAS.TERMSTATE[i].ActiveFlag)
		{
			strcat(aux, Card->TAS.TERMSTATE[i].ChannelNickname);
			if (i < 7) strcat(aux, ",");
		}
	}
	fresult = f_puts(aux,Card->TAS.RecordFile);
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
	if (f_size(Card->TAS.RecordFile) > Card->TAS.MaxRecordFile)
		startNewRecordFile(Card);

	char aux[128],aux_data[64];

	for (uint8_t i = 0; i < 8; i++)
	{
		if (Card->TAS.PT100_SENS[i].ActiveFlag)
		{
			sprintf(aux_data,"%.6f,",
								Card->TAS.PT100_SENS[i].CurrentTemperature);
			strcat(aux, aux_data);
		}
	}
	for (uint8_t i = 0; i < 8; i++)
	{
		if (Card->TAS.TERMSTATE[i].ActiveFlag)
		{
			sprintf(aux_data,"%d,",
								Card->TAS.TERMSTATE[i].State);
			strcat(aux, aux_data);
			if (i < 7) strcat(aux, ",");
		}
	}

	fresult = f_puts(aux,Card->TAS.RecordFile);
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
	Res = f_mkdir((TCHAR*)Card->TAS.RecordFolderName);
	if (Res != FR_OK || Res != FR_EXIST)
		Error_Handler();

	startNewRecordFile(Card);

	return 1;

}


int ReadConfigFile(RawConfig* Config)
{
	FIL fil;
	FRESULT fresult;

	fresult = f_open(&fil,"TAS-ConfigurationFile.csv",FA_READ);
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

int Parser(GeneralCard* TAS_Card, RawConfig* Config)
{
	/*
	 * Set CS, EN and TS pins
	 */
	TAS_Card->TAS.PT100_SENS[0].CS_Pin = CS_0_Pin;
	TAS_Card->TAS.PT100_SENS[1].CS_Pin = CS_1_Pin;
	TAS_Card->TAS.PT100_SENS[2].CS_Pin = CS_2_Pin;
	TAS_Card->TAS.PT100_SENS[3].CS_Pin = CS_3_Pin;
	TAS_Card->TAS.PT100_SENS[4].CS_Pin = CS_4_Pin;
	TAS_Card->TAS.PT100_SENS[5].CS_Pin = CS_5_Pin;
	TAS_Card->TAS.PT100_SENS[6].CS_Pin = CS_6_Pin;
	TAS_Card->TAS.PT100_SENS[7].CS_Pin = CS_7_Pin;

	TAS_Card->TAS.PT100_SENS[0].CS_Port = CS_0_GPIO_Port;
	TAS_Card->TAS.PT100_SENS[1].CS_Port = CS_1_GPIO_Port;
	TAS_Card->TAS.PT100_SENS[2].CS_Port = CS_2_GPIO_Port;
	TAS_Card->TAS.PT100_SENS[3].CS_Port = CS_3_GPIO_Port;
	TAS_Card->TAS.PT100_SENS[4].CS_Port = CS_4_GPIO_Port;
	TAS_Card->TAS.PT100_SENS[5].CS_Port = CS_5_GPIO_Port;
	TAS_Card->TAS.PT100_SENS[6].CS_Port = CS_6_GPIO_Port;
	TAS_Card->TAS.PT100_SENS[7].CS_Port = CS_7_GPIO_Port;

	TAS_Card->TAS.TERMSTATE[0].EN_Pin = EN_0_Pin;
	TAS_Card->TAS.TERMSTATE[1].EN_Pin = EN_1_Pin;
	TAS_Card->TAS.TERMSTATE[2].EN_Pin = EN_2_Pin;
	TAS_Card->TAS.TERMSTATE[3].EN_Pin = EN_3_Pin;
	TAS_Card->TAS.TERMSTATE[4].EN_Pin = EN_4_Pin;
	TAS_Card->TAS.TERMSTATE[5].EN_Pin = EN_5_Pin;
	TAS_Card->TAS.TERMSTATE[6].EN_Pin = EN_6_Pin;
	TAS_Card->TAS.TERMSTATE[7].EN_Pin = EN_7_Pin;

	TAS_Card->TAS.TERMSTATE[0].EN_Port = EN_0_GPIO_Port;
	TAS_Card->TAS.TERMSTATE[1].EN_Port = EN_1_GPIO_Port;
	TAS_Card->TAS.TERMSTATE[2].EN_Port = EN_2_GPIO_Port;
	TAS_Card->TAS.TERMSTATE[3].EN_Port = EN_3_GPIO_Port;
	TAS_Card->TAS.TERMSTATE[4].EN_Port = EN_4_GPIO_Port;
	TAS_Card->TAS.TERMSTATE[5].EN_Port = EN_5_GPIO_Port;
	TAS_Card->TAS.TERMSTATE[6].EN_Port = EN_6_GPIO_Port;
	TAS_Card->TAS.TERMSTATE[7].EN_Port = EN_7_GPIO_Port;

	TAS_Card->TAS.TERMSTATE[0].TERM_Pin = TS_0_Pin;
	TAS_Card->TAS.TERMSTATE[1].TERM_Pin = TS_1_Pin;
	TAS_Card->TAS.TERMSTATE[2].TERM_Pin = TS_2_Pin;
	TAS_Card->TAS.TERMSTATE[3].TERM_Pin = TS_3_Pin;
	TAS_Card->TAS.TERMSTATE[4].TERM_Pin = TS_4_Pin;
	TAS_Card->TAS.TERMSTATE[5].TERM_Pin = TS_5_Pin;
	TAS_Card->TAS.TERMSTATE[6].TERM_Pin = TS_6_Pin;
	TAS_Card->TAS.TERMSTATE[7].TERM_Pin = TS_7_Pin;

	TAS_Card->TAS.TERMSTATE[0].TERM_Port = TS_0_GPIO_Port;
	TAS_Card->TAS.TERMSTATE[1].TERM_Port = TS_1_GPIO_Port;
	TAS_Card->TAS.TERMSTATE[2].TERM_Port = TS_2_GPIO_Port;
	TAS_Card->TAS.TERMSTATE[3].TERM_Port = TS_3_GPIO_Port;
	TAS_Card->TAS.TERMSTATE[4].TERM_Port = TS_4_GPIO_Port;
	TAS_Card->TAS.TERMSTATE[5].TERM_Port = TS_5_GPIO_Port;
	TAS_Card->TAS.TERMSTATE[6].TERM_Port = TS_6_GPIO_Port;
	TAS_Card->TAS.TERMSTATE[7].TERM_Port = TS_7_GPIO_Port;

    // Set Name
    memcpy(TAS_Card->Card_Name,Config->Values[0],128);

    // CAN Id
    TAS_Card->Card_Id = atoi(Config->Values[1]);

    // Accept Master Comms
    if (strstr(Config->Values[2],"FALSE") != NULL);
    else
    {
        TAS_Card->ACH[0] = atoi(Config->Values[2]);
    }
    // Accept Communication Card Tx
    if (strstr(Config->Values[3],"FALSE") != NULL);
    else
    {
        TAS_Card->ACH[1] = atoi(Config->Values[3]);
    }
    
    // Supervisor frequency
    TAS_Card->SuperFSW = atof(Config->Values[4]);
    TAS_Card->SuperTime = 1/TAS_Card->SuperFSW;
    // Number of channels

    TAS_Card->TAS.RecordTime = atof(Config->Values[6]);

    TAS_Card->TAS.MaxRecordFolder = atoi(Config->Values[7]);

    TAS_Card->TAS.MaxRecordFile = atoi(Config->Values[8]);

    TAS_Card->TAS.PubTime 		= atoi(Config->Values[10]);

    memcpy(TAS_Card->TAS.RecordFolderName,Config->Values[9],256);

    // PT100 Configurations
    for (uint8_t i = 0,j = 11; i < 8; i++,j+=4)
    {
    	if (strstr(Config->Values[j],"0") != NULL);
    	else
    	{
    		TAS_Card->TAS.PT100_SENS[i].ActiveFlag = 1;
    		memcpy(TAS_Card->TAS.PT100_SENS[i].ChannelNickname,Config->Values[j+1],128);
    		TAS_Card->TAS.PT100_SENS[i].WarningTemperature = atof(Config->Values[j+2]);
    		TAS_Card->TAS.PT100_SENS[i].LimitTemperature = atof(Config->Values[j+3]);
    	}
    }
    // Thermostat Configurations
   for (uint8_t i = 0,j = 43; i < 8; i++,j+=2)
   {
	if (strstr(Config->Values[j],"0") != NULL);
	else
	{
		TAS_Card->TAS.TERMSTATE[i].ActiveFlag = 1;
		HAL_GPIO_WritePin(TAS_Card->TAS.TERMSTATE[i].EN_Port,TAS_Card->TAS.TERMSTATE[i].EN_Pin,GPIO_PIN_SET);
		memcpy(TAS_Card->TAS.TERMSTATE[i].ChannelNickname,Config->Values[j+1],128);

	}
   }
    TAS_Card->ConfiguredFlag = 1;
    return 1;
}