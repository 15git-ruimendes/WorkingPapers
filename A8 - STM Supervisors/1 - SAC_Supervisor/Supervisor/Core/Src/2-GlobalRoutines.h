#ifndef __GLOBAL_ROUTS_H_
#define __GLOBAL_ROUTS_H_

#include "1-ConfigurationParser.h"
#include "main.h"
#include "math.h"

/**
 * CAN Router
 */
uint8_t 	idParser(GeneralCard* Card, uint32_t Id);
uint32_t 	idBuilder(GeneralCard* Card, uint8_t Dest, uint8_t Function);
void 		canParser(GeneralCard* Card, uint32_t DLC,CAN_RxHeaderTypeDef* RxHeader,uint8_t *Data);
void		canTX(GeneralCard* Card,uint8_t* Data,uint8_t DLC,uint8_t Function);
/**
 * Supervision Routine
 */
void 		superRoutine(GeneralCard* SAC);

/**
 * Start Routine
 */
void 		startRoutine(GeneralCard* SAC);

/**
 * Stop Routine
 */
void 		stopRoutine(GeneralCard* SAC);

/**
 * Recall Routine
 */
void 		recalRoutine(GeneralCard* SAC);



#endif
