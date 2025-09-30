/*******************************************************************************
 * @file    calibration.cpp
 * @brief   Calibration function architecture and equations for co, co2, 
 *          temperature and relative humidity calibrations
 *
 * @author  Chiara Pesce, chiara.pesce@colorado.edu
 * @date    September 30, 2025
 * @log     Adding VOC Calibration and negative value handling
******************************************************************************/
#include "calibration.h"

/**************************************************************************/
 /*!
 *    @brief calls calibration functions and returns struct w/ variables
 */
/**************************************************************************/
calOutput Cal::calibrate (uint16_t co, float co2, float rh, float t, uint16_t fig2600, uint16_t fig2602) {
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
  #if CALIBRATE_VOC // Conditional
    out.TVOC_ = calibrate_voc (fig2600, fig2602, rh, t);
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
        case '5':
          co_cal = ((0.00680 * co) + (-0.07564 * rh) + 2.55893) * 1000;
          break;
        case '3':
          co_cal = ((0.00093 * co) + (-0.07671 * rh) + 2.07106) * 1000;
          break;
        default:
          co_cal = co; // Default = original signal
      }
      break;
    case 'V':
      switch (calID_number) { // Switch statement for number in ypodID
        case '6':
          co_cal = ((0.00109 * co) + (-0.07102 * rh) + 1.42407) * 1000;
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
        case '1':
          co_cal = ((0.00112 * co) + (-0.07675 * rh) + 1.25783) * 1000;
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
        case '1':
          co_cal = ((0.01298 * co) + (-0.07268 * rh) - 21.18629) * 1000;
          break;
        case '6':
          co_cal = ((0.00112 * co) + (-0.07675 * rh) + 1.95190) * 1000;
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
    case 'K':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2':
          co_cal = ((0.00104 * co) + (-0.03188 * rh) + 2.48679) * 1000;
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
      #if SERIAL_ENABLED
        // Serial.println("No CO calibration data for this pod.");
      #endif
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
        case '3':
          co2_cal = (0.78940 * co2) + (0.02548 * rh) + (1.48442 * t) - 81.97484;
          break;
        case '1':
          co2_cal = (0.58073 * co2) + (0.19558 * rh) + (-2.26283 * t) + 131.85519;
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
        case '2':
          co2_cal = (0.50596 * co2) + (0.35266 * rh) + (1.00766 * t) - 25.15380;
          break;
        case '6':
          co2_cal = (0.94356 * co2) + (0.53461 * rh) + (1.72155 * t) - 90.12135;
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
        case '1':
          co2_cal = (0.37740 * co2) + (-0.10422 * rh) + (-2.27307 * t) + 223.94961;
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
        case '1':
          co2_cal = (0.71347 * co2) + (0.68218 * rh) + (0.98741 * t) + 37.41322;
          break;
        case '6':
          co2_cal = (0.77203 * co2) + (0.68218 * rh) + (0.98741 * t) - 41.27804;
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
        case '2':
          co2_cal = (0.35536 * co2) + (0.79715 * rh) + (0.53309 * t) + 87.70961;
          break;
        default:
          co2_cal = co2; // Default = original signal
      }
      break;
    case 'L':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1':
          co2_cal = (0.26675 * co2) + (0.40967 * rh) + (-0.44070 * t) + 128.64243;
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
      #if SERIAL_ENABLED
        // Serial.println("No CO2 calibration data for this pod.");
      #endif
      co2_cal = co2; // Default = original signal 
  }
  if (co2_cal < 0) { // conditional for negative values
    co2_cal = 0;
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
          t_cal = (1.11116 * t) - 5.52967;
          break;
        case '8':
          t_cal = (1.05981 * t) - 4.09106;
          break;
        case '7':
          t_cal = (1.04182 * t) - 2.74393;
          break;
        case '2':
          t_cal = (1.06117 * t) - 3.69364;
          break;
        case '6':
          t_cal = (1.05834 * t) - 2.55614;
          break;
        case '1':
          t_cal = (1.07405 * t) - 4.30518;
          break;
        case '3':
          t_cal = (0.96286 * t) + 2.42155;
          break;
        case '5':
          t_cal = (0.92305 * t) + 3.22576;
          break;
        default:
          t_cal = t; // Default = original signal
      }
      break;
    case 'V':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1':
          t_cal = (1.03286 * t) - 1.94146;
          break;
        case '2':
          t_cal = (0.91601 * t) + 3.16763;
          break;
        case '6':
          t_cal = (0.98436 * t) + 2.32866;
          break;
        default:
          t_cal = t; // Default = original signal
      }
      break;
    case 'X':
      switch (calID_number) { // Switch statement for number in ypodID
        case '0':
          t_cal = (1.09128 * t) - 5.91627;
          break;
        case '1':
          t_cal = (0.93920 * t) + 1.15479;
          break;
        default:
          t_cal = t; // Default = original signal
      }
      break;
    case 'F':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2':
          t_cal = (1.10493 * t) - 5.64845;
          break;
        case '4':
          t_cal = (1.07020 * t) - 4.39655;
          break;
        default:
          t_cal = t; // Default = original signal
      }
      break;
    case 'D':
      switch (calID_number) { // Switch statement for number in ypodID
        case '4':
          t_cal = (1.01657 * t) - 3.23499;
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
          t_cal = (1.05173 * t) - 2.83754;
          break;
        default:
          t_cal = t; // Default = original signal
      }
      break;
    case 'T':
      switch (calID_number) { // Switch statement for number in ypodID
        case '4':
          t_cal = (1.05173 * t) - 2.83754;
          break;
        case '2':
          t_cal = (1.00995 * t) - 2.13822;
          break;
        case '1':
          t_cal = (1.10157 * t) + 2.37415;
          break;
        case '6':
          t_cal = (0.95264 * t) + 2.58850;
          break;
        default:
          t_cal = t; // Default = original signal
      }
      break;
    case 'O':
      switch (calID_number){ // Switch statement for number in ypodID
        case '1':
          t_cal = (1.08049 * t) - 4.03947; 
          break;
        default:
          t_cal = t; // Default = original signal
      }
      break;
    case 'K':
      switch (calID_number) { // Switch statement for number in ypodID
        case '3':
          t_cal = (1.10950 * t) - 5.75163;
          break;
        case '2':
          t_cal = (0.91166 * t) + 2.37415;
          break;
        default:
          t_cal = t; // Default = original signal
      }
      break;
    case 'L':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1':
          t_cal = (1.09721 * t) - 5.19718;
          break;
        default:
          t_cal = t; // Default = original signal
      }
      break;
    case 'B':
      switch (calID_number) { // Switch statement for number in ypodID
        case '8':
          t_cal = (1.05199 * t) - 4.55966;
          break;
        default:
          t_cal = t; // Default = original signal
      }
      break;
    case 'G':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2':
          t_cal = (1.10837 * t) - 5.12265;
          break;
        default:
          t_cal = t; // Default = original signal
      }
      break;
    default:
      #if SERIAL_ENABLED
        // Serial.println("No temperature calibration data for this pod.");
      #endif
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
          rh_cal = (1.18899 * rh) - 7.94909;
          break;
        case '8':
          rh_cal = (1.15852 * rh) - 2.94663;
          break;
        case '7':
          rh_cal = (1.08633 * rh) - 4.56042;
          break;
        case '2':
          rh_cal = (1.18720 * rh) - 11.41265;
          break;
        case '6':
          rh_cal = (1.04647 * rh) - 1.98453;
          break;
        case '1':
          rh_cal = (1.16004 * rh) - 5.86517;
          break;
        case '3':
          rh_cal = (0.86923 * rh) - 1.05297;
          break;
        case '5':
          rh_cal = (0.84466 * rh) + 1.02186;
          break;
        default:
          rh_cal = rh; // Default = original signal
      }
      break;
    case 'V':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1':
          rh_cal = (1.05049 * rh) - 2.28806;
          break;
        case '3':
          rh_cal = (0.84372 * rh) + 2.94169;
          break;
        case '6':
          rh_cal = (0.86409 * rh) + 1.26824;
          break;
        default:
          rh_cal = rh; // Default = original signal
      }
      break;
    case 'X':
      switch (calID_number) { // Switch statement for number in ypodID
        case '0':
          rh_cal = (1.29066 * rh) - 12.22035;
          break;
        case '1':
          rh_cal = (0.96548 * rh) + 0.32820;
          break;
        default:
          rh_cal = rh; // Default = original signal
      }
      break;
    case 'F':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2':
          rh_cal = (1.33239 * rh) - 17.35959;
          break;
        case '4':
          rh_cal = (1.24322 * rh) - 12.37479;
          break;
        default:
          rh_cal = rh; // Default = original signal
      }
      break;
    case 'D':
      switch (calID_number) { // Switch statement for number in ypodID
        case '4':
          rh_cal = (1.13629 * rh) - 3.32180;
          break;
        default:
          rh_cal = rh; // Default = original signal
      }
      break;  
    case 'Z':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2':
          rh_cal = (1.25800 * rh) - 5.96184;
          break;
        case '3': 
          rh_cal = (1.18946 * rh) - 8.40903;
          break;
        default:
          rh_cal = rh; // Default = original signal
      }
      break;
    case 'A':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1':
          rh_cal = (1.15608 * rh) - 5.57459;
          break;
        default:
          rh_cal = rh; // Default = original signal
      }
      break;
    case 'T':
      switch (calID_number) { // Switch statement for number in ypodID
        case '4':
          rh_cal = (1.06968 * rh) - 2.61488;
          break;
        case '2':
          rh_cal = (1.06249 * rh) - 3.53605;
          break;
        case '1':
          rh_cal = (0.99291 * rh) - 5.81132;
          break;
        case '6':
          rh_cal = (0.92664 * rh) - 5.81132;
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
        case '2':
          rh_cal = (0.96193 * rh) - 3.51642;
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
      #if Serial_Enabled
        // Serial.println("No relative humidity calibration data for this pod.");
      #endif
      rh_cal = rh; // Default = original signal
  }
  return rh_cal;
}

/**************************************************************************/
 /*!
 *    @brief  Stores the TVOC calibration equations
 */
/**************************************************************************/
int Cal::calibrate_voc (uint16_t fig2600, uint16_t fig2602, float rh, float t) {
  int voc_cal; // Stores calibrated relative humidity value
  switch (calID_letter) { // Switch statememnt for letter in ypodID
    case 'U':
      switch (calID_number) { // Switch statement for number in ypodID
        case '4':
          voc_cal = (0.24943 * fig2600) - (0.04176 * fig2602) - (2.14601 * t) + (2.08575 * rh) - 58.61145;
          break;
        case '8':
          voc_cal = (0.00007 * fig2600) + (0.27187 * fig2602) - (5.80255 * t) + (1.10081 * rh) - 47.22491;
          break;
        case '7':
          voc_cal = (-0.02459 * fig2600) + (0.34355 * fig2602) - (4.8841 * t) + (0.93670 * rh) - 2.81665;
          break;
        case '2':
          voc_cal = (0.19793 * fig2600) + (0.08935 * fig2602) - (6.93728 * t) + (1.29320 * rh) - 237.47212;
          break;
        case '6':
          voc_cal = (0.08350 * fig2600) + (0.09610 * fig2602) - (7.00497 * t) - (1.13425 * rh) - 26.52803;
          break;
        case '1':
          voc_cal = (0.32267 * fig2600) + (0.01340 * fig2602) - (8.07775 * t) - (3.84592 * rh) - 108.73772;
          break;
        case '3':
          voc_cal = (0.34530 * fig2600) - (0.02789 * fig2602) - (7.89486 * t) - (2.10618 * rh) - 123.04054;
          break;
        case '5':
          voc_cal = (0.23609 * fig2600) - (0.01862 * fig2602) - (3.15310 * t) - (2.22867 * rh) + 64.46123;
          break;
        default:
          voc_cal = 1; // Default = original signal
      }
      break;
    case 'V':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1':
          voc_cal = (-0.01203 * fig2600) + (0.30088 * fig2602) - (5.68046 * t) - (0.50848 * rh) - 65.52601;
          break;
        case '2':
          voc_cal = (-0.00280 * fig2600) + (0.30710 * fig2602) - (1.94186 * t) + (0.02974 * rh) - 140.82678;
          break;
        case '6':
          voc_cal = (0.05183 * fig2600) + (0.32653 * fig2602) - (9.03121 * t) - (2.86512 * rh) + 31.39674;
          break;
        default:
          voc_cal = 1; // Default = original signal
      }
      break;
    case 'X':
      switch (calID_number) { // Switch statement for number in ypodID
        case '0':
          voc_cal = (0.10555 * fig2600) + (0.52609 * fig2602) - (10.08610 * t) - (8.4633 * rh) - 140.17887;
          break;
        case '1':
          voc_cal = (1.10572 * fig2600) + (0.04663 * fig2602) - (10.25925 * t) - (7.99611 * rh) + 94.73553;
          break;
        default:
          voc_cal = 1; // Default = original signal
      }
      break;
    case 'F':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2':
          voc_cal = (0.08573 * fig2600) + (0.29712 * fig2602) - (6.74396 * t) - (3.74510 * rh) - 79.30350;
          break;
        case '4':
          voc_cal = (0.14993 * fig2600) + (0.76761 * fig2602) - (12.50848 * t) - (10.55894 * rh) - 60.76884;
          break;
        default:
          voc_cal = 1; // Default = original signal
      }
      break;
    case 'D':
      switch (calID_number) { // Switch statement for number in ypodID
        case '4':
          voc_cal = (0.28396 * fig2600) + (0.12025 * fig2602) - (28.22628 * t) - (19.14735 * rh) + 558.10205;
          break;
        default:
          voc_cal = 1; // Default = original signal
      }
      break;  
    case 'Z':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2':
          voc_cal = (0.11951 * fig2600) + (0.11722 * fig2602) - (10.51041 * t) - (3.26410 * rh) - 6.68153;
          break;
        case '3':
          voc_cal = (0.06341 * fig2600) + (0.12616 * fig2602) - (4.16462 * t) + (0.45900 * rh) - 166.55003;
          break;
        default:
          voc_cal = 1; // Default = original signal
      }
      break;
    case 'A':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1':
          voc_cal = (0.13273 * fig2600) + (0.23343 * fig2602) - (10.41855 * t) - (6.77488 * rh) - 168.42069;
          break;
        default:
          voc_cal = 1; // Default = original signal
      }
      break;
    case 'T':
      switch (calID_number) { // Switch statement for number in ypodID
        case '4':
          voc_cal = (0.31646 * fig2600) - (0.01155 * fig2602) - (9.47855 * t) - (1.69741 * rh) - 52.00493;
          break;
        case '2':
          voc_cal = (0.16532 * fig2600) + (0.29906 * fig2602) - (22.90767 * t) - (14.56504 * rh) - 333.32130;
          break;
        case '1':
          voc_cal = (0.24462 * fig2600) + (0.02381 * fig2602) - (8.01323 * t) - (5.18922 * rh) + 98.25879;
          break;
        case '6':
          voc_cal = (0.34719 * fig2600) + (0.02180 * fig2602) - (7.0645 * t) - (1.04831 * rh) - 34.33277;
          break;
        default:
          voc_cal = 1; // Default = original signal
      }
      break;
    case 'O':
      switch (calID_number){ // Switch statement for number in ypodID
        case '1':
          voc_cal = (0.21135 * fig2600) + (0.01461 * fig2602) - (6.22240 * t) - (1.91248 * rh) - 49.68230;
          break;
        default:
          voc_cal = 1; // Default = original signal
      }
      break;
    case 'K':
      switch (calID_number) { // Switch statement for number in ypodID
        case '3':
          voc_cal = (0.10336 * fig2600) + (0.16505 * fig2602) - (6.43139 * t) - (1.57642 * rh) - 63.34526;
          break;
        case '2':
          voc_cal = (0.12465 * fig2600) + (0.25099 * fig2602) - (11.69244 * t) - (8.85123 * rh) - 271.92746;
          break;
        default:
          voc_cal = 1; // Default = original signal
      }
      break;
    case 'L':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1':
          voc_cal = (0.06399 * fig2600) + (0.24491 * fig2602) - (11.73609 * t) - (8.63813 * rh) + 210.54627;
          break;
        default:
          voc_cal = 1; // Default = original signal
      }
      break;
    case 'B':
      switch (calID_number) { // Switch statement for number in ypodID
        case '8':
          voc_cal = (0.06657 * fig2600) + (0.11868 * fig2602) - (7.54970 * t) - (1.89487 * rh) + 7.06147;
          break;
        default:
          voc_cal = 1; // Default = original signal
      }
      break;
    case 'G':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2':
          voc_cal = (0.15554 * fig2600) + (0.01399 * fig2602) - (11.80565 * t) - (5.22316 * rh) + 20.15178;
          break;
        default:
          voc_cal = 1; // Default = original signal
      }
      break;
    default:
      #if Serial_Enabled
        // Serial.println("No relative humidity calibration data for this pod.");
      #endif
      voc_cal = 1; // Default = original signal
  }
  if (voc_cal < 0) { // conditional for negative values
    voc_cal = 0;
  }
  return voc_cal;
}