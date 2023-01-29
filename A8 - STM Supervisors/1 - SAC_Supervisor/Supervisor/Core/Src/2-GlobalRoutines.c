#include "2-GlobalRoutines.h"

#define START 	1
#define STOP 	2
#define RECAL	3
#define FAIL	0

#define ABS 	1
#define RMS 	2
#define FILT 	3
#define SFL		100

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

float rmsCalc(GeneralCard* Card, uint8_t Id)
{


	Card->SAC.Sensors[Id].RMSBuffer[Card->SAC.Sensors[Id].RMSPointer] = Card->SAC.Sensors[Id].ABS_Value;
	float sum = 0;

	for (int i = 0; i < 5; i++)
	{
		sum += pow(Card->SAC.Sensors[Id].RMSBuffer[i],2);
	}

	Card->SAC.Sensors[Id].RMS_Value = sqrt(sum)*RMS_CST;
	Card->SAC.Sensors[Id].RMSPointer++;
	if (Card->SAC.Sensors[Id].RMSPointer >= 5)
		Card->SAC.Sensors[Id].RMSPointer = 0;

	return Card->SAC.Sensors[Id].RMS_Value;
}

float filtCalc(GeneralCard* Card,uint8_t Id)
{
	Card->SAC.Sensors[Id].FiltValue = Card->SuperTime*Card->SAC.Sensors[Id].ABS_Value + Card->SAC.Sensors[Id].FiltTC*Card->SAC.Sensors[Id].FiltPrevVal;
	Card->SAC.Sensors[Id].FiltPrevVal = Card->SAC.Sensors[Id].FiltValue;
	return Card->SAC.Sensors[Id].FiltValue;
}

/**
 * @brief Chanell selection for ADC polling
 * @param Chanel Number
 * @retval ADC Measurement
 */
uint32_t ADC_SELECT(uint32_t Channel,GeneralCard *Card)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	switch (Channel)
	{
	case 0:
		sConfig.Channel = ADC_CHANNEL_0;
		sConfig.Rank = 1;
		if (HAL_ADC_ConfigChannel(Card->SAC.adc, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}
		break;
	case 1:
		sConfig.Channel = ADC_CHANNEL_1;
		sConfig.Rank = 1;
		if (HAL_ADC_ConfigChannel(Card->SAC.adc, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}
		break;
	case 2:
		sConfig.Channel = ADC_CHANNEL_2;
		sConfig.Rank = 1;
		if (HAL_ADC_ConfigChannel(Card->SAC.adc, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}
		break;
	case 3:
		sConfig.Channel = ADC_CHANNEL_3;
		sConfig.Rank = 1;
		if (HAL_ADC_ConfigChannel(Card->SAC.adc, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}
		break;
	case 4:
		sConfig.Channel = ADC_CHANNEL_4;
		sConfig.Rank = 1;
		if (HAL_ADC_ConfigChannel(Card->SAC.adc, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}
		break;
	case 5:
		sConfig.Channel = ADC_CHANNEL_5;
		sConfig.Rank = 1;
		if (HAL_ADC_ConfigChannel(Card->SAC.adc, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}
		break;
	}

	HAL_ADC_Start(Card->SAC.adc);
	HAL_ADC_PollForConversion(Card->SAC.adc,HAL_MAX_DELAY);
	uint32_t Meas;
	Meas = HAL_ADC_GetValue(Card->SAC.adc);
	HAL_ADC_Stop(Card->SAC.adc);
	return Meas;
};



/**
 * Supervision Routines
 */
void superRoutine(GeneralCard* Card)
{
	/*
	 * Static Variables
	 */
	static uint8_t OutSideSFL = 0;
	uint8_t Message[2];

	/**
	 * Get ADC
	 */
	uint32_t CH0_R = ADC_SELECT(0,Card);
	uint32_t CH1_R = ADC_SELECT(1,Card);
	uint32_t CH2_R = ADC_SELECT(2,Card);
	uint32_t CH3_R = ADC_SELECT(3,Card);
	uint32_t CH4_R = ADC_SELECT(4,Card);
	uint32_t CH5_R = ADC_SELECT(5,Card);

	/*
	 * Convert To Float Selected Channels
	 */
	Card->SAC.Sensors[0].ABS_Value = (Card->SAC.Sensors[0].ActiveFlag)?(((float)(CH0_R  - Card->SAC.Sensors[0].Offset))*Card->SAC.Sensors[0].Gain + Card->SAC.Sensors[0].Bias):0;
	Card->SAC.Sensors[1].ABS_Value = (Card->SAC.Sensors[1].ActiveFlag)?(((float)(CH1_R  - Card->SAC.Sensors[1].Offset))*Card->SAC.Sensors[1].Gain + Card->SAC.Sensors[1].Bias):0;
	Card->SAC.Sensors[2].ABS_Value = (Card->SAC.Sensors[2].ActiveFlag)?(((float)(CH2_R  - Card->SAC.Sensors[2].Offset))*Card->SAC.Sensors[2].Gain + Card->SAC.Sensors[2].Bias):0;
	Card->SAC.Sensors[3].ABS_Value = (Card->SAC.Sensors[3].ActiveFlag)?(((float)(CH3_R  - Card->SAC.Sensors[3].Offset))*Card->SAC.Sensors[3].Gain + Card->SAC.Sensors[3].Bias):0;
	Card->SAC.Sensors[4].ABS_Value = (Card->SAC.Sensors[4].ActiveFlag)?(((float)(CH4_R  - Card->SAC.Sensors[4].Offset))*Card->SAC.Sensors[4].Gain + Card->SAC.Sensors[4].Bias):0;
	Card->SAC.Sensors[5].ABS_Value = (Card->SAC.Sensors[5].ActiveFlag)?(((float)(CH5_R  - Card->SAC.Sensors[5].Offset))*Card->SAC.Sensors[5].Gain + Card->SAC.Sensors[5].Bias):0;

	/*
	 * Calculate RMS
	 */
	Card->SAC.Sensors[0].RMS_Value = (Card->SAC.Sensors[0].RMS_Flag)?rmsCalc(Card,0):0;
	Card->SAC.Sensors[1].RMS_Value = (Card->SAC.Sensors[1].RMS_Flag)?rmsCalc(Card,1):0;
	Card->SAC.Sensors[2].RMS_Value = (Card->SAC.Sensors[2].RMS_Flag)?rmsCalc(Card,2):0;
	Card->SAC.Sensors[3].RMS_Value = (Card->SAC.Sensors[3].RMS_Flag)?rmsCalc(Card,3):0;
	Card->SAC.Sensors[4].RMS_Value = (Card->SAC.Sensors[4].RMS_Flag)?rmsCalc(Card,4):0;
	Card->SAC.Sensors[5].RMS_Value = (Card->SAC.Sensors[5].RMS_Flag)?rmsCalc(Card,5):0;

	/*
	 * Calculate Filter
	 */
	Card->SAC.Sensors[0].FiltValue = (Card->SAC.Sensors[0].FilterFlag)?filtCalc(Card,0):0;
	Card->SAC.Sensors[1].FiltValue = (Card->SAC.Sensors[1].FilterFlag)?filtCalc(Card,1):0;
	Card->SAC.Sensors[2].FiltValue = (Card->SAC.Sensors[2].FilterFlag)?filtCalc(Card,2):0;
	Card->SAC.Sensors[3].FiltValue = (Card->SAC.Sensors[3].FilterFlag)?filtCalc(Card,3):0;
	Card->SAC.Sensors[4].FiltValue = (Card->SAC.Sensors[4].FilterFlag)?filtCalc(Card,4):0;
	Card->SAC.Sensors[5].FiltValue = (Card->SAC.Sensors[5].FilterFlag)?filtCalc(Card,5):0;

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
	for (int i = 0;i < 6; i++)
	{
		if (Card->SAC.Sensors[i].ActiveFlag)
		{
			if (Card->SAC.Sensors[i].ABS_Value > Card->SAC.Sensors[i].ABSUpLimit || Card->SAC.Sensors[i].ABS_Value < Card->SAC.Sensors[i].ABSLowLimit)
			{
				/*
				 * Lower safety Loop
				 */
				lowerSFL();
				/*
				 * Save Event Buffer
				 */
				saveEvent(Card,i,ABS);
				/*
				 * Communicate Failure
				 */
				uint8_t Message[6];
				union Data d;
				d.Number = Card->SAC.Sensors[i].ABS_Value;
				memcpy(Message,d.String,4);
				Message[4] = i;
				Message[5] = ABS;
				Card->Tx.DLC = 6;
				Card->Tx.IDE = CAN_ID_STD;
				Card->Tx.RTR = CAN_RTR_DATA;
				Card->Tx.StdId = idBuilder(Card, MASTER, FAIL);
				uint32_t MailBox;
				HAL_CAN_AddTxMessage(Card->CAN,&Card->Tx,Message,&MailBox);
			}
			if (!Card->SAC.Sensors[i].RMS_Flag)
				continue;
			if (Card->SAC.Sensors[i].RMS_Value > Card->SAC.Sensors[i].RMSUpLimit || Card->SAC.Sensors[i].RMS_Value < Card->SAC.Sensors[i].RMSLowLimit)
			{
				/*
				 * Lower safety Loop
				 */
				lowerSFL();
				/*
				 * Save Event Buffer
				 */
				saveEvent(Card,i,RMS);
				/*
				 * Communicate Failure
				 */
				uint8_t Message[6];
				union Data d;
				d.Number = Card->SAC.Sensors[i].RMS_Value;
				memcpy(Message,d.String,4);
				Message[4] = i;
				Message[5] = RMS;
				Card->Tx.DLC = 6;
				Card->Tx.IDE = CAN_ID_STD;
				Card->Tx.RTR = CAN_RTR_DATA;
				Card->Tx.StdId = idBuilder(Card, MASTER, FAIL);
				uint32_t MailBox;
				HAL_CAN_AddTxMessage(Card->CAN,&Card->Tx,Message,&MailBox);
			}
			if (!Card->SAC.Sensors[i].FilterFlag)
				continue;
			if (Card->SAC.Sensors[i].FiltValue > Card->SAC.Sensors[i].FiltUpLimit || Card->SAC.Sensors[i].FiltValue < Card->SAC.Sensors[i].FiltLowLimit)
			{
				/*
				 * Lower safety Loop
				 */
				lowerSFL();
				/*
				 * Save Event Buffer
				 */
				saveEvent(Card,i,FILT);
				/*
				 * Communicate Failure
				 */

				uint8_t Message[6];
				union Data d;
				d.Number = Card->SAC.Sensors[i].FiltValue;
				memcpy(Message,d.String,4);
				Message[4] = i;
				Message[5] = FILT;
				Card->Tx.DLC = 6;
				Card->Tx.IDE = CAN_ID_STD;
				Card->Tx.RTR = CAN_RTR_DATA;
				Card->Tx.StdId = idBuilder(Card, MASTER, FAIL);
				uint32_t MailBox;
				HAL_CAN_AddTxMessage(Card->CAN,&Card->Tx,Message,&MailBox);
			}
		}
	}

	/*
	 * Increase Event Buffer and Check for Record Time
	 */
	Card->SAC.SuperEvCounter++;
	Card->SAC.SuperRecCounter++;

	if (Card->SAC.SuperEvCounter >= Card->SAC.EventTime)
	{
		Card->SAC.SuperEvCounter = 0;
		Card->SAC.BufferPointer++;
		if (Card->SAC.BufferPointer > 512)
			Card->SAC.BufferPointer = 0;

		for (int i = 0,j = 0; i < 18; i+=3,j+=1)
		{
			Card->SAC.EventBuffer[Card->SAC.BufferPointer][i] = Card->SAC.Sensors[j].ABS_Value;
			Card->SAC.EventBuffer[Card->SAC.BufferPointer][i+1] = Card->SAC.Sensors[j].RMS_Value;
			Card->SAC.EventBuffer[Card->SAC.BufferPointer][i+2] = Card->SAC.Sensors[j].FiltValue;
		}
	}

	if (Card->SAC.SuperRecCounter >= Card->SAC.RecordTime)
	{
		Card->SAC.SuperRecCounter = 0;
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
	canTX(Card,Data,2,FILT);
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
