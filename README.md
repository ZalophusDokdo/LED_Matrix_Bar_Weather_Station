# ESP8266 LED Matrix Bar Weather Station (Version 1.0.4)
LED Matrix Bar Weather Station and Clock Massenger
<p align="center">
	<a href="https://github.com/ZalophusDokdo/LED_Matrix_Bar_Weather_Station/blob/master/images/LEDMatrixBarWeatherStation-V2_01.png?raw=true">
    <img src="https://github.com/ZalophusDokdo/LED_Matrix_Bar_Weather_Station/blob/master/images/LEDMatrixBarWeatherStation-V2_01.png?raw=true" width="100%">
  </a>
</p>
<p align="center">
	<a href="https://github.com/ZalophusDokdo/LED_Matrix_Bar_Weather_Station/blob/master/images/LEDMatrixBarWeatherStation-V2_02.png?raw=true">
    <img src="https://github.com/ZalophusDokdo/LED_Matrix_Bar_Weather_Station/blob/master/images/LEDMatrixBarWeatherStation-V2_02.png?raw=true" width="100%">
  </a>
</p>
<p align="center">
	<a href="https://github.com/ZalophusDokdo/LED_Matrix_Bar_Weather_Station/blob/master/images/LEDMatrixBarWeatherStation-V2_03.png?raw=true">
    <img src="https://github.com/ZalophusDokdo/LED_Matrix_Bar_Weather_Station/blob/master/images/LEDMatrixBarWeatherStation-V2_03.png?raw=true" width="100%">
  </a>
</p>
<p align="center">
	<a href="https://github.com/ZalophusDokdo/LED_Matrix_Bar_Weather_Station/blob/master/images/LEDMatrixBarWeatherStation-V2_24.png?raw=true">
    <img src="https://github.com/ZalophusDokdo/LED_Matrix_Bar_Weather_Station/blob/master/images/LEDMatrixBarWeatherStation-V2_24.png?raw=true" width="100%">
  </a>
</p>

Firmware Version: V1.0.4b
=========================
- Change WebApp scroll control (2021/01/11)
- Added timer setting (2020/10/13)
- Added alarm message setting function:
  .................Set scroll message to alarm message (2020/09/13)
- Added alarm setting (2020/09/10)
- AM-PM Mode, 24-hour clock, 12-hour clock (2020/09/09)
- Google clock replaced with NTP clock (2020/09/07)
- Resolved a problem that initialized when receiving Google clock
- Added web page refresh time (2020/09/06)
- Added RGB LED (2020/09/05)
- Error: ets jan 8 2013,rst cause:2, boot mode:(3,6) (2020/09/05)
- Added MQTT - Adafruit IO MQTT + IFTTT + Google Assistant (2020/07/30)
- Update Count 7.5m to 60m (2020/07/22)
- Added CHAR_SHIFT_DELAY - Config (2020/07/22)
- Change DHT12 Library (2018/03/12)

Video:
-
- https://youtu.be/NvMaL1UP2G4 - v1.0.4b
- - https://youtu.be/KqBzKSkwXMY - RGB LED Lamp Testing
- - https://youtu.be/YhxY4NB5p2Y - Dual LED Matix Display Testing
- https://www.youtube.com/embed/51wum5p9660 - v1.0.1

Features:
-
- ESP8266 WeMos D1 mini module which I finally found good application for (now you can buy better cheap ESPs)
- 6 x LEDMatrix module with MAX7219 driver
- supplied directly from USB port with USB plug
- all data are synchronized every 7-8 minutes
- time and date are taken from NTP
- weather informations are grabbed from openweather.org JSON api
- no hardware RTC clock is necessary
+ internal temperature, humidity sensor (options - DS18B20(Internal), DHT22(Indoor), DHT12(Outdoor))
+ ESP8266 setup and control as a Wi-Fi Web Server (WiFiManager, OTA, mDNS)
+ receives a message input from a User Input page, then displays the message on a scrolling LED matrix display.

* The interface for all the controls is designed as a webapp for internet browser.

Fuctions:
-
- mDNS
- WiFi Manager
- Web Server (WebUI)
- LED Matrix Clock
- Open Weather Map Weather Forecast
- Indoor and Outdoor temperature and Humidity sensor monitor
- Scrolling message
- Scroll Push Switch
- Lamp Push Switch
- Alarm time setting (WebUI)
- Alarm On/Off and Switch
- Google Assistant: Adafruit IO MQTT + IFTTT

BOM
-
- 6 x MAX7219 8x8 Matrix LED
- 1 x WeMos D1 mini
- 1 x Interface shield
Additional Parts:
- 1 x DS18B20 temperature sensor (Option)
- 1 x DHT22 temperature and humidity sensor (Option)
- 1 x DHT12 temperature and humidity sensor (Option)
- N x WS2812B RGB LED (Option)
- 1 x Alarm On/Off button switch
- 1 x Lamp Push button switch
- 1 x Scroll Push button switch
- 1 x Power On/Off switch
- 1 x Buzzer

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
- Adafruit MQTT Client by Adafruit Version 1.3.0
- Adafruit_NeoPixel by Adafruit Version 1.1.2

--------------
After install select ESP8266 board.
- Board: WeMos D1 R2 & mini
- Upload Speed: "921600"
- CPU Frequency: "80 MHz"
- Flash Size: "4M (3M SPIFFS)"
- Port: COM?? 

---------------
Schematic is available here:
- Normal: https://easyeda.com/neoseven/led-matrix-bar-weather-station-normal
- Expansion: https://easyeda.com/neoseven/led-matrix-bar-weather-station (Unfinished - Future plans)
<p align="center">
  <a href="https://image.easyeda.com/histories/2f5152c7b3b54089b06c75a85e043b24.png">
    <img src="https://image.easyeda.com/histories/2f5152c7b3b54089b06c75a85e043b24.png" width="100%">
  </a>
  <br>
  <a href="https://image.easyeda.com/histories/966b60a72ef94350a83fd2987f659129.png">
    <img src="https://image.easyeda.com/histories/966b60a72ef94350a83fd2987f659129.png" width="100%">
  </a>
</p>

---------------
3D Parts:
 - V2: http://shop.bluecomtech.com/ko_KR/product/led-matrix-bar-weather-station-v2-case-stnad
 - V3: http://shop.bluecomtech.com/ko_KR/product/led-matrix-bar-weather-station-v3-case-stnad
 - Pinshape : https://pinshape.com/items/46659-3d-printed-led-matrix-bar-weather-station-v1
 - Thingiverse : https://www.thingiverse.com/thing:2650808

---------------
WebApp:
<p align="center">
  <a href="https://github.com/ZalophusDokdo/LED_Matrix_Bar_Weather_Station/blob/master/images/screencapture-ledMatrixbarweatherstation-pc-2021-06-25-20_23_22.png">
    <img src="https://github.com/ZalophusDokdo/LED_Matrix_Bar_Weather_Station/blob/master/images/screencapture-ledMatrixbarweatherstation-pc-2021-06-25-20_23_22.png" width="100%">
  </a>
  <br>
  <a href="https://github.com/ZalophusDokdo/LED_Matrix_Bar_Weather_Station/blob/master/images/screencapture-ledmatrixbarweatherstation-mobile-2021-06-25-20_04_00.png">
    <img src="https://github.com/ZalophusDokdo/LED_Matrix_Bar_Weather_Station/blob/master/images/screencapture-ledmatrixbarweatherstation-mobile-2021-06-25-20_04_00.png" width="30%">
  </a>
</p>

