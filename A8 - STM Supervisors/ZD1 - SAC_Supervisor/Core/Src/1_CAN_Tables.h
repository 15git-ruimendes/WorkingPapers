/*
 * CAN_Tables.h
 *
 *  Created on: Nov 23, 2022
 *      Author: RuiMendes
 */

#ifndef SRC_CAN_TABLES_H_
#define SRC_CAN_TABLES_H_

#include "main.h"
#include "eeprom.h"
#include "3_Supervisor_Routines.h"

// CAN IDs Table

#define MASTER_CARD    	0xA
#define LOGCOM_CARD     0x9
#define DEBUG_CARD      0x8
#define SAC_CARD_1      0x7
#define SAC_CARD_2      0x6
#define SAC_CARD_3      0x5
#define IIS_CARD_1      0x4
#define IIS_CARD_2		0x3
#define GPRO_CARD_1     0x2
#define GPRO_CARD_2     0x1
#define TAS_CARD		0x0

/*
 * Card Specific Commands
 */
#define ACTIVATION		0x5
#define STOP			0x4
#define START			0x3
#define PUBLISH			0x2
#define FAILURE			0x1
#define CALIBRATION		0x0

// Card Structure

typedef struct
{
    uint8_t State; 			// General Inner Function State (i.e. Idle, Active, Calibration, Running, Failure)
    uint8_t SuperState; 	// State of the supervision routine (i.e. AOK, Failure CH1, Single PWM, HB Mode)

    uint8_t LastRxSource;	// Last Rx message source (i.e. Master, ComLog, Debug)
    uint8_t LastRxFunction; // Last Rx message function (i.e. Activate, Start ...)

    uint8_t LastMsgTxDest;		// Last Tx message destination (i.e. Master, ComLog, Debug)
    uint8_t LastMsgTxFunction;	// Last Tx message function (i.e. Activate, Start ...)

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


/*
 * Card Specific Structure
 */

typedef struct
{
	TypedefCard Card;

	ADC_HandleTypeDef* hadc;

	float SensGains[6];
	float SensOffset[6];
	float SensBias[6];

	float SensLimitsU[6];
	float SensLimitsD[6];

	float SensValues[6];
	uint32_t SensStates[6];

}TypedefSAC;

/*
 * Parser Functions
 */

/*
 * CAN Identifier Parser
 */

uint8_t IdentParser(TypedefSAC *SAC_Card)
{
	// Save Rx Id in separate variable
	uint16_t Header = SAC_Card->Card.rxHeader.StdId;

	// Parse
	uint8_t Source = (Header >> 5) >> 7;
	uint8_t Destination = ((Header >> 5) >> 3) & 0b00000000000000000000000000001111;
	uint8_t Function = (Header >> 5) & 0b00000000000000000000000000000111;

	// Check if destination Id is valid
	if (Destination != SAC_Card->Card.CardId)
	{
		return 0;
	}

	// Insert Sender and Function
	SAC_Card->Card.LastRxSource = Source;
	SAC_Card->Card.LastRxFunction = Function;
	return Source;

};

/*
 * CAN Identifier Builder
 */

void IdentBuilder(TypedefSAC *SAC_Card, uint8_t DestCard, uint8_t Function)
{
	// Build identifier
	uint32_t Header = SAC_Card->Card.CardId<<7;
	Header = Header + (DestCard << 3);
	Header = Header + (Function);
	Header = Header << 5;

	// Save to txHeader
	SAC_Card->Card.txHeader.StdId = Header;
};

/*
 * CAN Rx Parser
 */

void RxParser(TypedefSAC *SAC_Card)
{
	uint8_t RXBuffer[8];
	uint8_t RXBuffer_Size;
	uint8_t FunctionMode;
	uint8_t MessageCount;
	uint8_t CommandSize;
	uint8_t DataBytes;

	// Check for correct data size
	if (SAC_Card->Card.rxHeader.DLC < 1)
		return;

	// Parse header
	if (!IdentParser(SAC_Card))
		return;

	// Check Source
	if (SAC_Card->Card.LastRxSource != MASTER_CARD ||
		SAC_Card->Card.LastRxSource != LOGCOM_CARD ||
		SAC_Card->Card.LastRxSource != DEBUG_CARD)
	{
		return;
	}


	RXBuffer_Size = SAC_Card->Card.rxHeader.DLC;
	memcpy(RXBuffer,SAC_Card->Card.RxData,RXBuffer_Size);

	// Main Switch-Case
	switch (SAC_Card->Card.LastRxFunction)
	{
	case ACTIVATION:
		activRoutine(SAC_Card);
	case STOP:
		stopRoutine(SAC_Card);
	case START:
		startRoutine(SAC_Card);
	case PUBLISH:
		publishRoutine(SAC_Card);
	case FAILURE:
		stopRoutine(SAC_Card);
	case CALIBRATION:
		calibRoutine(SAC_Card);
	}
};


/*
 * CAN Tx Parser
 */

void TxParser(TypedefSAC *SAC_Card,uint8_t MessageType,...)
{

};

#endif /* SRC_CAN_TABLES_H_ */
