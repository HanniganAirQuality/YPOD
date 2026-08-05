/*********************************************************************************
 * @project HAQ Lab YPOD
 *
 * @file    YPOD_V4.2.2.ino
 * @author  Percy Smith, percy.smith@colorado.edu
            Chiara Pesce, chiara.pesce@colorado.edu
            Alex Hansen, alex.hansen@colorado.edu
 * @brief   Central firmware to collect data through the YPOD
 * 
 * @date    August 5, 2026
 * @version V4.2.2
 * @log     Calibration updates for E8 & D2 (CU Museum PODs)c
***********************************************************************************/
/*  Libraries  */
#include <Arduino.h>
#include <SPI.h>     //P - last tested with "SPI@1.0"
#include <Wire.h>    //P - last tested with "Wire@1.0"
#include <RTClib.h>  //P - last tested with "RTClib@2.1.4"
#include <string.h>
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
#if SD_ENABLED
  //SdFat (SD Card) & File file (file on SD)
  #include <SdFat.h>   //P - last tested with "SdFat@2.2.3"
  SdFat sd;
  File file;
  // Buffers
  // char ypodID[] = "YPODID";
  char fileName[] = "YPODID_YYYY_MM_DD.CSV";
#endif //SD_ENABLED
char firmwareFileName[32];
char bufftime[] = "YYYY-MM-DDThh:mm:ss";
int Y, M, D, h, m, s;

void printOutput(Print &output, bool pm_returned, double T, double P, float temperature_SHT25, float humidity_SHT25, float CO2);

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
  const char *sketchName = __FILE__;
  const char *slash = strrchr(__FILE__, '/');
  const char *backslash = strrchr(__FILE__, '\\');
  if (slash && slash > sketchName) {
    sketchName = slash + 1;
  }
  if (backslash && backslash > sketchName) {
    sketchName = backslash + 1;
  }
  strncpy(firmwareFileName, sketchName, sizeof(firmwareFileName) - 1);
  firmwareFileName[sizeof(firmwareFileName) - 1] = '\0';

  char *inoExtension = strstr(firmwareFileName, ".ino");
  if (inoExtension) {
    *inoExtension = '\0';
  }

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
  pinMode(G_LED, OUTPUT);

#if SD_ENABLED
  pinMode(SD_CS, OUTPUT);
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
  #else 
  DateTime now = RTC.now();
  #endif //SD_ENABLED
  digitalWrite(G_LED, LOW);                           //turn off green LED (file is closed)
}  //void setup()

void loop() {
  digitalWrite(G_LED, LOW);
  bool pm_returned = false;
  double T = -99;
  double P = -99;
  float temperature_SHT25 = 0;
  float humidity_SHT25 = 0;

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
  humidity_SHT25 = ((125 * (float)humidity_board) / (65536)) - 6.00;
  temperature_SHT25 = ((175.72 * (float)temperature_board) / (65536)) - 46.85;
  delay(100);
#endif  //SHT25

#if BME180
  //Get BMP data
  char status;
  status = BMP.startTemperature();
  if (status != 0) {
    //Serial.println(status);
    delay(status);
    status = BMP.getTemperature(T);
    status = BMP.startPressure(3);
    if (status != 0) {
      delay(status);
      status = BMP.getPressure(P, T);
    } else {
      //if good temp; but can't compute P
      P = -99;
    }  //if (status != 0)
  } else {
    //if bad temp; then can't compute temp or pressure
    T = -99;
    P = -99;
  }  //if (status != 0) outer loop?
  delay(100);
#endif  //BME180

  float CO2 = getS300CO2();
  delay(100);

  ads_data = ads_module.return_updated();

  //open SPI SD
  #if SD_ENABLED
  digitalWrite(SD_CS, LOW);

  while (!sd.begin(SD_CS)) {
#if SERIAL_ENABLED
    Serial.println("error in loop");
#endif  //SERIAL_ENABLED
    sd.begin(SD_CS);
  }
  #endif //SD_ENABLED

  DateTime now = RTC.now();
  Y = now.year();
  M = now.month();
  D = now.day();
  h = now.hour();
  m = now.minute();
  s = now.second();
  delay(100);
  sprintf(bufftime, "%04u-%02u-%02uT%02u:%02u:%02u", Y, M, D, h, m, s);
  #if SD_ENABLED
  if (sd.begin(SD_CS)) {
    // FILE FORMAT = RETIGO
    // delay(100);
    file.open(fileName, O_CREAT | O_APPEND | O_WRITE);
    delay(100);
    if (file.isOpen()) {
      digitalWrite(G_LED, HIGH);
      printOutput(file, pm_returned, T, P, temperature_SHT25, humidity_SHT25, CO2);
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
  #endif //SD_ENABLED

  //NOW ECHO TO SERIAL
  now = RTC.now();
#if SERIAL_ENABLED
  printOutput(Serial, pm_returned, T, P, temperature_SHT25, humidity_SHT25, CO2);
  Serial.flush();
#endif  //SERIAL_ENABLED
}

void printOutput(Print &output, bool pm_returned, double T, double P, float temperature_SHT25, float humidity_SHT25, float CO2) {
  // RTC, GPS blanks, YPOD ID, and firmware version
  output.print(bufftime);
  delay(100);
  output.print(",");
  
  output.print(",");  //GPS EAST_LONGITUDE
  output.print(",");  //GPS NORTH_LATITUDE

  output.print(ypodID);
  output.print(",");
  output.print(firmwareFileName);
  output.print(",");
  delay(100);

#if BME180
  output.print(T);
  output.print(",");
  output.print(P);
  output.print(",");
  delay(100);
#else
  output.print(",");
  output.print(",");
  delay(100);
#endif  //BME180

#if SHT25
// Temperature
#if CALIBRATE                                                                                                                 // Calls calibration eqn for temperature
  float tempt = cal.calibrate(ads_data.CO_ch1, CO2, humidity_SHT25, temperature_SHT25, ads_data.Fig1, ads_data.Fig2).T_;  // Temp variable to store object
  output.print(tempt);
#else
  output.print(temperature_SHT25);  // Default - no calibration
#endif
  delay(100);
  output.print(",");
// Relative humidity
#if CALIBRATE                                                                                                                   // Calls calibration eqn for relative humidity
  float temprh = cal.calibrate(ads_data.CO_ch1, CO2, humidity_SHT25, temperature_SHT25, ads_data.Fig1, ads_data.Fig2).RH_;  // Temp varibale to store object
  output.print(temprh);
#else
  output.print(humidity_SHT25);     // Default - no calibration
#endif
  delay(100);
  output.print(",");
#else
  output.print(",");
  output.print(",");
  delay(100);
#endif  //SHT25

// Figs
#if CALIBRATE                                                                                                                 // calls calibration eqn for voc
  int tvoc = cal.calibrate(ads_data.CO_ch1, CO2, humidity_SHT25, temperature_SHT25, ads_data.Fig1, ads_data.Fig2).TVOC_;  // Temp varibale to store object
  output.print(tvoc);
#endif
  output.print(",");
  output.print(ads_data.Fig1);  //Right slot - 2600
  output.print(",");
  output.print(ads_data.Fig2);  //Left slot - 2602
  output.print(",");
  delay(100);

// Ozone
#if MISC2611  // Conditional for ozone sensor
  output.print(ads_data.e2V);
#endif
  output.print(",");
  delay(100);

// CO
#if CALIBRATE                                                                                                                   // Calls calibraiton eqn for CO
  float tempco = cal.calibrate(ads_data.CO_ch1, CO2, humidity_SHT25, temperature_SHT25, ads_data.Fig1, ads_data.Fig2).CO_;  // Temp variable to store object
  output.print(tempco);
#endif
  output.print(",");
  output.print(ads_data.CO_ch1);
  output.print(",");  // Default - no calibration
  output.print(ads_data.CO_ch2);
  delay(100);
  output.print(",");

// CO2
#if CALIBRATE                                                                                                                   // Calls calibration eqn for CO2
  int tempco2 = cal.calibrate(ads_data.CO_ch1, CO2, humidity_SHT25, temperature_SHT25, ads_data.Fig1, ads_data.Fig2).CO2_;  // Temp variable to store object
  output.print(tempco2);
#else
  output.print(CO2);  // Default - no calibration
#endif
  delay(100);
  output.print(",");

#if PMS_ENABLED
  if (pm_returned) {
    output.print(pms_data.pm10_env);
    output.print(F(","));
    output.print(pms_data.pm25_env);
    output.print(F(","));
    output.print(pms_data.pm100_env);
    output.print(F(","));
  } else {
    output.print(F(",,,"));
  }  //if(pm_returned)
#else
  output.print(F(",,,"));
#endif  //PMS_ENABLED
#if QUAD_ENABLED
  output.print(qs_data.a1C1);
  output.print(",");
  output.print(qs_data.a1C2);
  output.print(",");
  output.print(qs_data.a2C1);
  output.print(",");
  output.print(qs_data.a2C2);
  output.print(",");
  output.print(qs_data.a3C1);
  output.print(",");
  output.print(qs_data.a3C2);
  output.print(",");
  output.print(qs_data.a4C1);
  output.print(",");
  output.print(qs_data.a4C2);
  output.print(",");
#endif
  output.print("\n");
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
