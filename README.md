# YPOD
The YPOD is a low-cost air quality monitor that we use for outreach in Project-Based Learning in Rural Schools at CU Boulder.

# Using this Firmware
To utilize the most recent firmware, you need to:
1. Move all un-zipped libraries into your your Documents/Arduino/libraries folder on your computer
	SdFat@2.2.3
	RTClib@2.1.4
	Adafruit_BusIO@1.16.1 (Adafruit_ADS1X15.h)
	Adafruit_ADS1X15@2.5.0 (ads_module.h)
	MCP342x@1.0.4 (quad_module.h)
2. Ensure that your YPOD_Vx.x.x folder includes:
	YPOD_Vx.x.x.ino
	YPOD_node.h
	ads_module.cpp
	ads_module.h
	plantower_module.cpp OR PMS.cpp
	plantower_module.h OR PMS.h
	quad_module.cpp
	quad_module.h
	SFE_BMP180.cpp
	SFE_BMP180.h

# For Live Visualization
MATLAB Live Data Visualization firmware linked here --> https://github.com/HanniganAirQuality/YPOD_LiveDataViz
MUST USE V3.5.1 (pre 01/20/2026 FW upload) OR V4.0.1 (post 01/20/2026 FW upload)

# Update Tracker
Thanks to Izzy for this suggestion! Here we will be tracking each version of the firmware.
| Version       | "Named" Ver.   | Pilot         | Date               | Description & Purpose                  |
| ------------- | -------------- | ------------- | -------------      | -------------------------------------- |
| V3.1.2        | Library Fix    | Percy         | March 19, 2024     | Corrects RTClib Version                |
| V3.2.0        | Rewrite 1      | Percy         | June 28, 2024      | Moves RTC firmware to central firmware |
|               |                |               |                    | Removes bug in SD write with pull LOW  |
|               |                |               |                    | Removes GPS code (unusable due to HW)  |
| V3.4.3        | RETIGO Rewrite | Percy         | July 08, 2024      | Updated all libraries		           |
|               |                |               |                    | Decreased memory/RAM usage 	           |
|               |                |               |                    | Rewrote file naming more robust	       |
|               |                |               |                    | Made RTC timestamps (not sep anymore)  |
|               |                |               |                    | Moved clunky functions to .h/.cpp      |
|               |                |               |                    | Added YPOD_node.h for setting/config   |
| V3.4.4        | RETIGO LiveVis | Percy         | September 9, 2024  | Changes how serial montior writes      |
|               |                |               |                    | Works with MATLAB Live Vis V3+         |
| V3.4.5        | RTC Config Set | Percy         | September 20, 2024 | Adds a RTC_ADJUST config var           |
|				|		 		 |               |					  |                                        |
| V3.5.0        | Embedded Cal   | Chiara        | August 4, 2025     | Adds embedded calibration equations    |
|				|		 		 |               |                    | CO, CO2, Temperature and RH eqns       |
| V3.5.1		| VOC Cal		 | Chiara        | September 20, 2025 | Adds co, co2, t, rh cals for some pods |
|				|				 |				 |					  | VOC cal for TVOC, negative val fix     |
|				|				 |				 |					  | CO in PPM, changed to float type	   |
| V4.0.0		| PMS5003 Script | Percy         | January 20, 2026   | Updates PM --> XPOD PM to fix timeout & repeats |
| V4.0.1		| PM Error Fix   | Percy         | January 20, 2026   | Replaces text error for MATLAB LV |
