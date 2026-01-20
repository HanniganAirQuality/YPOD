/*******************************************************************************
 * @file    PMS.h
 * @brief   Plantower PMS x003 Family Sensors  
 *
 * @cite    kintel - https://github.com/kintel/PMS/tree/particles
 * 
 * @editor  Percy Smith, percy.smith@colorado.edu
 * @date    July 31, 2025
 ******************************************************************************/
#ifndef PMS_H
#define PMS_H

#include "Stream.h"

class PMS
{
public:
  static const uint16_t SINGLE_RESPONSE_TIME = 1000;
  static const uint16_t TOTAL_RESPONSE_TIME = 1000 * 10;
  static const uint16_t STEADY_RESPONSE_TIME = 1000 * 30;

  static const uint16_t BAUD_RATE = 9600;

  struct DATA {
    // Standard Particles, CF=1
    uint16_t pm10_standard;
    uint16_t pm25_standard;
    uint16_t pm100_standard;

    // Atmospheric environment
    uint16_t pm10_env;
    uint16_t pm25_env;
    uint16_t pm100_env;

    // Total particles
    uint16_t particles_03um;
    uint16_t particles_05um;
    uint16_t particles_10um;
    uint16_t particles_25um;
    uint16_t particles_50um;
    uint16_t particles_100um;
    bool hasParticles;
  };

  PMS(Stream&);
  void sleep();
  void wakeUp();
  void activeMode();
  void passiveMode();

  void requestRead();
  bool read(DATA& data);
  bool readUntil(DATA& data, uint16_t timeout = SINGLE_RESPONSE_TIME);

private:
  enum STATUS { STATUS_WAITING, STATUS_OK };
  enum MODE { MODE_ACTIVE, MODE_PASSIVE };

  uint8_t _payload[24];
  Stream* _stream;
  DATA* _data;
  STATUS _status;
  MODE _mode = MODE_ACTIVE;

  uint8_t _index = 0;
  uint16_t _frameLen;
  uint16_t _checksum;
  uint16_t _calculatedChecksum;

  void loop();
};

#endif
