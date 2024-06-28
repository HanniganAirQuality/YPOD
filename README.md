# YPOD
The YPOD is a low-cost air quality monitor that we use for outreach in Project-Based Learning in Rural Schools at CU Boulder.

# Using this Firmware
To utilize the YPOD_V3.2.0 firmware, you need to:
1. Unzip the libraries.zip
2. Move all un-zipped libraries into your your Documents/Arduino/libraries folder on your computer
3. Ensure that your YPOD_V3.2.0 folder includes:
     YPOD_V3.2.0.ino
     RTClib.h
     RTClib.cpp
     RTC_DS3231.h
     RTC_DS3231.cpp

# Update Tracker
Thanks to Izzy for this suggestion! Here we will be tracking each version of the firmware.
| Version       | Pilot         | Date               | Description & Purpose                  |
| ------------- | ------------- | -------------      | -------------------------------------- |
| V3.1.2        | Percy         | March 19, 2024     | Corrects RTClib Version                |
| V3.2.0        | Percy         | June 28, 2024      | Moves RTC firmware to central firmware |
|               |               |                    | Removes bug in SD write with pull LOW  |
|               |               |                    | Removes GPS code (unusable due to HW)  |
