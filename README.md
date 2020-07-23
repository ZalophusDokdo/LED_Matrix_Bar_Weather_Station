# ESP8266 LED Matrix Bar Weather Station
ESP8266 LED Matrix Bar Weather Station and Clock Massenger

Features:
-
- ESP8266 WeMos D1 mini module which I finally found good application for (now you can buy better cheap ESPs)
- 6 x LEDMatrix module with MAX7219 driver
- supplied directly from USB port with USB plug
- all data are synchronized every 7-8 minutes
- time and date are taken from google.com
- weather informations are grabbed from openweather.org JSON api
- no hardware RTC clock is necessary
+ internal temperature, humidity sensor (options - DS18B20(Internal), DHT22(Indoor), DHT12(Outdoor))
+ ESP8266 setup and control as a Wi-Fi Web Server (WiFiManager, OTA, mDNS)
+ receives a message input from a User Input page, then displays the message on a scrolling LED matrix display.

* The interface for all the controls is designed as a webapp for internet browser.

BOM
-
- 6 x MAX7219 8 x 8 LED Matrix module
- 1 x WeMos D1 mini
- 1 x Interface Shield

Configuring the upload environment
==================================
How to configure standard Arduino IDE to use as Arduino ESP8266 IDE.

First download Arduino IDE and install it on computer.

IMPORTANT: Download old version Arduino IDE Version 1.8.1.
(Tested Version 1.8.9 and Version 1.8.13)

------------
Open Arduino IDE and go to File - Preferences.

Enter http://arduino.esp8266.com/stable/package_esp8266com_index.json under Adittional boards manager.

------------
Select Boards Manager in Tools - Board

Find ESP8266 and press Install

IMPORTANT: Download old version esp8266 by ESP8266 Community Version 2.3.0.

------------
IMPORTANT: Additional Libraries

go to Sketch - Include Library - Manage Libraries

Find and Install
- WiFiManager by tzapu,tablatronix Version 0.15.0
- DHT Sensor library by Adafruit Version 1.3.0
- ArduinoJson by Benoit Blanchon Version 5.11.1

--------------
After install select ESP9266 board.
- Board: NodeMCU 1.0 (ESP-12E Module)
- Upload Speed: "115200"
- CPU Frequency: "80 MHz"
- Flash Size: "4M (3M SPIFFS)"
- Port: COM?? 

---------------
Schematic is available here:
- Normal: https://easyeda.com/neoseven/led-matrix-bar-weather-station-normal
- Expansion: https://easyeda.com/neoseven/led-matrix-bar-weather-station (Unfinished - Future plans)

---------------
3D Parts:
- Pinshape : https://pinshape.com/items/46659-3d-printed-led-matrix-bar-weather-station-v1
- Thingiverse : https://www.thingiverse.com/thing:2650808
