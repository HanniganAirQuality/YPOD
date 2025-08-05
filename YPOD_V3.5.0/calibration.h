/*******************************************************************************
 * @file    calibration.h
 * @brief   header file for calibration.cpp, stores struct and class.
 *          Conditionals for which variables to run calibration on  
 *
 * @author  Chiara Pesxe, chiara.pesce@colorado.edu
 * @date    July 29, 2025
 * @log     Formatting & adding comments 
******************************************************************************/
#ifndef _CALIBRATION_H
#define _CALIBRATION_H

#define CALIBRATE_CO    1 // Calibrates CO sensor
#define CALIBRATE_CO2   1 // Calibrates CO2 sensor
#define CALIBRATE_T     1 // Calibrates temperature sensor
#define CALIBRATE_RH    1 // Calibrates relative humidity sensor

#include "YPOD_node.h" // include statement 

struct calOutput { // sruct for which var to calibrate
  int CO_;
  int CO2_;
  float T_;
  float RH_;
};

class Cal {
  public: // public variables
    calOutput calibrate (uint16_t co, float co2, float rh, float t);
    
  private: // private variables
    int calibrate_co (uint16_t co, float rh); 
    int calibrate_co2 (float co2, float rh, float t);
    float calibrate_t (float t);
    float calibrate_rh (float rh);
};

#endif // _CALIBRATION_H


