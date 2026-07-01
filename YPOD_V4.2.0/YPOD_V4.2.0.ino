/*********************************************************************************
 * @project HAQ Lab YPOD
 *
 * @file    YPOD_V4.2.0.ino
 * @author  Percy Smith, percy.smith@colorado.edu
            Chiara Pesce, chiara.pesce@colorado.edu
 * @brief   Central firmware to collect data through the YPOD
 * 
 * @date    June 29, 2026
 * @version V4.2.0
 * @log     Uses the same row formatter for SD and Serial outputs
***********************************************************************************/
/*  Libraries  */
#include <Arduino.h>
#include <SPI.h>     //P - last tested with "SPI@1.0"
#include <Wire.h>    //P - last tested with "Wire@1.0"
#include <SdFat.h>   //P - last tested with "SdFat@2.2.3"
#include <RTClib.h>  //P - last tested with "RTClib@2.1.4"
/*  Header Files  */
#include "YPOD_node.h"
#include "ads_module.h"  //P - last tested with "Adafruit ADS1X15@2.5.0" & "Adafruit BusIO@1.16.1"

/*  Conditional Declarations  */
// Calibration equations
#if CALIBRATE
#include "calibration.h"
Cal cal;
#endif
//BME 180 - Temperature & Pressure - Bosch (DISCONTINUED)
#if BME180
#include "SFE_BMP180.h"
SFE_BMP180 BMP;
#endif  //BME180
//SHT 25 - Temperature & Pressure - Sensirion (DISCONTINUED)
#if SHT25
unsigned int temperature_board, humidity_board;
#endif  //SHT25
//Quadstat - Various Electrolytic Gas Sensors - Alphasense
#if QUAD_ENABLED
#include "quad_module.h"  //P - last tested with "MCP342x@1.0.4"
QUAD_Module quad_module;
quad_data qs_data;
#endif  //QUAD_ENABLED

#if PMS_ENABLED
#include <SoftwareSerial.h>  //P - last tested with "SoftwareSerial@1.0"
#include "PMS.h"
SoftwareSerial pmsSerial(2, 3);
PMS pms(pmsSerial);
PMS::DATA pms_data;
#endif  //PMS_ENABLED
//ADS1115 Modules - Used for CO-B4 (CO), Fig2600 (VOC), Fig2602 (VOC) & MiSC 2611 (O3)
ADS_Module ads_module;
#if HEATERS_ENABLED
ads_heaters ads_data;
#else
ads_noheaters ads_data;
#endif  //HEATERS_ENABLED

/*  RTC & File Formatting */
//RTC DS3231 Module - to re-initialize time, use RTClib>examples>ds3231
RTC_DS3231 RTC;
//SdFat (SD Card) & File file (file on SD)
SdFat sd;
File file;
// Buffers
// char ypodID[] = "YPODID";
char fileName[] = "YPODID_YYYY_MM_DD.CSV";
char bufftime[] = "YYYY-MM-DDThh:mm:ss";
int Y, M, D, h, m, s;
const char firmwareVersion[] = "4.2.0";

void printDataRow(Print &out, bool pm_returned, double bme_temperature, double bme_pressure, float sht_temperature, float sht_humidity, float co2);
void printPMS(Print &out, bool pm_returned);
void printQuad(Print &out);

/***************************************************************************************/
void setup() {
  /*  Intializing Global Variables  */
#if SERIAL_ENABLED
  Serial.begin(9600);
#endif  //SERIAL_ENABLED
#if PMS_ENABLED
  pmsSerial.begin(9600);
  delay(100);
  pms.passiveMode();
  delay(100);
  pms.clearInput();
#endif  //PMS_ENABLED
  //Central Firmware (comms protocols)
  Wire.begin();
  SPI.begin();
  //Object Begins
  RTC.begin();  //Initialize RTC
#if RTC_UPDATE
  RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
#endif                 //RTC_UPDATE
  ads_module.begin();  //Initialize ads_module (creates objects in .cpp)
#if BME180
  BMP.begin();  //Initialize BME 180 (creates objects in .cpp)
#endif          //BME180
  //Initialize Pins - Establish direction of pin comms
  pinMode(SD_CS, OUTPUT);
  pinMode(G_LED, OUTPUT);

  /*  SD Card & File Setup  */
  digitalWrite(SD_CS, LOW);  //Pull SD_CS pin LOW to initialize SPI comms
  sd.begin(SD_CS);           //Initialize SD Card with relevant chip select pin
  // Establish contact with SD card - if initialization fails, run until success
  while (!sd.begin(SD_CS)) {
#if SERIAL_ENABLED
    Serial.println("insert sd card to begin");
#endif                        //SERIAL_ENABLED
    sd.begin(SD_CS);          //attempt to initialize again
  }                           //while(!sd.begin(SD_CS))
  digitalWrite(G_LED, HIGH);  //if we exit the while loop, blink green LED once to indicate success
  //File Naming (FORMATTING HAS TO BE CONSISTENT WITH GLOBAL DECLARATION!!)
  DateTime now = RTC.now();  //pulls setup() time so we have one file name per run in a day
  Y = now.year();
  M = now.month();
  D = now.day();
  // sprintf(ypodID, "YPOD%02X", YPODID);                                  //char array for podID
  sprintf(fileName, "%s_%04u_%02u_%02u.CSV", ypodID, Y, M, D);  //char array for fileName
  delay(100);
  file.open(fileName, O_CREAT | O_APPEND | O_WRITE);  //open with create, append, write permissions
  file.close();                                       //close file, we opened so loop() is faster
  digitalWrite(SD_CS, HIGH);                          //release chip select on SD - allow other comm with SPI
  digitalWrite(G_LED, LOW);                           //turn off green LED (file is closed)
}  //void setup()

void loop() {
  digitalWrite(G_LED, LOW);
  bool pm_returned = false;
  double bme_temperature = -99;
  double bme_pressure = -99;
  float sht_temperature = 0;
  float sht_humidity = 0;

#if PMS_ENABLED
  pms.clearInput();
  pms.requestRead();
  if (pms.readUntil(pms_data)) {
    pm_returned = true;
  } else {
    pm_returned = false;
  }  //if (pms.readUntil(pms_data))
  delay(100);
#endif

#if QUAD_ENABLED
  qs_data = quad_module.return_data();
#endif

#if SHT25
  const byte temp_command = B11100011;
  const byte hum_command = B11100101;
  temperature_board = read_wire(temp_command);
  humidity_board = read_wire(hum_command);
  sht_humidity = ((125 * (float)humidity_board) / (65536)) - 6.00;
  sht_temperature = ((175.72 * (float)temperature_board) / (65536)) - 46.85;
  delay(100);
#endif  //SHT25

#if BME180
  //Get BMP data
  char status;
  status = BMP.startTemperature();
  if (status != 0) {
    //Serial.println(status);
    delay(status);
    status = BMP.getTemperature(bme_temperature);
    status = BMP.startPressure(3);
    if (status != 0) {
      delay(status);
      status = BMP.getPressure(bme_pressure, bme_temperature);
    } else {
      //if good temp; but can't compute P
      bme_pressure = -99;
    }  //if (status != 0)
  } else {
    //if bad temp; then can't compute temp or pressure
    bme_temperature = -99;
    bme_pressure = -99;
  }  //if (status != 0) outer loop?
  delay(100);
#endif  //BME180

  float CO2 = getS300CO2();
  delay(100);

  ads_data = ads_module.return_updated();

  //open SPI SD
  digitalWrite(SD_CS, LOW);

  while (!sd.begin(SD_CS)) {
#if SERIAL_ENABLED
    Serial.println("error in loop");
#endif  //SERIAL_ENABLED
    sd.begin(SD_CS);
  }

  DateTime now = RTC.now();
  Y = now.year();
  M = now.month();
  D = now.day();
  h = now.hour();
  m = now.minute();
  s = now.second();
  delay(100);
  sprintf(bufftime, "%04u-%02u-%02uT%02u:%02u:%02u", Y, M, D, h, m, s);

  if (sd.begin(SD_CS)) {
    // FILE FORMAT = RETIGO
    // delay(100);
    file.open(fileName, O_CREAT | O_APPEND | O_WRITE);
    delay(100);
    if (file.isOpen()) {
      digitalWrite(G_LED, HIGH);
      printDataRow(file, pm_returned, bme_temperature, bme_pressure, sht_temperature, sht_humidity, CO2);
      delay(100);
      file.sync();
      file.close();
      // delay(100);
    } else {
      Serial.println("file not opening?");
      file.close();
    }
  }  //if (sd.begin())

  digitalWrite(SD_CS, HIGH);
  digitalWrite(G_LED, LOW);

  //NOW ECHO TO SERIAL
  now = RTC.now();
#if SERIAL_ENABLED
  printDataRow(Serial, pm_returned, bme_temperature, bme_pressure, sht_temperature, sht_humidity, CO2);
  Serial.flush();
#endif  //SERIAL_ENABLED
}

void printDataRow(Print &out, bool pm_returned, double bme_temperature, double bme_pressure, float sht_temperature, float sht_humidity, float co2) {
  out.print(bufftime);
  out.print(F(",,,"));
  out.print(ypodID);
  out.print(F(","));
  out.print(firmwareVersion);
  out.print(F(","));

#if BME180
  out.print(bme_temperature);
  out.print(F(","));
  out.print(bme_pressure);
  out.print(F(","));
#else
  out.print(F(",,"));
#endif  //BME180

#if CALIBRATE
  calOutput calibrated = cal.calibrate(ads_data.CO_ch1, co2, sht_humidity, sht_temperature, ads_data.Fig1, ads_data.Fig2);
#endif  //CALIBRATE

#if SHT25
#if CALIBRATE
  out.print(calibrated.T_);
  out.print(F(","));
  out.print(calibrated.RH_);
#else
  out.print(sht_temperature);
  out.print(F(","));
  out.print(sht_humidity);
#endif  //CALIBRATE
  out.print(F(","));
#else
  out.print(F(",,"));
#endif  //SHT25

#if CALIBRATE
  out.print(calibrated.TVOC_);
#endif
  out.print(F(","));
  out.print(ads_data.Fig1);
  out.print(F(","));
  out.print(ads_data.Fig2);
  out.print(F(","));

#if MISC2611
  out.print(ads_data.e2V);
#endif
  out.print(F(","));

#if CALIBRATE
  out.print(calibrated.CO_);
#endif
  out.print(F(","));
  out.print(ads_data.CO_ch1);
  out.print(F(","));
  out.print(ads_data.CO_ch2);
  out.print(F(","));

#if CALIBRATE
  out.print(calibrated.CO2_);
#else
  out.print(co2);
#endif
  out.print(F(","));

  printPMS(out, pm_returned);
  printQuad(out);
  out.println();
}

void printPMS(Print &out, bool pm_returned) {
#if PMS_ENABLED
  if (pm_returned) {
    out.print(pms_data.pm10_env);
    out.print(F(","));
    out.print(pms_data.pm25_env);
    out.print(F(","));
    out.print(pms_data.pm100_env);
    out.print(F(","));
#if INCLUDE_STANDARD
    out.print(pms_data.pm10_standard);
    out.print(F(","));
    out.print(pms_data.pm25_standard);
    out.print(F(","));
    out.print(pms_data.pm100_standard);
    out.print(F(","));
#else
    out.print(F(",,,"));
#endif  //INCLUDE_STANDARD
#if INCLUDE_PARTICLES
    if (pms_data.hasParticles) {
      out.print(pms_data.particles_03um);
      out.print(F(","));
      out.print(pms_data.particles_05um);
      out.print(F(","));
      out.print(pms_data.particles_10um);
      out.print(F(","));
      out.print(pms_data.particles_25um);
      out.print(F(","));
      out.print(pms_data.particles_50um);
      out.print(F(","));
      out.print(pms_data.particles_100um);
      out.print(F(","));
    } else {
      out.print(F(",,,,,,"));
    }
#else
    out.print(F(",,,,,,"));
#endif  //INCLUDE_PARTICLES
  } else {
    out.print(F(",,,,,,,,,,,,"));
  }
#else
  out.print(F(",,,,,,,,,,,,"));
#endif  //PMS_ENABLED
}

void printQuad(Print &out) {
#if QUAD_ENABLED
  out.print(qs_data.a1C1);
  out.print(F(","));
  out.print(qs_data.a1C2);
  out.print(F(","));
  out.print(qs_data.a2C1);
  out.print(F(","));
  out.print(qs_data.a2C2);
  out.print(F(","));
  out.print(qs_data.a3C1);
  out.print(F(","));
  out.print(qs_data.a3C2);
  out.print(F(","));
  out.print(qs_data.a4C1);
  out.print(F(","));
  out.print(qs_data.a4C2);
  out.print(F(","));
#endif  //QUAD_ENABLED
}


float getS300CO2() {
  int i = 1;
  long reading;
  //float CO2val;
  wire_setup(0x31, 0x52, 7);

  while (Wire.available()) {
    byte val = Wire.read();
    if (i == 2) {
      reading = val;
      reading = reading << 8;
    }
    if (i == 3) {
      reading = reading | val;
    }
    i = i + 1;
  }

  //Shift Calculation to Atheros
  //    CO2val = reading / 4095.0 * 5000.0;
  //    CO2val = reading;
  return reading;
}

void wire_setup(int address, byte cmd, int from) {
  Wire.beginTransmission(address);
  Wire.write(cmd);
  Wire.endTransmission();
  Wire.requestFrom(address, from);
}

unsigned int read_wire(byte cmd) {
  const int SHT2x_address = 64;
  const byte mask = B11111100;
  byte byte1, byte2, byte3;

  wire_setup(SHT2x_address, cmd, 3);

  byte1 = Wire.read();
  byte2 = Wire.read();
  byte3 = Wire.read();

  //HUM_byte1 shifted left by 1 byte, (|) bitwise inclusize OR operator
  return ((byte1 << 8) | (byte2)&mask);
}
