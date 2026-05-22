/*******************************************************************************
 * @file    plantower_module.cpp
 * @brief   Splits PMS5003 code from .ino 
 *
 * @cite    PM_Firmware.ino (YPOD) by ?? 
 *
 * @author  Percy Smith, percy.smith@colorado.edu
 * @date    July 4, 2024
 * @log     Formatting & adding comments 
 * @TBD     Optimize to include an alternative structure for data to save memory
******************************************************************************/
#include "plantower_module.h"

/**************************************************************************/
 /*!
 *    @brief  PMS5003 object for Plantower PMS5003
 */
/**************************************************************************/
PMS5003::PMS5003(){}

/**************************************************************************/
 /*!
 *    @brief  Reads incoming serial stream from PMS5003 sensor 
 *        @param  pmsSerial SoftwareSerial --> Stream *s
 *    @return True if data is available and false if not available
 */
/**************************************************************************/
bool PMS5003::readPMSdata(Stream *s) 
{
  // if pmsSerial is not available, returns false
  if (! s->available()) {return false;}
  
  // Read a byte at a time until we get to the special '0x42' start-byte1
  if (s->peek() != START_BYTE1) {s->read(); return false;}
 
  // Now read all 32 bytes
  if (s->available() < 32) {return false;}
    
  uint8_t buffer[32];    
  uint16_t sum = 0;
  s->readBytes(buffer, 32); //places serial stream into a readBytes buffer array
 
  // get checksum ready
  for (uint8_t i=0; i<30; i++) {sum += buffer[i];}
  
  // The data comes in endian'd, this solves it so it works on all platforms
  uint16_t buffer_u16[15];
  for (uint8_t i=0; i<15; i++) {buffer_u16[i] = buffer[2 + i*2 + 1];  buffer_u16[i] += (buffer[2 + i*2] << 8);}
 
  // put it into a nice struct :)
  memcpy((void *)&PMS_data, (void *)buffer_u16, 30);
 
  // make sure calculated sum is not equal to the checksum
  if (sum != PMS_data.checksum) {return false;}

  // if u got this far then we good
  return true;
}

/**************************************************************************/
 /*!
 *    @brief  Returns the private structure  
 *    @return pms5003data structure to include all relevant variables
 */
/**************************************************************************/
pms5003data PMS5003::returnPMdataset()
{
  return PMS_data;
}