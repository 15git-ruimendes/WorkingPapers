/*
 * routines.h
 *
 *  Created on: Nov 17, 2022
 *      Author: ruimendes
 */

#include "main.h"

// CAN IDs Table

#define CONTROL_CARD    0xA0
#define SAC_CARD        0x10
#define IIS_CARD        0x20
#define GPRO_CARD       0x30
#define SDI_CARD        0x40
#define ILOG_CARD       0x50
#define COM_CARD        0x60
#define EMERGENCY       0x01

// CAN Rx Flags

#define INIT_FLAG_RX    0x10
#define START_FLAG_RX   0x20
#define RECAL_VAL_RX    0x30
#define RECAL_STT_RX    0x40
#define RECAL_EVNT_Rx   0x50
#define SHUT_DW_RX      0x60
#define EMERGENCY_RX    0x70

// CAN Tx Flags

#define INIT_OK_TX      0xA1
#define START_OK_TX     0xA2
#define VAL_RET_TX      0xA3
#define STT_RET_TX      0xA4
#define EVNT_RET_TX     0xA5
#define SHUT_DW_OK_TX   0xA6
#define INIT_FAIL_TX    0xA7
#define START_FAIL_TX   0xA8

// Card Structure

typedef struct 
{
    uint8_t State;
    uint8_t SuperState;

    uint8_t* LastMsgRx;
    uint8_t  LastMsgRxSize;

    uint8_t* LastMsgTx;
    uint8_t LastMsgTxSize;

    uint8_t* ValueLog;
    uint8_t ValueLogSize;

    uint8_t* EventLog;
    uint8_t EventLogSize;

    uint8_t* AcceptableAddr;
    uint8_t NoAcceptableAddr;

    CAN_HandleTypeDef* CANChannel;
    CAN_RxHeaderTypeDef rxHeader;
    CAN_TxHeaderTypeDef txHeader;
    CAN_FilterTypeDef filtCAN;

    uint8_t CardId;
} TypedefCard;

// SAC Specific Strutures

typedef struct 
{
    TypedefCard SelfCard;

    float UpperLimits[6];
    float LowerLimits[6];

    float ADCValues[6];

    float Gains[6];
    float Offset[6];

    uint8_t CH1_State;
    uint8_t CH2_State;
    uint8_t CH3_State;
    uint8_t CH4_State;
    uint8_t CH5_State;
    uint8_t CH6_State;
} SAC_Card;


// Structure Initialization (Card Specific)

SAC_Card StrucInit(CAN_HandleTypeDef* Channel)
{
    SAC_Card Strct;

};