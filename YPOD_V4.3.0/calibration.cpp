/*******************************************************************************
 * @file    calibration.cpp
 * @brief   Calibration function architecture and equations for co, co2, 
 *          temperature and relative humidity calibrations
 *
 * @author  Chiara Pesce, chiara.pesce@colorado.edu
 *          Percy Smith, percy.smith@colorado.edu 
 * 
 * @date    August 5, 2026
 * @log     Updates for E8 & D2 from Jul/Aug Col/Cal for CU Museum
 * @check   int/float/uint16_t change cases 
******************************************************************************/
#include "calibration.h"

/**************************************************************************/
 /*!
 *    @brief calls calibration functions and returns struct w/ variables
 */
/**************************************************************************/
calOutput Cal::calibrate (uint16_t co, uint16_t noise, float co2, float rh, float t, uint16_t fig2600, uint16_t fig2602) {
  calOutput out; // creates an isntance of calOutput struct
  #if CALIBRATE_CO // Conditional
    out.CO_ = calibrate_co (co, noise, rh);
  #endif
  #if CALIBRATE_CO2 // Conditional
    out.CO2_ = calibrate_co2 (co2, rh, t);
  #endif
  #if CALIBRATE_T // Conditional
    out.T_ = calibrate_t(t);
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
int Cal::calibrate_co (uint16_t co, uint16_t noise, float rh) {
  int co_cal; // Stores calibrated CO value
  switch (calID_letter) { // Switch statememnt for letter in ypodID
    case 'U':
      switch (calID_number) { // Switch statement for number in ypodID
        case '4':
          co_cal = ((0.00109 * co) + (-0.12464 * rh) + 4.71174);
          break;
        case '8':
          co_cal = ((0.00110 * co) + (-0.12029 * rh) + 4.04736);
          break;
        case '7':
          co_cal = ((0.00113 * co) + (-0.11509 * rh) + 4.27234);
          break;
        case '2':
          co_cal = ((0.00117 * co) + (-0.12668 * rh) + 5.09077);
          break;
        case '1': //Include Noise Factor
          co_cal = (-0.00147948 * noise + (0.000373685 * co) + 5.07628);
          break;
        case '5': //Include Noise Factor
          co_cal = (-0.00174219 * noise + (0.0187948 * co) + -7.26234);
          break; 
        case '3':
          co_cal = ((0.00093 * co) + (-0.07671 * rh) + 2.07106);
          break;
        default:
          co_cal = co; // Default = original signal
      }
      break;
    case 'V':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1':
          co_cal = ((0.000568292 * co) + (-0.0589695));
          break; 
        case '5':
          co_cal = ((0.000509267 * co) + (-0.143343));
          break; 
        case '3':
          co_cal = ((0.00111 * co) + (-0.11678 * rh) + 4.11220);
          break;
        case '4':
          co_cal = ((0.00111 * co) + (-0.11678 * rh) + 4.11220);
          break;
        default:
          co_cal = co; // Default = original signal
      }
      break;
    case 'M':
      switch (calID_number) { // Switch statement for number in ypodID
        case '6':
          co_cal = ((0.000437432 * co) + (0.254876));
          break;
        default:
          co_cal = co; // Default = original signal
      } 
      break;
    case 'H':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1': //Include Noise Factor
          co_cal = (-0.00175449 * noise + (0.000470332 * co) + 5.59755);
          break;
        default:
          co_cal = co; // Default = original signal
      } 
      break;
    case 'X':
      switch (calID_number) { // Switch statement for number in ypodID
        case '0':
          co_cal = ((0.00108 * co) + (-0.12966 * rh) + 4.89108);
          break;
        case '1'://Include Noise Factor
          co_cal = (-0.00144829 * noise + (0.000486219 * co) + 4.75306);
          break;
        case '9':
          co_cal = ((0.000471372 * co) + (0.215808));
          break;
        default:
          co_cal = co; // Default = original signal
      }
      break;
    case 'F':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2'://Include Noise Factor
          co_cal = (-0.0016458 * noise + (0.000425477 * co) + 5.41702);
          break;
        case '4':
          co_cal = ((0.000816055 * co) + (-0.628235));
          break;
        default:
          co_cal = co; // Default = original signal
      }
      break;
    case 'D':
      switch (calID_number) { // Switch statement for number in ypodID
        case '6'://Include Noise Factor
          co_cal = (-0.00150594 * noise + (0.000518011 * co) + 4.89046);
          break;
        case '4':
          co_cal = ((0.00102 * co) + (-0.16957 * rh) + 5.30658);
          break;
        case '2':
          co_cal = ((0.000561651 * co) - 0.178779);
          break;
        default:
          co_cal = co; // Default = original signal
      }
      break;  
    case 'Z':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2':
          co_cal = ((0.000930762 * co) - 0.721443);
          break;
        case '3': 
          co_cal = ((0.000694492 * co) - 0.4909);
          break;
        default:
          co_cal = co; // Default = original signal
      }
      break;
    case 'A':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1':
          co_cal = ((0.00104 * co) + (-0.12167 * rh) + 4.43994);
          break;
        default:
          co_cal = co; // Default = original signal
      }
      break;
    case 'T':
      switch (calID_number) { // Switch statement for number in ypodID
        case '4':
          co_cal = ((0.00105 * co) + (-0.14761 * rh) + 4.78947);
          break;
        case '6':
          co_cal = ((0.000458691 * co) + 0.102502);
          break;
        case '2':
          co_cal = ((0.00111 * co) + (-0.11678 * rh) + 4.11220);
          break;
        case '1'://Include Noise Factor
          co_cal = (-0.00142065 * noise + (0.00039527 * co) + 4.71982);
          break;
        case '3':
          co_cal = ((0.00111 * co) + (-0.11678 * rh) + 4.11220);
          break;
        case '5':
          co_cal = ((0.000618324 * co) + -0.161696);
          break;
        default:
          co_cal = co; // Default = original signal
      }
      break;
    case 'O':
      switch (calID_number){ // Switch statement for number in ypodID
        case '1':
          co_cal = ((0.0004768 * co) + -0.00405324);
          break;
        default:
          co_cal = co; // Default = original signal
      }
      break;
    case 'K':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1':
          co_cal = ((0.000526590 * co) + 0.145893);
          break;
        case '2':
          co_cal = ((0.00104 * co) + (-0.03188 * rh) + 2.48679);
          break;
        case '3'://Include Noise Factor
          co_cal = (-0.0017354 * noise + (0.000375311 * co) + 5.7147);
          break;
        case '4':
          co_cal = ((0.000482931 * co) + 0.0197627);
          break;
        default:
          co_cal = co; // Default = original signal
      }
      break;
    case 'L':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1'://Include Noise Factor
          co_cal = (-0.0014153 * noise + (0.00068217 * co) + 4.2126);
          break;
        default:
          co_cal = co; // Default = original signal
      }
      break;
    case 'B':
      switch (calID_number) { // Switch statement for number in ypodID
        case '8':
          co_cal = ((0.00112 * co) + (-0.12563 * rh) + 4.56465);
          break;
        default:
          co_cal = co; // Default = original signal
      }
      break;
    case 'G':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2':
          co_cal = ((0.000690797 * co) + -0.470575);
          break;
        default:
          co_cal = co; // Default = original signal
      }
      break;
    case 'E':
      switch (calID_number) { // Switch statement for number in ypodID
        case '8':
          co_cal = ((0.0174481 * co) + 50.6432);
          break;
        default:
          co_cal = co; // Default = original signal
      }
      break;
    case 'R':
      switch (calID_number) { // Switch statement for number in ypodID
        case '8':
          co_cal = ((0.000473967 * co) + 0.23839);
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
          co2_cal = (-4.94891 * co2) + (244.931 * sqrt(co2)) + -2444.59;
          break;
        case '5':
          co2_cal = (-4.77898 * co2) + (230.883 * sqrt(co2)) + -2201.11;
          break;
        default:
          co2_cal = co2; // Default = original signal
      }
      break;
    case 'V':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1':
          co2_cal = (10.1329 * co2) + (-359.208 * sqrt(co2)) + 3607.71;
          break;
        case '5':
          co2_cal = (2.23607 * co2) + (-49.1908 * sqrt(co2)) + 554.44;
          break;
        case '2':
          co2_cal = (2.02172 * co2) + (-45.2756 * sqrt(co2)) + 586.762;
          break;
        case '6':
          co2_cal = (0.94356 * co2) + (0.53461 * rh) + (1.72155 * t) - 90.12135;
          break;
        case '3':
          co2_cal = (1.09692 * co2) + (0.03911 * rh) + (0.06376 * t) - 46.68639;
          break;
        case '4':
          co2_cal = (1.09692 * co2) + (0.03911 * rh) + (0.06376 * t) - 46.68639;
          break;
        default:
          co2_cal = co2; // Default = original signal
      }
      break;
    case 'M':
      switch (calID_number) { // Switch statement for number in ypodID
        case '6':
          co2_cal = (7.75456 * co2) + (-238.755 * sqrt(co2)) + 2206.92;
          break;
        default:
          co2_cal = co2; // Default = original signal
      }
      break;
    case 'H':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1':
          co2_cal = (-0.562765 * co2) + (46.2971 * sqrt(co2)) + -247.324;
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
          co2_cal = (1.84398 * co2) + (-28.3764 * sqrt(co2)) + 344.909;
          break;
        case '9':
          co2_cal = (19.4067 * co2) + (-642.18 * sqrt(co2)) + 5740.64;
          break;
        default:
          co2_cal = co2; // Default = original signal
      }
      break;
    case 'F':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2':
          co2_cal = (-1.64405 * co2) + (83.7334 * sqrt(co2)) + -401.619;
          break;
        case '4':
          co2_cal = (4.55935 * co2) + (-105.446 * sqrt(co2)) + 980.383;
          break;
        default:
          co2_cal = co2; // Default = original signal
      }
      break;
    case 'D':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2':
          co2_cal = (-0.305528 * co2) + (45.234 * sqrt(co2)) - 239.537;
          break;
        case '4':
          co2_cal = (1.09476 * co2) + (-1.89513 * rh) + (-5.76502 * t) - 4.97449;
          break;
        case '6':
          co2_cal = (4.24279 * co2) + (-134.794 * sqrt(co2)) + 1489.17;
          break;
        default:
          co2_cal = co2; // Default = original signal
      } 
      break; 
    case 'Z':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2':
          co2_cal = (6.58878 * co2) + (-233.701 * sqrt(co2)) + 2465.49;
          break;
        case '3': 
          co2_cal = (4.31232 * co2) + (-110.36 * sqrt(co2)) + 1091.88;
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
          co2_cal = (1.36979 * co2) + (-1.73881 * rh) + (4.56957 * t) + 628.022;
          break;
        case '1':
          co2_cal = ((2.48895 * co2) + (-44.459 * sqrt(co2)) + 711.659);
          break;
        case '6':
          co2_cal = ((3.2524 * co2) + (-72.5218 * sqrt(co2)) + 711.659);
          break;
        case '3':
          co2_cal = (1.09692 * co2) + (0.03911 * rh) + (0.06376 * t) - 46.68639;
          break;
        case '5':
          co2_cal = ((5.51758 * co2) + (-159.422 * sqrt(co2)) + 1495.2);
          break;
        default:
          co2_cal = co2; // Default = original signal
      }
      break;
    case 'O':
      switch (calID_number){ // Switch statement for number in ypodID
        case '1':
          co2_cal = ((8.78258 * co2) + (-289.459 * sqrt(co2)) + 2788.72);
          break;
        default:
          co2_cal = co2; // Default = original signal
      }
      break;
    case 'K':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1':
          co2_cal = (7.50096 * co2) + (-171.562 * sqrt(co2)) + 1424.42;
          break;
        case '3':
          co2_cal = (2.66501 * co2) + (-29.0544 * sqrt(co2)) + 524.144;
          break;
        case '2':
          co2_cal = (0.35536 * co2) + (0.79715 * rh) + (0.53309 * t) + 87.70961;
          break;
        case '4':
          co2_cal = (10.5065 * co2) - (300.198 * sqrt(co2)) + 2581.7;
          break;
        default:
          co2_cal = co2; // Default = original signal
      }
      break;
    case 'L':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1':
          co2_cal = (-1.46287 * co2) + (76.3163 * sqrt(co2)) + -472.462;
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
          co2_cal = ((2.20067 * co2) + (-45.5458 * sqrt(co2)) + 451.97);
          break;
        default:
          co2_cal = co2; // Default = original signal
      }
      break;
    case 'E':
      switch (calID_number) { // Switch statement for number in ypodID
        case '8':
          co2_cal = ((-0.14443 * co2) + (34.7383 * sqrt(co2)) + 886.83);
          break;
        default:
          co2_cal = co2; // Default = original signal
      }
      break;
    case 'R':
      switch (calID_number) { // Switch statement for number in ypodID
        case '8':
          co2_cal = ((3.40816 * co2) + (-88.392 * sqrt(co2)) - 228.426);
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
        case '3':
          t_cal = (1.03915 * t) - 2.26521;
          break;
        case '4':
          t_cal = (1.02993 * t) + 1.05383;
          break;
        default:
          t_cal = t; // Default = original signal
      }
      break;
    case 'M':
      switch (calID_number) { // Switch statement for number in ypodID
        case '6':
          t_cal = (1.02993 * t) + 1.05383;
          break;
        default:
          t_cal = t; // Default = original signal
      }
      break;
    case 'H':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1':
          t_cal = (1.02993 * t) + 1.05383;
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
        case '9':
          t_cal = (1.03915 * t) - 2.26521;
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
        case '3':
          t_cal = (1.03947 * t) - 2.37966;
          break;
        case '5':
          t_cal = (1.03947 * t) - 2.37966;
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
        case '4':
          t_cal = (1.03915 * t) - 2.26521;
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
        case '2':
          rh_cal = (0.84372 * rh) + 2.94169;
          break;
        case '6':
          rh_cal = (0.86409 * rh) + 1.26824;
          break;
        case '3':
          rh_cal = (1.09012 * rh) - 4.97333;
          break;
        case '4':
          rh_cal = (0.90586 * rh) - 0.12867;
          break;
        default:
          rh_cal = rh; // Default = original signal
      }
      break;
    case 'M':
      switch (calID_number) { // Switch statement for number in ypodID
        case '6':
          rh_cal = (0.90586 * rh) - 0.12867;
          break;
        default:
          rh_cal = rh; // Default = original signal
      }
      break;
    case 'H':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1':
          rh_cal = (0.90586 * rh) - 0.12867;
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
        case '9':
          rh_cal = (1.09012 * rh) - 4.97333;
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
        case '3':
          rh_cal = (1.09647 * rh) - 5.14926;
          break;
        case '5':
          rh_cal = (1.09647 * rh) - 5.14926;
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
        case '4':
          rh_cal = (1.09012 * rh) - 4.97333;
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
 *    @brief  Stores the TVOC or Methane calibration equations
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
        case '1'://METHANE
          voc_cal = (0.061028 * fig2600) + (0.0000452118 * sq(fig2600)) + 1889.65;
          break;
        case '3':
          voc_cal = (0.34530 * fig2600) - (0.02789 * fig2602) - (7.89486 * t) - (2.10618 * rh) - 123.04054;
          break;
        case '5'://METHANE
          voc_cal = (0.197263 * fig2600) + (0.00000779416 * sq(fig2600)) + 1729.19;
          break;
        default:
          voc_cal = 1; // Default = original signal
      }
      break;
    case 'V':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1'://METHANE
          voc_cal = (-0.484856 * fig2600) + (0.000153563 * sq(fig2600)) + 2427.63;
          break;
        case '5'://METHANE
          voc_cal = (-0.559647 * fig2600) + (0.000625833 * sq(fig2600)) + 2173.10;
          break;
        case '2'://METHANE
          voc_cal = (0.189332 * fig2600) + (0.00000829048 * sq(fig2600)) + 1733.98;
          break;
        case '6':
          voc_cal = (0.05183 * fig2600) + (0.32653 * fig2602) - (9.03121 * t) - (2.86512 * rh) + 31.39674;
          break;
        case '4': 
          voc_cal = (0.25902 * fig2600) + (0.10550 * fig2602) - (8.69664 * t) - (3.37784 * rh) - 33.94685;
          break;
        case '3':
          voc_cal = (0.25902 * fig2600) + (0.10550 * fig2602) - (8.69664 * t) - (3.37784 * rh) - 33.94685;
          break;
        default:
          voc_cal = 1; // Default = original signal
      }
      break;
    case 'M':
      switch (calID_number) { // Switch statement for number in ypodID
        case '6'://METHANE
          voc_cal = (-0.578614 * fig2600) + (0.000113721 * sq(fig2600)) + 2785.16;
          break;
        default:
          voc_cal = 1; // Default = original signal
      }
      break;
    case 'H':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1'://METHANE
          voc_cal = (0.159166 * fig2600) + (0.000000864988 * sq(fig2600)) + 1695.67;
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
        case '1'://METHANE
          voc_cal = (0.560671 * fig2600) + (-0.0000130556 * sq(fig2600)) + 1871.79;
          break;
        case '9'://METHANE
          voc_cal = (-0.541641 * fig2600) + (0.0000868594 * sq(fig2600)) + 2894.62;
          break;
        default:
          voc_cal = 1; // Default = original signal
      }
      break;
    case 'F':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2'://METHANE
          voc_cal = (0.0526873 * fig2600) + (0.0000548131 * sq(fig2600)) + 1845.42;
          break;
        case '4'://METHANE
          voc_cal = (-0.902638 * fig2600) + (0.000299265 * sq(fig2600)) + 2727.50;
          break;
        default:
          voc_cal = 1; // Default = original signal
      }
      break;
    case 'D':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2': //METHANE
          voc_cal = (-0.8405226 * fig2600) + (0.000310953 * sq(fig2600)) + 2566.71;
          break;
        case '4':
          voc_cal = (0.28396 * fig2600) + (0.12025 * fig2602) - (28.22628 * t) - (19.14735 * rh) + 558.10205;
          break;
        case '6': //METHANE
          voc_cal = (0.088112 * fig2600) + (0.0000351718 * sq(fig2600)) + 1798.27;
          break;
        default:
          voc_cal = 1; // Default = original signal
      }
      break;  
    case 'Z':
      switch (calID_number) { // Switch statement for number in ypodID
        case '2'://METHANE
          voc_cal = (-0.668306 * fig2600) + (0.00018269 * sq(fig2600)) + 2661.32;
          break;
        case '3'://METHANE
          voc_cal = (-0.699267 * fig2600) + (0.000234151 * sq(fig2600)) + 2571.19;
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
        case '1'://METHANE
          voc_cal = ((0.236544 * fig2600) + (-0.0000110424 * sq(fig2600)) + 1867.89);
          break;
        case '6'://METHANE
          voc_cal = ((-0.118872 * fig2600) + (0.0000261018 * sq(fig2600)) + 2181.49);
          break;
        case '3':
          voc_cal = (0.25902 * fig2600) + (0.10550 * fig2602) - (8.69664 * t) - (3.37784 * rh) - 33.94685;
          break;
        case '5'://METHANE
          voc_cal = ((-0.0763529 * fig2600) + (0.0000409808 * sq(fig2600)) + 2082.05);
          break;
        default:
          voc_cal = 1; // Default = original signal
      }
      break;
    case 'O':
      switch (calID_number){ // Switch statement for number in ypodID
        case '1': //METHANE
          voc_cal = (-0.401827 * fig2600) + (0.000173683 * sq(fig2600)) + 2270.87;
          break;
        default:
          voc_cal = 1; // Default = original signal
      }
      break;
    case 'K':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1': //METHANE
          voc_cal = (-0.637129 * fig2600) + (0.000161527 * sq(fig2600)) + 2677.52;
          break;
        case '3'://METHANE
          voc_cal = (0.320599 * fig2600) + (-0.0000106905 * sq(fig2600)) + 1706.07;
          break;
        case '2':
          voc_cal = (0.12465 * fig2600) + (0.25099 * fig2602) - (11.69244 * t) - (8.85123 * rh) - 271.92746;
          break;
        case '4': //METHANE
          voc_cal = (-0.864984 * fig2600) + (0.000146441 * sq(fig2600)) + 3322.26;
          break;
        default:
          voc_cal = 1; // Default = original signal
      }
      break;
    case 'L':
      switch (calID_number) { // Switch statement for number in ypodID
        case '1'://METHANE
          voc_cal = (0.062429 * fig2600) + (0.0000253738 * sq(fig2600)) + 1801.6;
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
        case '2'://METHANE
          voc_cal = ((-0.755082 * fig2600) + (0.000150379 * sq(fig2600)) + 2997.84);
          break;
        default:
          voc_cal = 1; // Default = original signal
      }
      break;
    case 'E':
      switch (calID_number) { // Switch statement for number in ypodID
        case '8': //METHANE
          voc_cal = ((-0.913449 * fig2600) + (0.000233333 * sq(fig2600)) + 2938.14);
          break;
        default:
          voc_cal = 1; // Default = original signal
      }
      break;
    case 'R':
      switch (calID_number) { // Switch statement for number in ypodID
        case '8': //METHANE
          voc_cal = ((-0.771919 * fig2600) + (0.000155072 * sq(fig2600)) + 3008.15);
          break;
        default:
          voc_cal = 1; // Default = original signal
      }
      break;
    default:
      #if Serial_Enabled
        // Serial.println("No VOC calibration data for this pod.");
      #endif
      voc_cal = 1; // Default = original signal
  }
  if (voc_cal < 0) { // conditional for negative values
    voc_cal = 0;
  }
  return voc_cal;
}
