/*******************************************************************************
 * @file    ads_module.cpp
 * @brief   Splits ADS1115 code from .ino & updates from ADS1015.h --> ADS1115.h
 *
 * @cite    XPOD >> ads_module.cpp by Ajay Kandagal, ajka9053@colorado.edu
 *
 * @author  Percy Smith, percy.smith@colorado.edu
 * @date    July 4, 2024
 * @log     Formatting & adding comments 
 * @TBD     Add a read function returning heaters structure
******************************************************************************/
#include "ads_module.h"

/*******************************************************************************
 * @file    ads_module.cpp
 * @brief   Splits ADS1115 code from .ino & updates from ADS1015.h --> ADS1115.h
 *
 * @cite    XPOD >> ads_module.cpp by Ajay Kandagal, ajka9053@colorado.edu
 *
 * @author  Percy Smith, percy.smith@colorado.edu
 * @date    July 4, 2024
 * @log     Formatting & adding comments 
 * @TBD     Add a read function returning heaters structure


/**************************************************************************/
 /*!
 *    @brief  ADS_Module object; Assigns addresses & channels to ADS1115 modules
 */
/**************************************************************************/
ADS_Module::ADS_Module()
{
  ads_module[FIG1_H].addr = 0x48;
  ads_module[FIG1_H].channel = 0;

  ads_module[FIG1].addr = 0x48;
  ads_module[FIG1].channel = 1;

  ads_module[FIG2_H].addr = 0x48;
  ads_module[FIG2_H].channel = 2;

  ads_module[FIG2].addr = 0x48;
  ads_module[FIG2].channel = 3;

  ads_module[CO].addr = 0x49;
  ads_module[CO].channel = 0;

  ads_module[ADC_UNUSED].addr = 0x49;
  ads_module[ADC_UNUSED].channel = 1;

  ads_module[E2V_H].addr = 0x49;
  ads_module[E2V_H].channel = 2;

  ads_module[E2V].addr = 0x49;
  ads_module[E2V].channel = 3;

  for (int i = 0; i < ADS_SENSOR_COUNT; i++)
    ads_module[i].status = false;
} //ADS_Module()

/**************************************************************************/
 /*!
 *   @brief  Start ADS_Module by init all ads sensors on ADS1115
 *   @return True if find ADS1115 channels (4 & 4), 
 *           False if status of channels is false
 */
/**************************************************************************/
bool ADS_Module::begin()
{
  for (int i = 0; i < ADS_SENSOR_COUNT; i++)
  {
    if (ads_module[i].module.begin(ads_module[i].addr))
      ads_module[i].status = true;
  }

  for (int i = 0; i < ADS_SENSOR_COUNT; i++)
  {
    if (ads_module[i].status == false)
      return false;
  }

  return true;
} //bool ADS_Module::begin()

/**************************************************************************/
 /*!
 *    @brief  Reads raw sensor readings; no signal processing!!
 *        @param  ads_sensor_id index of the sensor to be read (in id_e form)
 *    @return Raw ADS1115 reading for relevant channel (or -999 for error)
 */
/**************************************************************************/
uint16_t ADS_Module::read_raw(ads_sensor_id_e ads_sensor_id)
{
  ads_module_t *sensor = &ads_module[ads_sensor_id];

  if (!sensor->status)
    return -999;

  return sensor->module.readADC_SingleEnded(sensor->channel);
} //uint16_t ADS_Module::read_raw(ads_sensor_id_e ads_sensor_id)

/**************************************************************************/
 /*!
 *    @brief  Updates values and returns structured dataset
 *    @return ads_noheaters structured dataset (w/o heaters)
 */
/**************************************************************************/
ads_noheaters ADS_Module::return_updated()
{
  ads_user.Fig1 = read_raw(FIG1);
  delay(100);
  ads_user.Fig2 = read_raw(FIG2);
  delay(100);
  ads_user.e2V = read_raw(E2V);
  delay(100);
  ads_user.CO = read_raw(CO);
  delay(100);

  return ads_user;
} //ads_noheaters ADS_Module::return_updated()

