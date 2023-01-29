/*
 * CAN_Tables.h
 *
 *  Created on: Nov 23, 2022
 *      Author: RuiMendes
 */

#ifndef SRC_CAN_TABLES_H_
#define SRC_CAN_TABLES_H_

#include "main.h"

// CAN IDs Table

#define MASTER_CARD    	0x01
#define SAC_CARD_1      0xA1
#define SAC_CARD_2      0xA2
#define SAC_CARD_3      0xA3
#define GPRO_CARD_1     0xB1
#define GPRO_CARD_2     0xB2
#define IIS_CARD_1      0xC1
#define IIS_CARD_2		0xC2
#define TAS_CARD		0xD1
#define LOGCOM_CARD     0xE1
#define DEBUG_CARD      0xF1

// CAN Rx Commands

#define RX_EMERGENCY    0x1
#define RX_STOP			0x2
#define RX_INIT			0x3
#define RX_START		0x4
#define RX_REVAL		0xA
#define RX_RESTT		0xB
#define RX_REEVNT		0xC
#define RX_ACTIVE		0xF

// CAN Tx Commands

#define TX_STOP_OK		0x1
#define TX_INIT_OK		0x3
#define TX_START_OK		0x4
#define TX_RETVAL		0xA
#define TX_RETSTT		0xB
#define TX_RETEVNT		0xC
#define TX_ACTIV_OK		0xF
#define ERROR_TX		0x0

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

    uint8_t RxData[8];
    uint8_t TxData[8];
    uint32_t TxMailbox;
    CAN_HandleTypeDef* CANChannel;
    CAN_RxHeaderTypeDef rxHeader;
    CAN_TxHeaderTypeDef txHeader;
    CAN_FilterTypeDef filtCAN;

    TIM_HandleTypeDef* htim;

    uint8_t CardId;
} TypedefCard;

#endif /* SRC_CAN_TABLES_H_ */
