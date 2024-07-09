# YPOD
The YPOD is a low-cost air quality monitor that we use for outreach in Project-Based Learning in Rural Schools at CU Boulder.

# Using this Firmware
To utilize the YPOD_V3.4.3 firmware, you need to:
1. Move all un-zipped libraries into your your Documents/Arduino/libraries folder on your computer
	SdFat@2.2.3
	RTClib@2.1.4
	Adafruit_BusIO@1.16.1 (Adafruit_ADS1X15.h)
	Adafruit_ADS1X15@2.5.0 (ads_module.h)
	MCP342x@1.0.4 (quad_module.h)
2. Ensure that your YPOD_V3.2.0 folder includes:
	YPOD_V3.4.3.ino
	YPOD_node.h
	ads_module.cpp
	ads_module.h
	plantower_module.cpp
	plantower_module.h
	quad_module.cpp
	quad_module.h
	SFE_BMP180.cpp
	SFE_BMP180.h

# Update Tracker
Thanks to Izzy for this suggestion! Here we will be tracking each version of the firmware.
| Version       | "Named" Ver.   | Pilot         | Date               | Description & Purpose                  |
| ------------- | -------------- | ------------- | -------------      | -------------------------------------- |
| V3.1.2        | Library Fix    | Percy         | March 19, 2024     | Corrects RTClib Version                |
| V3.2.0        | Rewrite 1      | Percy         | June 28, 2024      | Moves RTC firmware to central firmware |
|               |                |               |                    | Removes bug in SD write with pull LOW  |
|               |                |               |                    | Removes GPS code (unusable due to HW)  |
| V3.4.3        | RETIGO Rewrite | Percy         | July 08, 2024      | Updated all libraries		       |
|               |                |               |                    | Decreased memory/RAM usage 	       |
|               |                |               |                    | Rewrote file naming more robust	       |
|               |                |               |                    | Made RTC timestamps (not sep anymore)  |
|               |                |               |                    | Moved clunky functions to .h/.cpp      |
|               |                |               |                    | Added YPOD_node.h for setting/config   |

