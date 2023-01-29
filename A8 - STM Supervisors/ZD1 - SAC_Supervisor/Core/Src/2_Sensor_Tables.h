/*
 * Sensor_Tables.h
 *
 *  Created on: Nov 23, 2022
 *      Author: RuiMendes
 */

#ifndef SRC_SENSOR_TABLES_H_
#define SRC_SENSOR_TABLES_H_

float SENSOR_DEFINES[10][3] =
{
		{0.04080000,-84.11100000,0.00000000},	// LEM
		{0.04080000,-61.00999900,0.00000000},	// VSB3000
		{0.04080000,-61.00999900,0.00000000},
		{0.04080000,-84.11100000,0.00000000},
		{0.04080000,-61.00999900,0.00000000},
		{0.04080000,-84.11100000,0.00000000},
		{0.04080000,-84.11100000,0.00000000},
		{0.04080000,-84.11100000,0.00000000},
		{0.04080000,-84.11100000,0.00000000},
		{0.04080000,-84.11100000,0.00000000},
};

#define MILI	0x1
#define CENTI	0x2
#define DECI	0x3
#define UNI		0x4
#define DEC		0x5
#define CENT	0x6
#define KILO	0x7

#endif /* SRC_SENSOR_TABLES_H_ */