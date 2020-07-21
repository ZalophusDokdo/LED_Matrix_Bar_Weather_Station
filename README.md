# ESP8266 LED Matrix Bar Weather Station
ESP8266 LED Matrix Bar Weather Station and Clock Massenger

Configuring the upload environment
==================================
How to configure standard Arduino IDE to use as Arduino ESP8266 IDE.
First download Arduino IDE and install it on computer.
IMPORTANT: Download old version Arduino IDE Version 1.8.1.
(Tested Version 1.8.9 and Version 1.8.13)

Open Arduino IDE and go to File - Preferences.
Enter http://arduino.esp8266.com/stable/package_esp8266com_index.json under Adittional boards manager.

Select Boards Manager in Tools - Board
Find ESP8266 and press Install
IMPORTANT: Download old version esp8266 by ESP8266 Community Version 2.3.0.

IMPORTANT: Additional Libraries
go to Sketch - Include Library - Manage Libraries
Find and Install
- WiFiManager by tzapu,tablatronix Version 0.15.0
- DHT Sensor library by Adafruit Version 1.3.0
- ArduinoJson by Benoit Blanchon Version 5.11.1

After install select ESP9266 board.
- Board: NodeMCU 1.0 (ESP-12E Module)
- Upload Speed: "115200"
- CPU Frequency: "80 MHz"
- Flash Size: "4M (3M SPIFFS)"
- Port: COM?? 
