/*******************************************************************************
 * @file    calibration.cpp
 * @brief   Calibration function architecture and equations for co, co2, 
 *          temperature and relative humidity calibrations
 *
 * @author  Chiara Pesce, chiara.pesce@colorado.edu
 * @date    July 31, 2025
 * @log     Formatting & adding comments 
******************************************************************************/
#include "calibration.h"

/**************************************************************************/
 /*!
 *    @brief calls calibration functions and returns struct w/ variables
 */
/**************************************************************************/
calOutput Cal::calibrate (uint16_t co, float co2, float rh, float t) {
  calOutput out; // creates an isntance of calOutput struct
  #if CALIBRATE_CO // Conditional
    out.CO_ = calibrate_co (co, rh);
  #endif
  #if CALIBRATE_CO2 // Conditional
    out.CO2_ = calibrate_co2 (co2, rh, t);
  #endif
  #if CALIBRATE_T // Conditional
    out.T_ = calibrate_t (t);
  #endif
  #if CALIBRATE_RH // Conditional
    out.RH_ = calibrate_rh (rh);
  #endif
  return out;
}

/**************************************************************************/
 /*!
 *    @brief  Stores the CO calibration equations
 */
/**************************************************************************/
int Cal::calibrate_co (uint16_t co, float rh) {
  int co_cal; // Stores calibrated CO value
  switch (calID_letter) { // Switch statememnt for letter in ypodID
    case 'U':
      switch (calID_number) { // Switch statement for number in ypodID
        case '4':
          co_cal = ((0.00109 * co) + (-0.12464 * rh) + 4.71174) * 1000;
          break;
        case '8':
          co_cal = ((0.00110 * co) + (-0.12029 * rh) + 4.04736) * 1000;
          break;
        case '7':
          co_cal = ((0.00113 * co) + (-0.11509 * rh) + 4.27234) * 1000;
          break;
        case '2':
          co_cal = ((0.00117 * co) + (-0.12668 * rh) + 5.09077) * 1000;
          break;
        case '1':
          co_cal = ((0.00107 * co) + (-0.12473 * rh) + 4.53371) * 1000;
          break;
        default:
          co_cal = co; // Default = original signal
      }
      break;
    case 'X':
      switch (calID_number) { // Switch statement for number in ypodID
        case '0':
          co_cal = ((0.00108 * co) + (-0.12966 * rh) + 4.89108) * 1000;
          break;
        default:
          co_cal = co; // Default = original signal
      }
      break;
    case 'F':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2':
          co_cal = ((0.00107 * co) + (-0.14491 * rh) + 5.75429) * 1000;
          break;
        case '4':
          co_cal = ((0.00117 * co) + (-0.13572 * rh) + 5.46203) * 1000;
          break;
        default:
          co_cal = co; // Default = original signal
      }
      break;
    case 'D':
      switch (calID_number) { // Switch statement for number in ypodID
        case '4':
          co_cal = ((0.00102 * co) + (-0.16957 * rh) + 5.30658) * 1000;
          break;
        default:
          co_cal = co; // Default = original signal
      }
      break;  
    case 'Z':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2':
          co_cal = ((0.00123 * co) + (-0.13601 * rh) + 4.78696) * 1000;
          break;
        case '3': 
          co_cal = ((0.00114 * co) + (-0.12002 * rh) + 4.42928) * 1000;
          break;
        default:
          co_cal = co; // Default = original signal
      }
      break;
    case 'A':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1':
          co_cal = ((0.00104 * co) + (-0.12167 * rh) + 4.43994) * 1000;
          break;
        default:
          co_cal = co; // Default = original signal
      }
      break;
    case 'T':
      switch (calID_number) { // Switch statement for number in ypodID
        case '4':
          co_cal = ((0.00105 * co) + (-0.14761 * rh) + 4.78947) * 1000;
          break;
        default:
          co_cal = co; // Default = original signal
      }
      break;
    case 'O':
      switch (calID_number){ // Switch statement for number in ypodID
        case '1':
          co_cal = ((0.00107 * co) + (-0.11883 * rh) + 4.46679) * 1000;
          break;
        default:
          co_cal = co; // Default = original signal
      }
      break;
    case 'L':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1':
          co_cal = ((0.00107 * co) + (-0.11803 * rh) + 3.62155) * 1000;
          break;
        default:
          co_cal = co; // Default = original signal
      }
      break;
    case 'B':
      switch (calID_number) { // Switch statement for number in ypodID
        case '8':
          co_cal = ((0.00112 * co) + (-0.12563 * rh) + 4.56465) * 1000;
          break;
        default:
          co_cal = co; // Default = original signal
      }
      break;
    case 'G':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2':
          co_cal = ((0.00115 * co) + (-0.11353 * rh) + 3.00400) * 1000;
          break;
        default:
          co_cal = co; // Default = original signal
      }
      break;
    default:
      Serial.println("No CO calibration data for this pod.");
      co_cal = co; // Default = original signal 
  }
  if (co_cal < 0) { // conditional for negative values
    co_cal = 0;
  }
  return co_cal;
}

/**************************************************************************/
 /*!
 *    @brief  Stores the CO2 calibration equations
 */
/**************************************************************************/
int Cal::calibrate_co2 (float co2, float rh, float t) {
  int co2_cal; // Stores calibrated CO2 value
  switch (calID_letter) { // Switch statememnt for letter in ypodID
    case 'U':
      switch (calID_number) { // Switch statement for number in ypodID
        case '4':
          co2_cal = (1.16717 * co2) + (0.20373 * rh) + (0.01642 * t) - 82.95474;
          break;
        case '8':
          co2_cal = (1.16663 * co2) + (-0.64001 * rh) + (-0.58624 * t) - 31.31421;
          break;
        case '7':
          co2_cal = (1.17296 * co2) + (-0.01352 * rh) + (0.98768 * t) - 132.54824;
          break;
        case '2':
          co2_cal = (1.26202 * co2) + (0.32735 * rh) + (-0.82488 * t) + 87.94083;
          break;
        case '6':
          co2_cal = (1.04357 * co2) + (-1.31559 * rh) + (-0.68401 * t) - 72.63513;
          break;
        default:
          co2_cal = co2; // Default = original signal
      }
      break;
    case 'V':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1':
          co2_cal = (1.18498 * co2) + (-0.09612 * rh) + (1.94091 * t) - 267.48365;
          break;
        default:
          co2_cal = co2; // Default = original signal
      }
      break;
    case 'X':
      switch (calID_number) { // Switch statement for number in ypodID
        case '0':
          co2_cal = (1.03612 * co2) + (1.17989 * rh) + (0.85961 * t) - 186.12921;
          break;
        default:
          co2_cal = co2; // Default = original signal
      }
      break;
    case 'F':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2':
          co2_cal = (1.22328 * co2) + (0.16343 * rh) + (-1.40132 * t) - 15.57537;
          break;
        case '4':
          co2_cal = (1.09864 * co2) + (0.57402 * rh) + (0.46539 * t) - 89.00339;
          break;
        default:
          co2_cal = co2; // Default = original signal
      }
      break;
    case 'D':
      switch (calID_number) { // Switch statement for number in ypodID
        case '4':
          co2_cal = (1.09476 * co2) + (-1.89513 * rh) + (-5.76502 * t) - 4.97449;
          break;
        default:
          co2_cal = co2; // Default = original signal
      } 
      break; 
    case 'Z':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2':
          co2_cal = (1.03796 * co2) + (-0.77552 * rh) + (-0.36187 * t) - 134.25129;
          break;
        case '3': 
          co2_cal = (1.22535 * co2) + (-0.05251 * rh) + (1.22153 * t) - 64.50792;
          break;
        default:
          co2_cal = co2; // Default = original signal
      }
      break;
    case 'A':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1':
          co2_cal = (1.12864 * co2) + (0.02037 * rh) + (-0.78409 * t) - 110.70007;
          break;
        default:
          co2_cal = co2; // Default = original signal
      }
      break;
    case 'T':
      switch (calID_number) { // Switch statement for number in ypodID
        case '4':
          co2_cal = (1.20414 * co2) + (-0.54367 * rh) + (-1.09033 * t) - 71.13862;
          break;
        case '2':
          co2_cal = (1.36979 * co2) + (-1.73881 * rh) + (4.56957 * t) - 390.66394;
          break;
        default:
          co2_cal = co2; // Default = original signal
      }
      break;
    case 'O':
      switch (calID_number){ // Switch statement for number in ypodID
        case '1':
          co2_cal = (1.16921 * co2) + (0.60567 * rh) + (1.10530 * t) - 191.66687;
          break;
        default:
          co2_cal = co2; // Default = original signal
      }
      break;
    case 'K':
      switch (calID_number) { // Switch statement for number in ypodID
        case '3':
          co2_cal = (1.09907 * co2) + (-0.14214 * rh) + (-1.74229 * t) + 98.61317;
          break;
        default:
          co2_cal = co2; // Default = original signal
      }
      break;
    case 'B':
      switch (calID_number) { // Switch statement for number in ypodID
        case '8':
          co2_cal = (1.29633 * co2) + (-1.04402 * rh) + (-3.17786 * t) + 266.75475;
          break;
        default:
          co2_cal = co2; // Default = original signal
      }
      break;
    case 'G':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2':
          co2_cal = (1.11746 * co2) + (0.14695 * rh) + (0.74198 * t) - 272.54046;
          break;
        default:
          co2_cal = co2; // Default = original signal
      }
      break;
    default:
      Serial.println("No CO2 calibration data for this pod.");
      co2_cal = co2; // Default = original signal 
  }
  return co2_cal;
}

/**************************************************************************/
 /*!
 *    @brief  Stores the temperature calibration equations
 */
/**************************************************************************/
float Cal::calibrate_t (float t) {
  float t_cal; // Stores calibrated temperature value
  switch (calID_letter) { // Switch statememnt for letter in ypodID
    case 'U':
      switch (calID_number) { // Switch statement for number in ypodID
        case '4':
          t_cal = (1.11116 * t) -5.52967;
          break;
        case '8':
          t_cal = (1.05981 * t) -4.09106;
          break;
        case '7':
          t_cal = (1.04182 * t) -2.74393;
          break;
        case '2':
          t_cal = (1.06117 * t) -3.69364;
          break;
        case '6':
          t_cal = (1.05834 * t) -2.55614;
          break;
        case '1':
          t_cal = (1.07405 * t) -4.30518;
          break;
        default:
          t_cal = t; // Default = original signal
      }
      break;
    case 'V':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1':
          t_cal = (1.03286 * t) -1.94146;
          break;
        default:
          t_cal = t; // Default = original signal
      }
      break;
    case 'X':
      switch (calID_number) { // Switch statement for number in ypodID
        case '0':
          t_cal = (1.09128 * t) -5.91627;
          break;
        default:
          t_cal = t; // Default = original signal
      }
      break;
    case 'F':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2':
          t_cal = (1.10493 * t) -5.64845;
          break;
        case '4':
          t_cal = (1.07020 * t) -4.39655;
          break;
        default:
          t_cal = t; // Default = original signal
      }
      break;
    case 'D':
      switch (calID_number) { // Switch statement for number in ypodID
        case '4':
          t_cal = (1.01657 * t) -3.23499;
          break;
        default:
          t_cal = t; // Default = original signal
      }  
      break;
    case 'Z':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2':
          t_cal = (1.09779 * t) -5.73499;
          break;
        case '3': 
          t_cal = (1.08138 * t) -4.36113;
          break;
        default:
          t_cal = t; // Default = original signal
      }
      break;
    case 'A':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1':
          t_cal = (1.05173 * t) -2.83754;
          break;
        default:
          t_cal = t; // Default = original signal
      }
      break;
    case 'T':
      switch (calID_number) { // Switch statement for number in ypodID
        case '4':
          t_cal = (1.05173 * t) -2.83754;
          break;
        case '2':
          t_cal = (1.00995 * t) -2.13822;
          break;
        default:
          t_cal = t; // Default = original signal
      }
      break;
    case 'O':
      switch (calID_number){ // Switch statement for number in ypodID
        case '1':
          t_cal = (1.08049 * t) -4.03947; 
          break;
        default:
          t_cal = t; // Default = original signal
      }
      break;
    case 'K':
      switch (calID_number) { // Switch statement for number in ypodID
        case '3':
          t_cal = (1.10950 * t) -5.75163;
          break;
        default:
          t_cal = t; // Default = original signal
      }
      break;
    case 'L':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1':
          t_cal = (1.09721 * t) -5.19718;
          break;
        default:
          t_cal = t; // Default = original signal
      }
      break;
    case 'B':
      switch (calID_number) { // Switch statement for number in ypodID
        case '8':
          t_cal = (1.05199 * t) -4.55966;
          break;
        default:
          t_cal = t; // Default = original signal
      }
      break;
    case 'G':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2':
          t_cal = (1.10837 * t) -5.12265;
          break;
        default:
          t_cal = t; // Default = original signal
      }
      break;
    default:
      Serial.println("No temperature calibration data for this pod.");
      t_cal = t; // Default = original signal
  }
  return t_cal;
}

/**************************************************************************/
 /*!
 *    @brief  Stores the relative humidity calibration equations
 */
/**************************************************************************/
float Cal::calibrate_rh (float rh) {
  float rh_cal; // Stores calibrated relative humidity value
  switch (calID_letter) { // Switch statememnt for letter in ypodID
    case 'U':
      switch (calID_number) { // Switch statement for number in ypodID
        case '4':
          rh_cal = (1.18899 * rh) -7.94909;
          break;
        case '8':
          rh_cal = (1.15852 * rh) -2.94663;
          break;
        case '7':
          rh_cal = (1.08633 * rh) -4.56042;
          break;
        case '2':
          rh_cal = (1.18720 * rh) -11.41265;
          break;
        case '6':
          rh_cal = (1.04647 * rh) -1.98453;
          break;
        case '1':
          rh_cal = (1.16004 * rh) -5.86517;
          break;
        default:
          rh_cal = rh; // Default = original signal
      }
      break;
    case 'V':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1':
          rh_cal = (1.05049 * rh) -2.28806;
          break;
        default:
          rh_cal = rh; // Default = original signal
      }
      break;
    case 'X':
      switch (calID_number) { // Switch statement for number in ypodID
        case '0':
          rh_cal = (1.29066 * rh) -12.22035;
          break;
        default:
          rh_cal = rh; // Default = original signal
      }
      break;
    case 'F':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2':
          rh_cal = (1.33239 * rh) -17.35959;
          break;
        case '4':
          rh_cal = (1.24322 * rh) -12.37479;
          break;
        default:
          rh_cal = rh; // Default = original signal
      }
      break;
    case 'D':
      switch (calID_number) { // Switch statement for number in ypodID
        case '4':
          rh_cal = (1.13629 * rh) -3.32180;
          break;
        default:
          rh_cal = rh; // Default = original signal
      }
      break;  
    case 'Z':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2':
          rh_cal = (1.25800 * rh) -5.96184;
          break;
        case '3': 
          rh_cal = (1.18946 * rh) -8.40903;
          break;
        default:
          rh_cal = rh; // Default = original signal
      }
      break;
    case 'A':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1':
          rh_cal = (1.15608 * rh) -5.57459;
          break;
        default:
          rh_cal = rh; // Default = original signal
      }
      break;
    case 'T':
      switch (calID_number) { // Switch statement for number in ypodID
        case '4':
          rh_cal = (1.06968 * rh) -2.61488;
          break;
        case '2':
          rh_cal = (1.06249 * rh) -3.53605;
          break;
        default:
          rh_cal = rh; // Default = original signal
      }
      break;
    case 'O':
      switch (calID_number){ // Switch statement for number in ypodID
        case '1':
          rh_cal = (1.14766 * rh) -6.53990;
          break;
        default:
          rh_cal = rh; // Default = original signal
      }
      break;
    case 'K':
      switch (calID_number) { // Switch statement for number in ypodID
        case '3':
          rh_cal = (1.20688 * rh) -6.58213;
          break;
        default:
          rh_cal = rh; // Default = original signal
      }
      break;
    case 'L':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1':
          rh_cal = (1.20574 * rh) -4.02710;
          break;
        default:
          rh_cal = rh; // Default = original signal
      }
      break;
    case 'B':
      switch (calID_number) { // Switch statement for number in ypodID
        case '8':
          rh_cal = (1.22586 * rh) -8.15827;
          break;
        default:
          rh_cal = rh; // Default = original signal
      }
      break;
    case 'G':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2':
          rh_cal = (1.13665 * rh) -5.63727;
          break;
        default:
          rh_cal = rh; // Default = original signal
      }
      break;
    default:
      Serial.println("No relative humidity calibration data for this pod.");
      rh_cal = rh; // Default = original signal
  }
  return rh_cal;
}
