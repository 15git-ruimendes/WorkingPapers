#ifndef _CONFIG_PARSE_H_
#define _CONFIG_PARSE_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "fatfs.h"
#include "main.h"

#define LINE_LIMIT 		150
#define TAS_CONFIG_NUM 	44

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

    char ChannelNickname[128];

    uint8_t State;

    GPIO_TypeDef* 	TERM_Port;
    uint16_t			TERM_Pin;

    GPIO_TypeDef* 	EN_Port;
	uint16_t			EN_Pin;

} TERMSTAT;

/**
 * @brief  Sensor Channel Structure
 * @note
 * @retval None
 */
typedef struct
{
    uint8_t ActiveFlag;

    char ChannelNickname[128];

    float CurrentTemperature;
    float WarningTemperature;
    float LimitTemperature;
    GPIO_TypeDef* 	CS_Port;
    uint16_t			CS_Pin;

} PT100;

/**
 * @brief  Card specific structure
 * @note   TAS Example
 * @retval None
 */
typedef struct 
{
    uint8_t NO_ACTIVE_PT100;   // Number of active PT100 sensors
    uint8_t NO_ACTIVE_TERMSTAT;
    
    uint8_t RecordTime;           // How many supervision cycles between record

    char RecordFolderName[256];     // File system record folder name

    char RecordFileName[256];     // File system record folder name

    uint32_t MaxRecordFolder;   // Maximum record folder size before deleting old files

    uint32_t MaxRecordFile;     // Maximum record folder size before creating new file
    FIL* RecordFile;

    uint32_t RecordFileCount;

    uint8_t ConfiguredFlag;

    uint32_t SuperRecCounter;

    PT100 		PT100_SENS[8];         // Sensor array
    TERMSTAT	TERMSTATE[8];

    uint32_t PubTime;

}TAS_CARD;

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
    TAS_CARD TAS;

} GeneralCard;


int ReadConfigFile(RawConfig* Config);
int Parser(GeneralCard* Card, RawConfig* Config);
int startNewRecordFile(GeneralCard* Card);
int recordFile(GeneralCard* Card);
int saveEvent(GeneralCard* Card,uint8_t CH,uint8_t Flag);
int CreateFoldersStartHeaders(GeneralCard* Card);

#endif
