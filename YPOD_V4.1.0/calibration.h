/*******************************************************************************
 * @file    calibration.h
 * @brief   header file for calibration.cpp, stores struct and class.
 *          Conditionals for which variables to run calibration on  
 *
 * @author  Chiara Pesxe, chiara.pesce@colorado.edu
 * @date    September 30, 2025
 * @log     Adding VOC Calibration and negative value handling 
******************************************************************************/
#ifndef _CALIBRATION_H
#define _CALIBRATION_H

#define CALIBRATE_CO    1 // Calibrates CO sensor
#define CALIBRATE_CO2   1 // Calibrates CO2 sensor
#define CALIBRATE_T     1 // Calibrates temperature sensor
#define CALIBRATE_RH    1 // Calibrates relative humidity sensor
#define CALIBRATE_VOC   1 // Calibrates VOC sensors 

#include "YPOD_node.h" // include statement 

struct calOutput { // sruct for which var to calibrate
  int CO_;
  int CO2_;
  float T_;
  float RH_;
  int TVOC_;
};

class Cal {
  public: // public variables
    calOutput calibrate (uint16_t co, float co2, float rh, float t, uint16_t fig2600, uint16_t fig2602);
    
  private: // private variables
    int calibrate_co (uint16_t co, float rh); 
    int calibrate_co2 (float co2, float rh, float t);
    float calibrate_t (float t);
    float calibrate_rh (float rh);
    int calibrate_voc (uint16_t fig2600, uint16_t fig2602, float rh, float t);
};

#endif // _CALIBRATION_H


