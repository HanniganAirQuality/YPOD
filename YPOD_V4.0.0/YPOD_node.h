/*******************************************************************************
 * @file    YPOD_node.h
 * @brief   Adds calibration functions for CO, CO2, temperature and RH
 *
 * @author  Percy Smith, percy.smith@colorado.edu
 * @author  Chiara Pesce, chiara.pesce@colorado.edu
 * @date    July 31, 2025
 * 
 * FOR AQIQ RE-UPLOAD PMS_ENABLED = 1, BME180 = 0, SHT25 = 1 & MISC2611 = 1
******************************************************************************/
#ifndef _YPOD_NODE_H
#define _YPOD_NODE_H

#include <Arduino.h>

#define CALIBRATE    1 // Embedded calibration

#define SERIAL_ENABLED        1
#define PMS_ENABLED           1
#define QUAD_ENABLED          0 

#define RTC_UPDATE            1 // IF you have to update RTC, please upload after with a 0

#define INCLUDE_HEATERS       0
#define INCLUDE_STANDARD      0
#define INCLUDE_PARTICLES     0

#define BME180      0
#define SHT25       1
// #define BME680      0 //NOT WRITTEN
#define MISC2611    1 // Ozone sensor

const int PM_RX = 2;
const int PM_TX = 3;
#define G_LED     10

// SD Card Settings
const int SD_CS = 4;

const char ypodID[] = "YPODV2";

#if CALIBRATE
  const char calID_letter = ypodID[4]; // Letter for calID
  const char calID_number = ypodID[5]; // Number for calID
#endif

#endif // _YPOD_NODE_H
