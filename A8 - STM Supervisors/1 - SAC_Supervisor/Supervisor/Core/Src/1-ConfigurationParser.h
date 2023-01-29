#ifndef _CONFIG_PARSE_H_
#define _CONFIG_PARSE_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "fatfs.h"
#include "main.h"

#define LINE_LIMIT 		150
#define SAC_CONFIG_NUM 	50

#define MASTER 			0x10
#define COM				0x11
/**
 * @brief  Union to transmit floats to and from CAN and USART bus
 * @note   
 * @retval 
 */
union Data
{
    uint8_t String[4];
    float Number;
};

typedef struct 
{
    char Values[LINE_LIMIT][128];
    char Labels[LINE_LIMIT][128];
} RawConfig;

/**
 * @brief  Sensor Channel Structure
 * @note   
 * @retval None
 */
typedef struct 
{
    uint8_t ActiveFlag;
    float Gain;
    float Bias;
    float Offset;

    char ChannelNickname[64];
    uint8_t CH_Id;              // ID Correspondent to IOC
    uint8_t GPIO;

    uint8_t FilterFlag;
    uint8_t RMS_Flag;
    uint8_t ABS_Flag;

    float ABSUpLimit;
    float ABSLowLimit;

    float RMSBuffer[5];
    uint8_t RMSPointer;
    float RMSUpLimit;
    float RMSLowLimit;

    float FiltPrevVal;
    float FiltGain;
    float FiltTC;
    float FiltUpLimit;
    float FiltLowLimit;

    float RMS_Value;
    float ABS_Value;
    float FiltValue;

    uint8_t AOK_Signal;         // AOK signal for the comparaison value
    uint8_t Fail_Signal;        // Fail signal for the comparaison value
} SENSOR_CH;

/**
 * @brief  Card specific structure
 * @note   SAC Example  
 * @retval None
 */
typedef struct 
{
    uint8_t NO_ActiveSensors;   // Number of active sensors
    
    uint8_t RecordTime;           // How many supervision cycles between record
    uint8_t EventTime;            // How many supervision cycles between events record

    char RecordFolderName[256];     // File system record folder name
    char EventFolderName[256];      // File system event folder name

    char RecordFileName[256];     // File system record folder name
    char EventFileName[256];      // File system event folder name

    uint32_t MaxRecordFolder;   // Maximum record folder size before deleting old files
    uint32_t MaxEventFolder;   // Maximum event folder size before deleting old files

    uint32_t MaxRecordFile;     // Maximum record folder size before creating new file
    uint32_t MaxEventFile;      // Maximum event folder size before creating new file

    FIL* RecordFile;
    FIL* EventFile;

    uint32_t RecordFileCount;
    uint32_t EventFilecount;

    uint8_t ConfiguredFlag;

    float EventBuffer[256][18];
    uint32_t BufferPointer;
    uint32_t SuperEvCounter;
    uint32_t SuperRecCounter;

    SENSOR_CH Sensors[6];         // Sensor array
    ADC_HandleTypeDef* adc;		// ADC HAL struct

}SAC_CARD;

/**
 * @brief Generic structure for any card  
 * @note   
 * @retval None
 */
typedef struct 
{
    char Card_Name[128];        // Card nickname
    uint8_t Card_Id;            // CAN Self Id
    uint8_t ConfiguredFlag;

    uint8_t ACH[2];             // CAN Ids of accepted CAN Txs
    uint8_t RecentRx;			// Last Id received

    float SuperFSW;             // Frequency in HZ of the Supervision Routine
    float SuperTime;
    TIM_HandleTypeDef* sTim;	// Supervision timer

    RTC_HandleTypeDef *sRTC; 	// RTC handler
    RTC_DateTypeDef Date;
	RTC_TimeTypeDef Time;

	CAN_HandleTypeDef* CAN;
	CAN_TxHeaderTypeDef Tx;
    // Card Specific Structure
    SAC_CARD SAC;

} GeneralCard;


int ReadConfigFile(RawConfig* Config);
int Parser(GeneralCard* Card, RawConfig* Config);
int startNewRecordFile(GeneralCard* Card);
int recordFile(GeneralCard* Card);
int saveEvent(GeneralCard* Card,uint8_t CH,uint8_t Flag);
int CreateFoldersStartHeaders(GeneralCard* Card);

#endif
