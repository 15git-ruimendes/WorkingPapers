/*
 * routines.h
 *
 *  Created on: Nov 17, 2022
 *      Author: ruimendes
 */

#include "main.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "1_CAN_Tables.h"
#include "2_Sensor_Tables.h"




/*
 * Global Variables and functions
 */

#define CALIBRATION_MODE	0x10
#define NORMAL_MODE			0x01

/**
 * @brief Chanell selection for ADC polling
 * @param Chanel Number
 * @retval ADC Measurement
 */
uint32_t ADC_SELECT(uint32_t Channel,TypedefSAC *SAC_Card)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	switch (Channel)
	{
	case 0:
		sConfig.Channel = ADC_CHANNEL_0;
		sConfig.Rank = 1;
		if (HAL_ADC_ConfigChannel(SAC_Card->hadc, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}
		break;
	case 1:
		sConfig.Channel = ADC_CHANNEL_1;
		sConfig.Rank = 1;
		if (HAL_ADC_ConfigChannel(SAC_Card->hadc, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}
		break;
	case 2:
		sConfig.Channel = ADC_CHANNEL_2;
		sConfig.Rank = 1;
		if (HAL_ADC_ConfigChannel(SAC_Card->hadc, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}
		break;
	case 3:
		sConfig.Channel = ADC_CHANNEL_3;
		sConfig.Rank = 1;
		if (HAL_ADC_ConfigChannel(SAC_Card->hadc, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}
		break;
	case 4:
		sConfig.Channel = ADC_CHANNEL_4;
		sConfig.Rank = 1;
		if (HAL_ADC_ConfigChannel(SAC_Card->hadc, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}
		break;
	case 5:
		sConfig.Channel = ADC_CHANNEL_5;
		sConfig.Rank = 1;
		if (HAL_ADC_ConfigChannel(SAC_Card->hadc, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}
		break;
	}

	HAL_ADC_Start(SAC_Card->hadc);
	HAL_ADC_PollForConversion(SAC_Card->hadc,HAL_MAX_DELAY);
	uint32_t Meas;
	Meas = HAL_ADC_GetValue(SAC_Card->hadc);
	HAL_ADC_Stop(SAC_Card->hadc);
	return Meas;
};

/**
 * @brief  Card Activation Routine
 * @note   Pass from idle to active (either calibration or start)
 * @param  *SAC_Card: 
 * @retval None
 */
void activRoutine(TypedefSAC *SAC_Card)
{

	// Check card status
	if (SAC_Card->Card.State != 0)
		return;

	// Read Safety Line Pins
	uint8_t High_StatusP = HAL_GPIO_ReadPin(SFLH_In_GPIO_Port,SFLH_In_Pin);
	uint8_t Low_StatusP = HAL_GPIO_ReadPin(SFLL_In_GPIO_Port,SFLL_In_Pin);
	// Toggle Safety Line Pins
	HAL_GPIO_WritePin(SFLH_Out_GPIO_Port,SFLH_Out_Pin,SET);
	HAL_GPIO_WritePin(SFLL_Out_GPIO_Port,SFLL_Out_Pin,SET);

	// Read Safety Line Pins
	uint8_t High_StatusA = HAL_GPIO_ReadPin(SFLH_In_GPIO_Port,SFLH_In_Pin);
	uint8_t Low_StatusA = HAL_GPIO_ReadPin(SFLL_In_GPIO_Port,SFLL_In_Pin);

	// Toggle Safety Line Pins
	HAL_GPIO_WritePin(SFLH_Out_GPIO_Port,SFLH_Out_Pin,RESET);
	HAL_GPIO_WritePin(SFLL_Out_GPIO_Port,SFLL_Out_Pin,RESET);

	// Read Safety Line Pins
	uint8_t High_StatusAA = HAL_GPIO_ReadPin(SFLH_In_GPIO_Port,SFLH_In_Pin);
	uint8_t Low_StatusAA = HAL_GPIO_ReadPin(SFLL_In_GPIO_Port,SFLL_In_Pin);

	// Failure Message
	uint8_t FailType;

	if (!High_StatusP || !High_StatusAA || High_StatusA)
	{
		FailType = ACTIVE_H_FAIL;
	}
	if (Low_StatusP || Low_StatusAA || !Low_StatusA)
	{
		FailType = ACTIVE_L_FAIL;
	}

	if (FailType != 0)
	{
		stopRoutine(SAC_Card); // Return to Idle
		return;
	}


	// Check activation mode
	// Calibration
	if (SAC_Card->Card.RxData[0] == CALIBRATION_MODE)
	{
		SAC_Card->Card.State = 100;
		SAC_Card->Card.SuperState = 100;
		return;
	}
	// Normal
	if (SAC_Card->Card.RxData[8] == NORMAL_MODE)
	{
		SAC_Card->Card.State = 1;
		SAC_Card->Card.SuperState = 0;
		TxParser(SAC_Card,ACTIVE_OK);
	}


};

/**
 * @brief  Initialisation Routine
 * @note   
 * @param  *SAC_Card: 
 * @param  Message: 
 * @param  DataLen: 
 * @param  ComandLen: 
 * @param  MessgCount: 
 * @retval None
 */
void initRoutine(TypedefSAC *SAC_Card,uint8_t* Message,uint8_t DataLen,uint8_t ComandLen,uint8_t MessgCount)
{
	// Check if message correct
	if (SAC_Card->Card.State != 1 || DataLen != 8)
	{
		stopRoutine(SAC_Card,INIT_FAIL);
		return;
	}

	// Resolve messages to initial setup
	uint8_t SensorId = 	Message[3];
	uint8_t ScaleU = 	Message[4] & 0x0F;
	uint8_t SignalU = 	Message[4] >> 4;
	float MultiplierU = 0;

	uint8_t ScaleD = 	Message[6] & 0x0F;
	uint8_t SignalD = 	Message[6] >> 4;
	float MultiplierD = 0;

	// Scale Switch Case
	switch (ScaleU)
	{
	case(MILI):
		MultiplierU = 0.001;
	case(CENTI):
		MultiplierU = 0.01;
	case(DECI):
		MultiplierU = 0.1;
	case(UNI):
		MultiplierU = 1;
	case(DEC):
		MultiplierU = 10;
	case(CENT):
		MultiplierU = 100;
	case(KILO):
		MultiplierU = 1000;
	}

	switch (ScaleD)
	{
	case(MILI):
		MultiplierD = 0.001;
	case(CENTI):
		MultiplierD = 0.01;
	case(DECI):
		MultiplierD = 0.1;
	case(UNI):
		MultiplierD = 1;
	case(DEC):
		MultiplierD = 10;
	case(CENT):
		MultiplierD = 100;
	case(KILO):
		MultiplierD = 1000;
	}

	// Signal Decoder
	float SignU = (SignalU == 0)?1:-1;
	float SignD = (SignalD == 0)?1:-1;

	// Sensor definitions
	SAC_Card->SensGains[MessgCount - 1] 	= SENSOR_DEFINES[SensorId][0];
	SAC_Card->SensOffset[MessgCount - 1] 	= SENSOR_DEFINES[SensorId][1];
	SAC_Card->SensBias[MessgCount - 1] 		= SENSOR_DEFINES[SensorId][2];

	// Limits definitions
	SAC_Card->SensLimitsU[MessgCount -1] = MultiplierU * SignU * Message[5];
	SAC_Card->SensLimitsD[MessgCount -1] = MultiplierD * SignD * Message[7];

	// State Update
	if (MessgCount == ComandLen)
	{
		SAC_Card->Card.State = 2;
		SAC_Card->Card.SuperState = 0;
		TxParser(SAC_Card,INIT_OK);
	}
	else
	{
		TxParser(SAC_Card,INIT_P_OK);
	}
};

/**
 * @brief  Start Command Routine
 * @note   
 * @param  *SAC_Card: 
 * @param  Buffer: 
 * @retval None
 */
void startRoutine(TypedefSAC *SAC_Card,uint8_t* Buffer)
{
	// Check if status is correct 
	if (SAC_Card->Card.State != 2)
	{
		stopRoutine(SAC_Card,START_FAIL);
		return;
	}
	
	// Nothing to do with buffer message

	// Start super routine (enable interrupt routine)
	if (HAL_TIM_Base_Start_IT(SAC_Card->Card.htim) != HAL_OK)
	{
		stopRoutine(SAC_Card,INTERRUPT_FAIL);
		return;
	}
	
	// Return Successful Start
	SAC_Card->Card.State = 3;
	SAC_Card->Card.SuperState = 1;
	TxParser(SAC_Card,START_OK);
};

/**
 * @brief  
 * @note   
 * @param  *SAC_Card: 
 * @retval None
 */
void superRoutine(TypedefSAC *SAC_Card)
{
	// Read ADC Values
	uint32_t CH0_R = ADC_SELECT(0,SAC_Card);
	uint32_t CH1_R = ADC_SELECT(1,SAC_Card);
	uint32_t CH2_R = ADC_SELECT(2,SAC_Card);
	uint32_t CH3_R = ADC_SELECT(3,SAC_Card);
	uint32_t CH4_R = ADC_SELECT(4,SAC_Card);
	uint32_t CH5_R = ADC_SELECT(5,SAC_Card);
	
	// Convert to Float values
	float CH0_F = ((float)(CH0_R  - SAC_Card->SensOffset[0]))*SAC_Card->SensGains[0] + SAC_Card->SensBias[0];
	float CH1_F = ((float)(CH1_R  - SAC_Card->SensOffset[1]))*SAC_Card->SensGains[1] + SAC_Card->SensBias[1];
	float CH2_F = ((float)(CH2_R  - SAC_Card->SensOffset[2]))*SAC_Card->SensGains[2] + SAC_Card->SensBias[2];
	float CH3_F = ((float)(CH3_R  - SAC_Card->SensOffset[3]))*SAC_Card->SensGains[3] + SAC_Card->SensBias[3];
	float CH4_F = ((float)(CH4_R  - SAC_Card->SensOffset[4]))*SAC_Card->SensGains[4] + SAC_Card->SensBias[4];
	float CH5_F = ((float)(CH5_R  - SAC_Card->SensOffset[5]))*SAC_Card->SensGains[5] + SAC_Card->SensBias[5];

	SAC_Card->SensValues[0] = CH0_F;
	SAC_Card->SensValues[1] = CH1_F;
	SAC_Card->SensValues[2] = CH2_F;
	SAC_Card->SensValues[3] = CH3_F;
	SAC_Card->SensValues[4] = CH4_F;
	SAC_Card->SensValues[5] = CH5_F;

	// SAC Finite State Machine
	for (int i = 0; i < 6 && SAC_Card->Card.State != FAILURE; i++)
	{
		if (SAC_Card->SensValues[i] > SAC_Card->SensLimitsU[i])
		{
			HAL_GPIO_WritePin(SFLH_Out_GPIO_Port,SFLH_Out_Pin,SET);
			HAL_GPIO_WritePin(SFLL_Out_GPIO_Port,SFLL_Out_Pin,SET);
			SAC_Card->Card.State = FAILURE;
			SAC_Card->Card.SuperState = i*10;
			SAC_Card->SensStates[i] = 10;
			stopRoutine(SAC_Card,FAILURE);
		}
		if (SAC_Card->SensValues[i] < SAC_Card->SensLimitsD[i])
		{
			HAL_GPIO_WritePin(SFLH_Out_GPIO_Port,SFLH_Out_Pin,SET);
			HAL_GPIO_WritePin(SFLL_Out_GPIO_Port,SFLL_Out_Pin,SET);
			SAC_Card->Card.State = FAILURE;
			SAC_Card->Card.SuperState = i*11;
			SAC_Card->SensStates[i] = 10;
			stopRoutine(SAC_Card,FAILURE);
		}
	}
	
};

/**
 * @brief  
 * @note   
 * @param  *SAC_Card: 
 * @param  typeShutdown: 
 * @retval None
 */
void stopRoutine(TypedefSAC *SAC_Card, uint8_t typeShutdown)
{
	// Failure Mode Switch Case
	switch (typeShutdown)
	{
	case ACTIVE_H_FAIL:
		memcpy(SAC_Card->Card.TxData,{MASTER_CARD,ERROR_TX},2);
		HAL_CAN_AddTxMessage(SAC_Card->Card.CANChannel,&SAC_Card->Card.txHeader, SAC_Card->Card.TxData, &SAC_Card->Card.TxMailbox);
	case ACTIVE_L_FAIL:
		memcpy(SAC_Card->Card.TxData,[MASTER_CARD,ERROR_TX],2);
		HAL_CAN_AddTxMessage(SAC_Card->Card.CANChannel,&SAC_Card->Card.txHeader, SAC_Card->Card.TxData, &SAC_Card->Card.TxMailbox);
	case INIT_FAIL:
		memcpy(SAC_Card->Card.TxData,[MASTER_CARD,ERROR_TX],2);
		HAL_CAN_AddTxMessage(SAC_Card->Card.CANChannel,&SAC_Card->Card.txHeader, SAC_Card->Card.TxData, &SAC_Card->Card.TxMailbox);
	case START_FAIL:
		memcpy(SAC_Card->Card.TxData,[MASTER_CARD,ERROR_TX],2);
		HAL_CAN_AddTxMessage(SAC_Card->Card.CANChannel,&SAC_Card->Card.txHeader, SAC_Card->Card.TxData, &SAC_Card->Card.TxMailbox);
	case INTERRUPT_FAIL:
		memcpy(SAC_Card->Card.TxData,[MASTER_CARD,ERROR_TX],2);
		HAL_CAN_AddTxMessage(SAC_Card->Card.CANChannel,&SAC_Card->Card.txHeader, SAC_Card->Card.TxData, &SAC_Card->Card.TxMailbox);
	case FAILURE:
		memcpy(SAC_Card->Card.TxData,[MASTER_CARD,ERROR_TX],2);
		HAL_CAN_AddTxMessage(SAC_Card->Card.CANChannel,&SAC_Card->Card.txHeader, SAC_Card->Card.TxData, &SAC_Card->Card.TxMailbox);
	case EMERGENCY:
		// Do nothing
	case STOP:
		// Do nothing
	}
};

/**
 * @brief  
 * @note   
 * @param  *SAC_Card: 
 * @retval None
 */
void recValues(TypedefSAC *SAC_Card)
{

};

/**
 * @brief  
 * @note   
 * @param  *SAC_Card: 
 * @retval None
 */
void recEvents(TypedefSAC *SAC_Card)
{

};

/**
 * @brief  
 * @note   
 * @param  *SAC_Card: 
 * @retval None
 */
void recStatus(TypedefSAC *SAC_Card)
{

};


