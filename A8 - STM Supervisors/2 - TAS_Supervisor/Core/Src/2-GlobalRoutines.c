#include "2-GlobalRoutines.h"

#define START 	1
#define STOP 	2
#define RECAL	3

#define ABS 	1
#define RMS 	2
#define FILT 	3
#define SFL		100

#define FAIL	0x11
#define WARN	0x10

#define RMS_CST	0.447213595499957939f

/**
 * Other Functions
 */

void lowerSFL()
{
	HAL_GPIO_WritePin(SFLH_Out_GPIO_Port,SFLH_Out_Pin,SET);
	HAL_GPIO_WritePin(SFLL_Out_GPIO_Port,SFLL_Out_Pin,SET);
	return;
}

/**
 * Supervision Routines
 */
void superRoutine(GeneralCard* Card)
{
	/*
	 * Static Variables
	 */
	static uint8_t OutSideSFL = 0;

	/*
	 * Get Temperatures
	 */
	for (uint8_t i = 0;i < 8; i++)
	{
		if (Card->TAS.PT100_SENS[i].ActiveFlag)
		{
			Card->TAS.PT100_SENS[i].CurrentTemperature = MAX31865_readTemp(Card,i);
		}
	}

	/*
	 * Check Thermostat
	 */
	for (uint8_t i = 0;i < 8; i++)
	{
		if (Card->TAS.TERMSTATE[i].ActiveFlag)
		{
			Card->TAS.TERMSTATE[i].State = HAL_GPIO_ReadPin(Card->TAS.TERMSTATE[i].TERM_Port,Card->TAS.TERMSTATE[i].TERM_Pin);
		}
	}

	/*
	 * Check SFL
	 */
	if ((!HAL_GPIO_ReadPin(SFLH_In_GPIO_Port,SFLH_In_Pin) || HAL_GPIO_ReadPin(SFLL_In_GPIO_Port,SFLL_In_Pin)) && !OutSideSFL)
	{
		saveEvent(Card,100,SFL);
		OutSideSFL = 1;
	}

	/*
	 * Check if Out-Of-Bounds
	 */
	for (uint8_t i = 0;i < 8; i++)
	{
		 if (Card->TAS.PT100_SENS[i].CurrentTemperature > Card->TAS.PT100_SENS[i].LimitTemperature)
		 {
			 /*
			  * Lower SFL
			  */

			 lowerSFL();

			 uint8_t Message[5];
			 union Data d;
			 d.Number = Card->TAS.PT100_SENS[i].CurrentTemperature;
			 memcpy(Message,d.String,4);
			 Message[4] = i;
			 Card->Tx.DLC = 6;
			 Card->Tx.IDE = CAN_ID_STD;
			 Card->Tx.RTR = CAN_RTR_DATA;
			 Card->Tx.StdId = idBuilder(Card, MASTER, FAIL);
			 uint32_t MailBox;
			 HAL_CAN_AddTxMessage(Card->CAN,&Card->Tx,Message,&MailBox);
		 }
		 else if (Card->TAS.PT100_SENS[i].CurrentTemperature > Card->TAS.PT100_SENS[i].WarningTemperature)
		 {
			 uint8_t Message[5];
			 union Data d;
			 d.Number = Card->TAS.PT100_SENS[i].CurrentTemperature;
			 memcpy(Message,d.String,4);
			 Message[4] = i;
			 Card->Tx.DLC = 6;
			 Card->Tx.IDE = CAN_ID_STD;
			 Card->Tx.RTR = CAN_RTR_DATA;
			 Card->Tx.StdId = idBuilder(Card, MASTER, WARN);
			 uint32_t MailBox;
			 HAL_CAN_AddTxMessage(Card->CAN,&Card->Tx,Message,&MailBox);
		 }
	}

	for (int i = 0;i < 6; i++)
	{
		 if (!Card->TAS.TERMSTATE[i].State)
		 {
			 /*
			  * Lower SFL
			  */

			 lowerSFL();

			 uint8_t Message[5];
			 union Data d;
			 d.Number = Card->TAS.PT100_SENS[i].CurrentTemperature;
			 memcpy(Message,d.String,4);
			 Message[4] = i;
			 Card->Tx.DLC = 6;
			 Card->Tx.IDE = CAN_ID_STD;
			 Card->Tx.RTR = CAN_RTR_DATA;
			 Card->Tx.StdId = idBuilder(Card, MASTER, FAIL);
			 uint32_t MailBox;
			 HAL_CAN_AddTxMessage(Card->CAN,&Card->Tx,Message,&MailBox);
		 }
	}
	/*
	 * Increase Event Buffer and Check for Record Time
	 */

	Card->TAS.SuperRecCounter++;

	if (Card->TAS.SuperRecCounter >= Card->TAS.RecordTime)
	{
		Card->TAS.SuperRecCounter = 0;
		recordFile(Card);
	}

}

/**
 * Start Routine
 */
void startRoutine(GeneralCard* Card)
{
	/**
	 * Create Folder Structures
	 */
	CreateFoldersStartHeaders(Card);

	/*
	 * Initialise MAX31865s
	 */
	MAX31865_init(Card,3);

	/**
	 * Configures Timer
	 */
	uint32_t SystFreq = HAL_RCC_GetHCLKFreq();

	Card->sTim->Init.Prescaler = 2;
	Card->sTim->Init.Period = (uint32_t)(SystFreq/Card->SuperFSW*0.5);

	if (HAL_TIM_Base_Init(Card->sTim) != HAL_OK)
	{
	   Error_Handler();
	}

	/*
	 * Start Interrupt
	 */
	HAL_TIM_Base_Start_IT(Card->sTim);

	/*
	 * Transmit Start Successful
	 */
	uint8_t Data[2];
	uint8_t DLC = 2;
	canTX(Card,Data,DLC,FILT);
}

/**
 * Stop Routine not used in SAC
 */
void stopRoutine(GeneralCard* Card)
{

}

/*
 * Recal Routine
 */
void recalRoutine(GeneralCard* Card)
{

}


/***********************************
 * CAN Message Routing
 **********************************/
/**
 * CAN Id Builder
 */
uint32_t idBuilder(GeneralCard* Card,uint8_t Dest, uint8_t Function)
{
	// Build identifier
	uint32_t Header = Card->Card_Id<<7;
	Header = Header + (Dest << 3);
	Header = Header + (Function);
	Header = Header << 5;

	return Header;
}

/**
 * CAN Id Parser
 */
uint8_t idParser(GeneralCard* Card, uint32_t Id)
{
	// Save Rx Id in separate variable
		uint16_t Header = Id;

		// Parse
		uint8_t Source = (Header >> 5) >> 7;
		uint8_t Destination = ((Header >> 5) >> 3) & 0b00000000000000000000000000001111;
		uint8_t Function = (Header >> 5) & 0b00000000000000000000000000000111;

		// Check Source Id
		if (Source != Card->ACH[0] && Source != Card->ACH[1])
			return 0;

		// Check if destination Id is valid
		if (Destination != Card->Card_Id)
			return 0;

		Card->RecentRx = Source;
		// Return Function
		return Function;
}

/**
 * CAN Router
 */
void canParser(GeneralCard* Card, uint32_t DLC,CAN_RxHeaderTypeDef* RxHeader,uint8_t *Data)
{
	uint8_t Function;

	/*
	 * Check Payload Size
	 */
	if (DLC < 1)
		return;

	/*
	 * Check if relevant id
	 */
	Function = idParser(Card,RxHeader->StdId);
	if (!Function)
		return;

	/*
	 * Parse Function
	 */
	switch (Function)
		{
		case START:
			startRoutine(Card);
		case STOP:
			stopRoutine(Card);
		case RECAL:
			recalRoutine(Card);
		}

	return;
}

/**
 * CAN Tx Build
 */
void canTX(GeneralCard* Card,uint8_t* Data,uint8_t DLC,uint8_t Function)
{

}
