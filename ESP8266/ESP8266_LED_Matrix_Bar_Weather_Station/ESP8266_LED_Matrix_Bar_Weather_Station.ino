/* =========================================================================
 *  Author: Zalophus Dokdo (https://zddh.blogspot.com)
 *  Date: 31/08/2017       (https://zalophus.tistory.com/)
 *  License: GPL v2
 * =========================================================================
 *  LED Matrix Bar Weather Station V1.0.4 (Publish: 2018/01/02)
 * =========================================================================
 *  -> Added alarm message setting function:
 *     .................Set scroll message to alarm message (2020/09/13)
 *  -> AM-PM Mode, 24-hour clock, 12-hour clock (2020/09/09)
 *  -> Google clock replaced with NTP clock (2020/09/07)
 *  -> Resolved a problem that initialized when receiving Google clock
 *  -> Added web page refresh time (2020/09/06)
 *  -> Added RGB LED (2020/09/05)
 *  -> Error: ets jan 8 2013,rst cause:2, boot mode:(3,6) (2020/09/05)
 *  -> Added MQTT - Adafruit IO MQTT + IFTTT + Google Assistant (2020/07/30)
 *  -> Update Count 7.5m to 60m (2020/07/22)
 *  -> Added CHAR_SHIFT_DELAY - Config (2020/07/22)
 *  -> Change DHT12 Library (2018/03/12)
 * =========================================================================
 *  Parts List:
 *  - 6 x MAX7219 8x8 Matrix LED
 *  - 1 x WeMos D1 mini
 *  - 1 x Interface shield
 *  Additional Parts:
 *  - 1 x DS18B20 temperature sensor (Option)
 *  - 1 x DHT22 temperature and humidity sensor (Option)
 *  - 1 x DHT12 temperature and humidity sensor (Option)
 *  - N x WS2812B RGB LED (Option)
 *  - 1 x Alarm On/Off button switch
 *  - 1 x Lamp Push button switch
 *  - 1 x Scroll Push button switch
 *  - 1 x Power On/Off switch
 *  - 1 x Buzzer
 * -------------------------------------------------------------------------
 *  Fuctions:
 *  - OTA
 *  - mDNS
 *  - WiFi Manager
 *  - Web Server (WebUI)
 *  - LED Matrix Clock
 *  - Open Weather Map Weather Forecast
 *  - Indoor and Outdoor temperature and Humidity sensor monitor
 *  - Scrolling message
 *  - Scroll Push Switch
 *  - Lamp Push Switch
 *  - Alarm time setting (WebUI)
 *  - Alarm On/Off and Switch
 *  - Google Assistant: Adafruit IO MQTT + IFTTT
 * =========================================================================
 */

#include <FS.h>  // this needs to be first, or it all crashes and burns...

#include <ESP8266WiFi.h>
#include "Config.h"
#include "WiFi_Setup.h"
// Shields: ================================================================
#ifdef   USE_DS18B20
  #include "DS18B20.h"
#endif
#ifdef   USE_DHT
  #include "DHT.h"
#endif
#ifdef   USE_DHT12
  #include "DHT12.h"
  // Set dht12 i2c comunication on default Wire pin
  DHT12 dht12;
  int timeSinceLastRead   =  0;
  int dht12get            =  0;
#endif
#ifdef   USE_RGB_LED && RGB_LED_PIN
  #include "WS2812B.h"
#endif
// =========================================================================
#ifdef   USE_LEDMATRIX_CLOCKWEATHER
  #include "LEDMatrix_ClockWeather.h"
#endif
#ifdef   USE_WEB_SERVER
  #include "Web_Server.h"
#endif
#ifdef   USE_MQTT
  #include "MQTT.h"
#endif

int ScrollingMessage      =  0;
int weatherScrollInterval =  WEATHER_SCROLL_INTERVAL;

int    freq[] = {1047, 1175, 1319, 1397, 1568, 1760, 1976, 2093};
String note[] = {"C6", "D6", "E6", "F6", "G6", "A6", "B6", "C7"};

int    alarm_action          =  1;
String alarm_message         = "";

int    button16count         =  0;
int    button00count         =  0;
int    button12count         =  0;

void setup() {  // #########################################################
  // put your setup code here, to run once:
  Serial.begin(SERIAL_SPEED);
  Serial.println("LED Matrix Bar Weather Station");
  Serial.println();
  delay(10);
  pinMode(DHTPIN,       INPUT);  // TXD --> DHT22
  pinMode(ONE_WIRE_BUS, INPUT);  // DS18B20
  pinMode(RED_PIN,     OUTPUT);  // Alarm ON/OFF SW (Red)
  pinMode(GREEN_PIN,   OUTPUT);  // Lamp Push Button SW (Green)
  pinMode(BLUE_PIN,    OUTPUT);  // Weather Scroll Push Button SW (Blue)
  pinMode(WHITE_PIN,   OUTPUT);  // RXD --> Buzzer & LED
  digitalWrite(RED_PIN,   LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN,  LOW);
  digitalWrite(WHITE_PIN, LOW);

  WiFi_setup();

// Shields =================================================================
#ifdef   USE_DS18B20
  DS18B20_setup();
#endif
#ifdef   USE_DHT
  DHT22_setup();
#endif
#ifdef   USE_DHT12
  dht12.begin();
#endif
#ifdef   USE_RGB_LED && USE_RGB_LED_PIN
  RGB_LED_setup();
#endif
// =========================================================================
#ifdef   USE_LEDMATRIX_CLOCKWEATHER
  LEDMatrix_ClockWeather_setup();
#endif
#ifdef   USE_WEB_SERVER
  Webserver_setup();
#endif
#ifdef   USE_MQTT
  MQTT_setup();
#endif
}

void loop() {  // ##########################################################
ESP.wdtFeed(); // feeds the dog // Error: ets jan 8 2013,rst cause:2, boot mode:(3,6)
#ifdef   USE_OTA
  if (OTAmode == 1) {
    OTA_action();
  }
#endif
#ifdef   USE_WEB_SERVER
  Webserver_action();
#endif
#ifdef   USE_LEDMATRIX_CLOCKWEATHER
  if (ledMatrix == 1) {
    LEDMatrix_action();
  }
  if(millis() - clkTime > weatherScrollInterval && !del && dots) {  // clock for 15s, then scrolls for about 30s
#ifdef   USE_DS18B20
    DS18B20_action();
#endif
#ifdef   USE_DHT
    DHT22_action();
#endif
#ifdef   USE_DHT12
    // Report every 2 seconds.
    if(timeSinceLastRead > 2000) {
      // Read temperature as Celsius (the default)
      float t12 = dht12.readTemperature();
      // Read temperature as Fahrenheit (isFahrenheit = true)
      float f12 = dht12.readTemperature(true);
      // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
      float h12 = dht12.readHumidity();
      bool dht12Read = true;
      // Check if any reads failed and exit early (to try again).
      if (isnan(h12) || isnan(t12) || isnan(f12)) {
        Serial.println("Failed to read from DHT12 sensor!");
        dht12Read = false;
        dht12get = 1;
      }
      dht12get = 0;
      timeSinceLastRead = 0;
    }
    timeSinceLastRead += 100;
#endif
    if (dateScroll == 1) {
      LEDMatrix_Date_action();
    }
    if (indoorScroll == 1) {
      LEDMatrix_DHT22_action();
    }
    if (outdoorScroll == 1) {
      LEDMatrix_DHT12_action();
    }
    if (weatherScroll == 1) {
      LEDMatrix_Weather_action();
    }
    updCnt--;
    clkTime = millis();
  }
  if (digitalRead(RED_PIN)) {  // Alarm On/Off Switch
    alarm_state = 1;
    if (printMsg == "") {
      alarm_message = ALARM_MESSAGE;
    } else {
      alarm_message = printMsg;      
    }
    if (m == alarm_m_set && alarm_action == 1) {
      if (onTimeAlarm == 1 && m == alarm_m_set && alarm_action == 1 && alarm_state == 1) {
        for (int i = 0; i < 8; i++) {
          pinMode(WHITE_PIN, OUTPUT);
          digitalWrite(WHITE_PIN,  HIGH);
          delay(500);
          digitalWrite(WHITE_PIN,  LOW);
          delay(500);
        }
        printStringWithShift("                ", stringShiftDelay);
        printStringWithShift("Break Time!", stringShiftDelay);
        delay(1000);
        printStringWithShift("                ", stringShiftDelay);
        delay(waitScroll);
      } else if (am_pm == 12 && ampm == alarm_ampm_select && h == alarm_h_set && m == alarm_m_set && alarm_action == 1 && alarm_state == 1) {
        for (int i = 0; i < 8; i++) {
         pinMode(WHITE_PIN, OUTPUT);
          digitalWrite(WHITE_PIN,  HIGH);
          delay(500);
          digitalWrite(WHITE_PIN,  LOW);
          delay(500);
        }
        printStringWithShift("                ", stringShiftDelay);
        printStringWithShift(alarm_message.c_str(), stringShiftDelay);
        delay(1000);
        printStringWithShift("                ", stringShiftDelay);
        delay(waitScroll);
      } else if (h == alarm_h_set && m == alarm_m_set && alarm_action == 1 && alarm_state == 1) {
        for (int i = 0; i < 8; i++) {
          pinMode(WHITE_PIN, OUTPUT);
          digitalWrite(WHITE_PIN,  HIGH);
          delay(200);
          digitalWrite(WHITE_PIN,  LOW);
          delay(200);
        }
        printStringWithShift("                ", stringShiftDelay);
        printStringWithShift(alarm_message.c_str(), stringShiftDelay);
        delay(1000);
        printStringWithShift("                ", stringShiftDelay);
        delay(waitScroll);
      }
      alarm_action = 0; 
    } else if (m == alarm_m_set + 1) {
      alarm_action = 1;
    }
  } else {
    alarm_state = 0;
  }
  if (digitalRead(GREEN_PIN)) {  // Lamp Push Button Switch
#ifdef   USE_RELAY_PIN
      pinMode(USE_RELAY_PIN, OUTPUT);
      digitalWrite(USE_RELAY_PIN,  HIGH);
#endif
#ifdef   USE_LED_PIN
      pinMode(USE_LED_PIN, OUTPUT);
      digitalWrite(USE_LED_PIN,  HIGH);
#endif
#ifdef   USE_RGB_LED && USE_RGB_LED_PIN
      //colorWipe(strip.Color(255,   0,   0), 10);     // Red
      //colorWipe(strip.Color(  0, 255,   0), 10);     // Green
      //colorWipe(strip.Color(  0,   0, 255), 10);     // Blue
      colorWipe(strip.Color(255, 255, 255), 10);     // White
      //colorWipe(strip.Color(  0,   0,   0), 10);     // Black
      // Send a theater pixel chase in...
      //theaterChase(strip.Color(127,   0,   0), 10);  // Red
      //theaterChase(strip.Color(  0, 127,   0), 10);  // Green
      //theaterChase(strip.Color(  0,   0, 127), 10);  // Blue
      //theaterChase(strip.Color(127, 127, 127), 10);  // White
      //rainbow(10);
      //rainbowCycle(10);
      //theaterChaseRainbow(30);
#endif
/*
    button00count = button00count++;
    if (button00count == 1) {
#ifdef   USE_RGB_LED
      //colorWipe(strip.Color(255,   0,   0), 20);     // Red
      //colorWipe(strip.Color(  0, 255,   0), 20);     // Green
      //colorWipe(strip.Color(  0,   0, 255), 20);     // Blue
      colorWipe(strip.Color(255, 255, 255), 10);     // White
      //colorWipe(strip.Color(  0,   0,   0), 20);     // Black
      // Send a theater pixel chase in...
      //theaterChase(strip.Color(127,   0,   0), 20);  // Red
      //theaterChase(strip.Color(  0, 127,   0), 20);  // Green
      //theaterChase(strip.Color(  0,   0, 127), 20);  // Blue
      //theaterChase(strip.Color(127, 127, 127), 20);  // White
      //rainbow(20);
      //rainbowCycle(20);
      //theaterChaseRainbow(50);
#endif
    } else if (button00count == 2) {
#ifdef   USE_RGB_LED
      theaterChase(strip.Color(127,   0,   0), 20);  // Red
      theaterChase(strip.Color(  0, 127,   0), 20);  // Green
      theaterChase(strip.Color(  0,   0, 127), 20);  // Blue
      theaterChase(strip.Color(127, 127, 127), 20);  // White
#endif
    } else if (button00count == 3) {
#ifdef   USE_RGB_LED
      rainbow(20);
#endif
    } else if (button00count == 4) {
#ifdef   USE_RGB_LED
      rainbowCycle(20);
#endif
    } else if (button00count == 5) {
#ifdef   USE_RGB_LED
      theaterChaseRainbow(50);
#endif
      button00count == 0;
    }
 */
  }
  if (!digitalRead(GREEN_PIN)) {  // Lamp Push Button Switch
#ifdef   USE_RELAY_PIN
      digitalWrite(USE_RELAY_PIN,  LOW);
#endif
#ifdef   USE_LED_PIN
      digitalWrite(USE_LED_PIN,  LOW);
#endif
#ifdef   USE_RGB_LED
    //colorWipe(strip.Color(  0,   0,   0),  0);
#endif
  }
  if (digitalRead(BLUE_PIN)) {  // Weather Scroll Push Button Switch
    if (dateScroll == 0) {
        LEDMatrix_Date_action();
    }
    if (indoorScroll == 0) {
        LEDMatrix_DHT22_action();
    }
    if (outdoorScroll == 0) {
        LEDMatrix_DHT12_action();
    }
    if (weatherScroll == 0) {
        LEDMatrix_Weather_action();
    }
  }
  if (Clock == 1) {
    LEDMatrix_Clock_action();
  }
#endif
#ifdef   USE_MQTT
  MQTT_action();
#endif
}
