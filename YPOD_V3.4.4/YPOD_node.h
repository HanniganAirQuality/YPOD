/*******************************************************************************
 * @file    YPOD_node.h
 * @brief   Splits ADS1115 code from .ino 
 *
 * @author  Percy Smith, percy.smith@colorado.edu
 * @date    July 4, 2024
 * @log     Formatting & adding comments 
 * @TBD     Add a read function returning heaters structure
******************************************************************************/
#ifndef _YPOD_NODE_H
#define _YPOD_NODE_H

#include <Arduino.h>

#define SERIAL_ENABLED        1
#define PMS_ENABLED           1
#define QUAD_ENABLED          0

#define INCLUDE_HEATERS       0
#define INCLUDE_STANDARD      0
#define INCLUDE_PARTICLES     0

#define BME180      1
#define SHT25       1
#define BME680      0

const int PM_RX = 2;
const int PM_TX = 3;
#define G_LED     10

//SD Card Settings
#define SD_CS         4

const char ypodID[] = "YPODM6";

#endif // _YPOD_NODE_H
