/*********************************************************************************
 * @project HAQ Lab YPOD
 *
 * @file    YPOD_V3.5.0.ino
 * @author  Percy Smith
 * @author  Chiara Pesce
 * @brief   Central firmware to collect data through the YPOD
 * 
 * @date    September 30, 2025
 * @version V3.5.1
 * @log     Works with the live data visualization tool
 *          Changes the serial logging mech & decreases the delays\
 *          Added RTC adjust setting to reset DT
***********************************************************************************/
/*  Libraries  */
#include <Arduino.h>          
#include <SPI.h>              //P - last tested with "SPI@1.0"      
#include <Wire.h>             //P - last tested with "Wire@1.0"
#include <SoftwareSerial.h>   //P - last tested with "SoftwareSerial@1.0"
#include <SdFat.h>            //P - last tested with "SdFat@2.2.3"
#include <RTClib.h>           //P - last tested with "RTClib@2.1.4"
/*  Header Files  */
#include "YPOD_node.h"
#include "ads_module.h"       //P - last tested with "Adafruit ADS1X15@2.5.0" & "Adafruit BusIO@1.16.1"

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
//PMS5003 - PM 2.5 - Plantower
#if PMS_ENABLED
  #include "plantower_module.h"
  PMS5003 pms5003;
  pms5003data pmdata;
  SoftwareSerial pmsSerial(PM_RX, PM_TX);
#endif //PMS_ENABLED
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
  int Y,M,D,h,m,s;
  
/***************************************************************************************/
void setup() 
{
  /*  Intializing Global Variables  */
  #if SERIAL_ENABLED
    Serial.begin(9600);
  #endif  //SERIAL_ENABLED
  #if PMS_ENABLED
    pmsSerial.begin(9600);
  #endif //PMS_ENABLED
  //Central Firmware (comms protocols)
  Wire.begin();
  SPI.begin();
  //Object Begins
  RTC.begin();            //Initialize RTC
  #if RTC_UPDATE
    RTC.adjust(DateTime(F(__DATE__),F(__TIME__)));
  #endif //RTC_UPDATE
  ads_module.begin();     //Initialize ads_module (creates objects in .cpp)
  #if BME180
    BMP.begin();          //Initialize BME 180 (creates objects in .cpp)
  #endif  //BME180
  //Initialize Pins - Establish direction of pin comms
  pinMode(SD_CS, OUTPUT);
  pinMode(G_LED, OUTPUT);  

  /*  SD Card & File Setup  */
  digitalWrite(SD_CS, LOW);       //Pull SD_CS pin LOW to initialize SPI comms
  sd.begin(SD_CS);                //Initialize SD Card with relevant chip select pin
  // Establish contact with SD card - if initialization fails, run until success
  while (!sd.begin(SD_CS)) 
  {
    #if SERIAL_ENABLED
        Serial.println("insert sd card to begin");
    #endif  //SERIAL_ENABLED
    sd.begin(SD_CS);      //attempt to initialize again
  } //while(!sd.begin(SD_CS))
  digitalWrite(G_LED, HIGH);      //if we exit the while loop, blink green LED once to indicate success
  //File Naming (FORMATTING HAS TO BE CONSISTENT WITH GLOBAL DECLARATION!!)
  DateTime now = RTC.now();     //pulls setup() time so we have one file name per run in a day
    Y = now.year();
    M = now.month();
    D = now.day();
  // sprintf(ypodID, "YPOD%02X", YPODID);                                  //char array for podID
  sprintf(fileName, "%s_%04u_%02u_%02u.CSV", ypodID, Y, M, D);    //char array for fileName
  delay(100);   
  file.open(fileName, O_CREAT | O_APPEND | O_WRITE);  //open with create, append, write permissions
  file.close();                                       //close file, we opened so loop() is faster 
  digitalWrite(SD_CS, HIGH);    //release chip select on SD - allow other comm with SPI
  digitalWrite(G_LED, LOW);     //turn off green LED (file is closed)
} //void setup() 

/***************************************************************************************/
void loop() 
{
  digitalWrite(G_LED, LOW);
  #if PMS_ENABLED
  //poll for plantower data first cause it has to run a while loop and i'm paranoid !!
  bool pm_available = 0;
  while(!pm_available)
  {
    pm_available = pms5003.readPMSdata(&pmsSerial);
    delay(100);
    if(pm_available) { pmdata = pms5003.returnPMdataset(); }
  } //while(!pm_available)
  delay(100);
  #endif //PMS_ENABLED

  #if QUAD_ENABLED
    qs_data = quad_module.return_data();
  #endif

  delay(100);
  #if SHT25
    const byte temp_command = B11100011;
    const byte hum_command = B11100101;
    temperature_board = read_wire(temp_command);
    humidity_board = read_wire(hum_command);
    float humidity_SHT25 = ((125 * (float)humidity_board) / (65536)) - 6.00;
    float temperature_SHT25 = ((175.72 * (float)temperature_board) / (65536)) - 46.85;
  #endif  //SHT25
  delay(100);

  #if BME180
  //Get BMP data
  double T, P;
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
    } //if (status != 0)
  } else {
    //if bad temp; then can't compute temp or pressure
    T = -99;
    P = -99;
  } //if (status != 0) outer loop?
  #endif  //BME180
  delay(100);

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

  delay(100);
  DateTime now = RTC.now();
  Y = now.year();  M = now.month();  D = now.day();  h = now.hour();  m = now.minute();  s = now.second();
  sprintf(bufftime, "%04u-%02u-%02uT%02u:%02u:%02u", Y, M, D, h, m, s);

  if (sd.begin(SD_CS)) {
    // FILE FORMAT = RETIGO
    delay(100);
    file.open(fileName, O_CREAT | O_APPEND | O_WRITE); 
    delay(100);
    if(file.isOpen())
    {
      digitalWrite(G_LED, HIGH);
      // file RTC Writing
      file.print(bufftime);
      delay(100);
      file.print(",");
      //GPS EAST_LONGITUDE
      file.print(",");
      //GPS NORTH LATITUDE
      file.print(",");
      file.print(ypodID);
      file.print(",");
      delay(100);

      #if BME180
        file.print(T);
        file.print(",");
        file.print(P);
        file.print(",");
      #endif  //BME180
      delay(100);

      #if SHT25
        // Temeprature
        #if CALIBRATE // Calls calibration eqn for temperature
          float tempt = cal.calibrate(ads_data.CO, CO2, humidity_SHT25, temperature_SHT25, ads_data.Fig1, ads_data.Fig2).T_; // Temp variable to store object
          file.print(tempt);
        #else
          file.print(temperature_SHT25); // Default - no calibration
        #endif
        delay(100);
        file.print(",");
        // Relative humidity
        #if CALIBRATE // Calls calibration eqn for relative humidity
          float temprh = cal.calibrate(ads_data.CO, CO2, humidity_SHT25, temperature_SHT25, ads_data.Fig1, ads_data.Fig2).RH_; // Temp varibale to store object
          file.print(temprh);
        #else
          file.print(humidity_SHT25); // Default - no calibration
        #endif
        delay(100);
        file.print(",");
      #endif  //SHT25

      // Figs
      #if CALIBRATE // calls calibration eqn for voc
        int tvoc = cal.calibrate(ads_data.CO, CO2, humidity_SHT25, temperature_SHT25, ads_data.Fig1, ads_data.Fig2).TVOC_; // Temp varibale to store object
        file.print(tvoc);
      #endif 
      file.print(",");
      delay(100);
      file.print(ads_data.Fig1); //Right slot - 2600
      file.print(",");
      delay(100);
      file.print(ads_data.Fig2); //Left slot - 2602
      file.print(",");
      delay(100);

      // Ozone
      #if MISC2611 // Conditional for ozone sensor
        file.print(ads_data.e2V);
        file.print(",");
        delay(100);
      #endif

      // CO
      #if CALIBRATE // Calls calibraiton eqn for CO
        int tempco = cal.calibrate(ads_data.CO, CO2, humidity_SHT25, temperature_SHT25, ads_data.Fig1, ads_data.Fig2).CO_; // Temp variable to store object
        file.print(tempco); 
      #else
        file.print(ads_data.CO); // Default - no calibration
      #endif
      file.print(",");
      delay(100);

      // CO2
      #if CALIBRATE // Calls calibration eqn for CO2
        int tempco2 = cal.calibrate(ads_data.CO, CO2, humidity_SHT25, temperature_SHT25, ads_data.Fig1, ads_data.Fig2).CO2_; // Temp variable to store object
        file.print(tempco2);
      #else
        file.print(CO2); // Default - no calibration
      #endif
      delay(100);
      file.print(",");

      #if PMS_ENABLED
      file.print(pmdata.pm10_env);
      file.print(",");
      delay(100);
      file.print(pmdata.pm25_env);
      file.print(",");
      delay(100);
      file.print(pmdata.pm100_env);
      file.print(",");
      delay(100);
        #if INCLUDE_STANDARD
          file.print(pmdata.pm10_standard);
          file.print(",");
          file.print(pmdata.pm25_standard);
          file.print(",");
          file.print(pmdata.pm100_standard);
          file.print(",");
        #endif //INCLUDE_STANDARD
        delay(100);
        #if INCLUDE_PARTICLES
          file.print(pmdata.particles_03um);
          file.print(",");
          file.print(pmdata.particles_05um);
          file.print(",");
          file.print(pmdata.particles_10um);
          file.print(",");
          file.print(pmdata.particles_25um);
          file.print(",");
          file.print(pmdata.particles_50um);
          file.print(",");
          file.print(pmdata.particles_100um);
          file.print(",");
        #endif  //INCLUDE_PARTICLES
        delay(100);
      #endif //PMS_ENABLED
      #if QUAD_ENABLED
        file.print(qs_data.a1C1);
        file.print(",");
        file.print(qs_data.a1C2);
        file.print(",");
        file.print(qs_data.a2C1);
        file.print(",");
        file.print(qs_data.a2C2);
        file.print(",");
        file.print(qs_data.a3C1);
        file.print(",");
        file.print(qs_data.a3C2);
        file.print(",");
        file.print(qs_data.a4C1);
        file.print(",");
        file.print(qs_data.a4C2);
        file.print(",");
      #endif
      file.print("\n");
      delay(100);
      file.sync();
      file.close();
      delay(100);
    }    else    {
      Serial.println("file not opening?");
      file.close();
    }
  } //if (sd.begin())

  digitalWrite(SD_CS, HIGH);
  digitalWrite(G_LED, LOW);

  //NOW ECHO TO SERIAL
  now = RTC.now();
  #if SERIAL_ENABLED
    // Serial.print("\n");
    Serial.print(bufftime);
    Serial.print(",");
    delay(100);
    // YPOD ID
    // Serial.print(ypodID);
    // Serial.print(",");
    // delay(100);
    //T, H, T, P
    #if BME180
      Serial.print(T);
      Serial.print(",");
      Serial.print(P);
      Serial.print(",");
      delay(100);
    #endif  //BME180

    #if SHT25
      // Temperature
      #if CALIBRATE // Calls calibration eqn for temperature
        float tempt = cal.calibrate(ads_data.CO, CO2, humidity_SHT25, temperature_SHT25, ads_data.Fig1, ads_data.Fig2).T_; // Temp variable to store object
        Serial.print(tempt);
      #else
        Serial.print(temperature_SHT25); // Default - no calibration
      #endif
      Serial.print(",");
      // Relative humidity
      #if CALIBRATE // Calls calibration eqn for relative humidity
        float temprh = cal.calibrate(ads_data.CO, CO2, humidity_SHT25, temperature_SHT25, ads_data.Fig1, ads_data.Fig2).RH_; // Temp varibale to store object
        Serial.print(temprh);
      #else
        Serial.print(humidity_SHT25); // Default - no calibration
      #endif
      Serial.print(",");
      delay(100);
    #endif  //SHT25

    //Figs
    #if CALIBRATE // Calls calibration eqn for tvoc
      int tvoc = cal.calibrate(ads_data.CO, CO2, humidity_SHT25, temperature_SHT25, ads_data.Fig1, ads_data.Fig2).TVOC_; // Temp varibale to store object
      Serial.print(tvoc);
    #endif
    Serial.print(",");
    delay(100);
    Serial.print(ads_data.Fig1);
    Serial.print(",");
    delay(100);
    Serial.print(ads_data.Fig2);
    Serial.print(",");
    delay(100);

    // Ozone
    #if MISC2611
      Serial.print(ads_data.e2V);
      Serial.print(",");
      delay(100);
    #endif

    // CO
    #if CALIBRATE // Calls calibraiton eqn for CO
      int tempco = cal.calibrate(ads_data.CO, CO2, humidity_SHT25, temperature_SHT25, ads_data.Fig1, ads_data.Fig2).CO_; // Temp variable to store object
      Serial.print(tempco); 
    #else
      Serial.print(ads_data.CO); // Default - no calibration
    #endif
    Serial.print(",");
    delay(100);

    // CO2
    #if CALIBRATE // Calls calibration eqn for CO2
      int tempco2 = cal.calibrate(ads_data.CO, CO2, humidity_SHT25, temperature_SHT25, ads_data.Fig1, ads_data.Fig2).CO2_; // Temp variable to store object
      Serial.print(tempco2);
    #else
      Serial.print(CO2); // Default - no calibration
    #endif
    Serial.print(",");
    delay(100);

    #if PMS_ENABLED
      Serial.print(pmdata.pm10_env);
      Serial.print(",");
      Serial.print(pmdata.pm25_env);
      Serial.print(",");
      Serial.print(pmdata.pm100_env);
      Serial.print(",");
      delay(100);
      #if INCLUDE_STANDARD
        Serial.print(pmdata.pm10_standard);
        Serial.print(",");
        Serial.print(pmdata.pm25_standard);
        Serial.print(",");
        Serial.print(pmdata.pm100_standard);
        Serial.print(",");
      #endif //INCLUDE_STANDARD
      delay(100);
      #if INCLUDE_PARTICLES
        Serial.print(pmdata.particles_03um);
        Serial.print(",");
        Serial.print(pmdata.particles_05um);
        Serial.print(",");
        Serial.print(pmdata.particles_10um);
        Serial.print(",");
        Serial.print(pmdata.particles_25um);
        Serial.print(",");
        Serial.print(pmdata.particles_50um);
        Serial.print(",");
        Serial.print(pmdata.particles_100um);
        Serial.print(",");
      #endif //INCLUDE_PARTICLES
      delay(200);
    #endif //PMS_ENABLED
    #if QUAD_ENABLED
      Serial.print(qs_data.a1C1);
      Serial.print(",");
      Serial.print(qs_data.a1C2);
      Serial.print(",");
      Serial.print(qs_data.a2C1);
      Serial.print(",");
      Serial.print(qs_data.a2C2);
      Serial.print(",");
      Serial.print(qs_data.a3C1);
      Serial.print(",");
      Serial.print(qs_data.a3C2);
      Serial.print(",");
      Serial.print(qs_data.a4C1);
      Serial.print(",");
      Serial.print(qs_data.a4C2);
      Serial.print(",");
    #endif //QS_ENABLED
    Serial.write(13);
    Serial.write(10);
  #endif  //SERIAL_ENABLED
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