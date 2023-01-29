#ifndef _CONFIG_PARSE_H_
#define _CONFIG_PARSE_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "fatfs.h"

#define LINE_LIMIT 150

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
    char Values[LINE_LIMIT][1024];
    char Labels[LINE_LIMIT][1024];
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

    char ChannelNickname[1024];
    uint8_t CH_Id;              // ID Correspondent to IOC
    uint8_t GPIO;

    uint8_t FilterFlag;
    uint8_t RMS_Flag;
    uint8_t ABS_Flag;

    float ABSUpLimit;
    float ABSLowLimit;

    float RMSUpLimit;
    float RMSLowLimit;

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
    
    float RecordTime;           // Time between lines being written in the record file
    float EventTime;            // Sample rate of event record file

    char RecordFolderName[512];     // File system record folder name
    char EventFolderName[512];      // File system event folder name

    uint32_t MaxRecordFolder;   // Maximum record folder size before deleting old files
    uint32_t MaxEventFolder;   // Maximum event folder size before deleting old files

    uint32_t MaxRecordFile;     // Maximum record folder size before creating new file
    uint32_t MaxEventFile;      // Maximum event folder size before creating new file

    SENSOR_CH Sensors[6];         // Sensor array
    
}SAC_CARD;

/**
 * @brief Generic structure for any card  
 * @note   
 * @retval None
 */
typedef struct 
{
    char Card_Name[128];            // Card nickname
    uint8_t Card_Id;            // CAN Self Id

    uint8_t ACH[2];               // CAN Ids of accepted CAN Txs

    float SuperFSW;             // Frequency in HZ of the Supervision Routine

    // Card Specific Structure
    SAC_CARD SAC;

} GeneralCard;


void ReadConfigFile(RawConfig* Config);
void Parser(GeneralCard* Card, RawConfig* Config);

#endif
