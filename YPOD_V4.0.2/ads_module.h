/*******************************************************************************
 * @file    ads_module.h
 * @brief   Splits ADS1115 code from .ino & updates from ADS1015.h --> ADS1115.h
 *
 * @cite    XPOD >> ads_module.h by Ajay Kandagal, ajka9053@colorado.edu
 *
 * @author  Percy Smith, percy.smith@colorado.edu
 * @date    July 4, 2024
 * @log     Formatting & adding comments 
 * @TBD     Add a read function returning heaters structure
******************************************************************************/
#ifndef _ADS_MODULE_H
#define _ADS_MODULE_H

#include <Arduino.h>
#include <Adafruit_ADS1X15.h>

#include "YPOD_node.h"

/*! Index: FIG1, FIG2, E2V, CO, FIG1_H, FIG2_H, E2V_H, ADC_UNUSED, ADS_SENSOR_COUNT */
enum ads_sensor_id_e
{
    FIG1 = 0,
    FIG2,
    E2V,
    CO,
    FIG1_H,
    FIG2_H,
    E2V_H,
    ADC_UNUSED,
    ADS_SENSOR_COUNT
};  //enum ads_sensor_id_e

/*! (per each sensor) addr, channel, status, module (ADS1115) */
struct ads_module_t
{
    uint8_t addr;
    int8_t channel;
    bool status;
    Adafruit_ADS1115 module;
}; //struct ads_module_t

/*! ADS data structure (ALL DATA) as uint16_t */
struct ads_heaters
{
  uint16_t Fig1;
  uint16_t Fig2;
  uint16_t e2V;
  uint16_t CO;
  uint16_t Fig1_H;
  uint16_t Fig2_H;
  uint16_t e2V_H;
  uint16_t NA_ADC;
};  //struct ads_heaters

/*! ADS data structure (NO HEATERS OR UNUSED) as uint16_t */
struct ads_noheaters
{
  uint16_t Fig1;
  uint16_t Fig2;
  uint16_t e2V;
  uint16_t CO;
};  //struct ads_noheaters

/*! ADS1115 to include Fig 2600, Fig 2602, MiCS-2611, CO-B4 */
class ADS_Module {
  public:
    ADS_Module();
    bool begin();

    uint16_t read_raw(ads_sensor_id_e ads_sensor_id);
    ads_noheaters return_updated();

  private:
    ads_module_t ads_module[ADS_SENSOR_COUNT];
    ads_heaters ads_alldata;
    ads_noheaters ads_user;
};  //class ADS_Module

#endif  //_ADS_MODULE_H
