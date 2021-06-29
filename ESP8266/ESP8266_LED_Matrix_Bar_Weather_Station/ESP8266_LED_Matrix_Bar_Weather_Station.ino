/* =========================================================================
 *  Author: Zalophus Dokdo (https://zddh.blogspot.com)
 *  Date: 31/08/2017       (https://zalophus.tistory.com/)
 *  License: GPL v2
 * =========================================================================
 *  LED Matrix Bar Weather Station V1.0.4b (Publish: 2018/01/02)
 * =========================================================================
 *  -> Change WebApp scroll control (2021/01/11)
 *  -> Added timer setting (2020/10/13)
 *  -> Added alarm message setting function:
 *     .................Set scroll message to alarm message (2020/09/13)
 *  -> Added alarm setting (2020/09/10)
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
#endif
#ifdef   USE_RGB_LED_NEOPIXEL
  #include "WS2812B_NeoPixel.h"
#endif
#ifdef   USE_RGB_LED_STRIP_DRIVER
  #include "LEDStrip.h"
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
String onTimeMessage         = OnTIME_MESSAGE;
int    timer_action_start    =  1;
int    timer_action_stop     =  1;
String timerStartMessage     = TIMER_START_MESSAGE;
String timerStopMessage      = TIMER_STOP_MESSAGE;

void setup() {  // #########################################################
  // put your setup code here, to run once:
  Serial.begin(SERIAL_SPEED);
  Serial.println("LED Matrix Bar Weather Station");
  Serial.println();
  delay(10);
#ifdef   USE_DHT
  pinMode(DHTPIN,                INPUT);  // TXD --> DHT11, DHT22
#endif
#ifdef   USE_DS18B20
  pinMode(ONE_WIRE_BUS,          INPUT);  // DS18B20
#endif
  pinMode(RED_PIN,              OUTPUT);  // Alarm ON/OFF SW (Red)
  pinMode(GREEN_PIN,            OUTPUT);  // Lamp Push Button SW (Green)
  pinMode(BLUE_PIN,             OUTPUT);  // Weather Scroll Push Button SW (Blue)
  pinMode(WHITE_PIN,            OUTPUT);  // RXD --> Buzzer & LED
  digitalWrite(RED_PIN,            LOW);
  digitalWrite(GREEN_PIN,          LOW);
  digitalWrite(BLUE_PIN,           LOW);
  digitalWrite(WHITE_PIN,          LOW);
#ifndef USE_DHT
  #ifndef USE_RGB_LED_STRIP_DRIVER
    pinMode(DHTPIN,             OUTPUT);  // D4    (GPIO02)
    digitalWrite(DHTPIN,          HIGH);  // Build-in blue LED disable
  #endif
#endif
#ifdef USE_RGB_LED_DIRECT
  pinMode(RGB_LED_RED_PIN,      OUTPUT);
  pinMode(RGB_LED_GREEN_PIN,    OUTPUT);
  pinMode(RGB_LED_BLUE_PIN,     OUTPUT);
  digitalWrite(RGB_LED_RED_PIN,    LOW);
  digitalWrite(RGB_LED_GREEN_PIN,  LOW);
  digitalWrite(RGB_LED_BLUE_PIN,   LOW);
#endif
#if    defined(DEV_BOARD_WeMos_D1_mini) || defined(DEV_BOARD_ESP01)
  pinMode(USE_RELAY_PIN,        OUTPUT);  // Relay
  digitalWrite(USE_RELAY_PIN,      LOW);
#endif
#ifdef DEV_BOARD_Witty
  pinMode(USE_LIGHT_PIN,        OUTPUT);  // Light
  pinMode(USE_RELAY_PIN,        OUTPUT);  // Relay
  digitalWrite(USE_LIGHT_PIN,      LOW);
  digitalWrite(USE_RELAY_PIN,      LOW);
#endif
#ifdef DEV_BOARD_WiFi_RELAY_MODULE
  pinMode(USE_INPUT_PIN,        OUTPUT);  // Input to Output
  pinMode(USE_RELAY_PIN,        OUTPUT);  // Relay
  digitalWrite(USE_INPUT_PIN,      LOW);
  digitalWrite(USE_RELAY_PIN,      LOW);
#endif

  WiFi_setup();

// Shields =================================================================
#ifdef   USE_DS18B20
  DS18B20_setup();
#endif
#ifdef   USE_DHT
  DHT_setup();
#endif
#ifdef   USE_DHT12
  DHT12_setup();
#endif
#ifdef   USE_RGB_LED_NEOPIXEL
  NeoPixel_setup();
#endif
#ifdef   USE_RGB_LED_STRIP_DRIVER
  led.setColor();
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
  LEDMatrix_action();
  if(millis() - clkTime > weatherScrollInterval && !del && dots) {  // clock for 15s, then scrolls for about 30s
#ifdef USE_LED_MATRIX
    if (dateScroll == 1) {
      LEDMatrix_Date_action();
    }
  #ifdef   USE_DHT
    if (indoorScroll == 1) {
      LEDMatrix_DHT_action();
    }
  #endif
  #ifdef   USE_DHT12
    if (outdoorScroll == 1) {
      LEDMatrix_DHT12_action();
    }
  #endif
    if (weatherScroll == 1) {
      LEDMatrix_Weather_action();
    }
#endif
    updCnt--;
    clkTime = millis();
  }
#endif
  if (Clock == 1) {
    LEDMatrix_Clock_action();
  }
  // Alarm On/Off Switch
  if (digitalRead(RED_PIN)) {  
    if (printMsg == "") {
      alarm_message = ALARM_MESSAGE;
    } else {
      alarm_message = printMsg;      
    }
    if (m == alarm_m_set && alarm_action == 1) {
      // On-time Alarm
      if (onTimeAlarm == 1 && m == alarm_m_set && alarm_state == 1) {
        for (int i = 0; i < 8; i++) {
          digitalWrite(WHITE_PIN,  HIGH);
          #ifdef USE_RGB_LED_DIRECT
            digitalWrite(RGB_LED_RED_PIN, HIGH); digitalWrite(RGB_LED_GREEN_PIN, HIGH); digitalWrite(RGB_LED_BLUE_PIN, HIGH);
          #endif
          delay(500);
          digitalWrite(WHITE_PIN,  LOW);
          #ifdef USE_RGB_LED_DIRECT
            digitalWrite(RGB_LED_RED_PIN, LOW); digitalWrite(RGB_LED_GREEN_PIN, LOW); digitalWrite(RGB_LED_BLUE_PIN, LOW);
          #endif
          delay(500);
        }
        #ifdef USE_RGB_LED_STRIP_DRIVER
          ledStripColorWipe();
        #endif
        #ifdef USE_LED_MATRIX
          printStringWithShift("                ", stringShiftDelay);
          printStringWithShift(onTimeMessage.c_str(), stringShiftDelay);
          delay(1000);
          printStringWithShift("                ", stringShiftDelay);
          delay(waitScroll);
        #endif
      // 12-hour clock Alarm
      } else if (am_pm == 12 && ampm == alarm_ampm_select && h == alarm_h_set && m == alarm_m_set && alarm_state == 1) {
        for (int i = 0; i < 8; i++) {
          digitalWrite(WHITE_PIN,  HIGH);
          #ifdef USE_RGB_LED_DIRECT
            digitalWrite(RGB_LED_RED_PIN, HIGH); digitalWrite(RGB_LED_GREEN_PIN, HIGH); digitalWrite(RGB_LED_BLUE_PIN, HIGH);
          #endif
          delay(500);
          digitalWrite(WHITE_PIN,  LOW);
          #ifdef USE_RGB_LED_DIRECT
            digitalWrite(RGB_LED_RED_PIN, LOW); digitalWrite(RGB_LED_GREEN_PIN, LOW); digitalWrite(RGB_LED_BLUE_PIN, LOW);
          #endif
          delay(500);
        }
        #ifdef USE_RGB_LED_STRIP_DRIVER
          ledStripColorWipe();
        #endif
        #ifdef USE_LED_MATRIX
          printStringWithShift("                ", stringShiftDelay);
          printStringWithShift(alarm_message.c_str(), stringShiftDelay);
          delay(1000);
          printStringWithShift("                ", stringShiftDelay);
          delay(waitScroll);
        #endif
      // 24-hour clock Alarm
      } else if (am_pm == 24 && h == alarm_h_set && m == alarm_m_set && alarm_state == 1) {
        for (int i = 0; i < 8; i++) {
          digitalWrite(WHITE_PIN,  HIGH);
          #ifdef USE_RGB_LED_DIRECT
            digitalWrite(RGB_LED_RED_PIN, HIGH); digitalWrite(RGB_LED_GREEN_PIN, HIGH); digitalWrite(RGB_LED_BLUE_PIN, HIGH);
          #endif
          delay(200);
          digitalWrite(WHITE_PIN,  LOW);
          #ifdef USE_RGB_LED_DIRECT
            digitalWrite(RGB_LED_RED_PIN, LOW); digitalWrite(RGB_LED_GREEN_PIN, LOW); digitalWrite(RGB_LED_BLUE_PIN, LOW);
          #endif
          delay(200);
        }
        #ifdef USE_RGB_LED_STRIP_DRIVER
          ledStripColorWipe();
        #endif
        #ifdef USE_LED_MATRIX
          printStringWithShift("                ", stringShiftDelay);
          printStringWithShift(alarm_message.c_str(), stringShiftDelay);
          delay(1000);
          printStringWithShift("                ", stringShiftDelay);
          delay(waitScroll);
        #endif
      }
      alarm_action = 0; 
    } else if (m == alarm_m_set + 1) {
      alarm_action = 1;
    }
  }
  // Lamp Push Button Switch
  if (digitalRead(GREEN_PIN)) {
    #ifdef   USE_RELAY
      digitalWrite(USE_RELAY_PIN, HIGH);
    #endif
  }
  if (!digitalRead(GREEN_PIN)) {
    #ifdef   USE_RELAY
      digitalWrite(USE_RELAY_PIN,  LOW);
    #endif
  }
#ifdef USE_LED_MATRIX
  // Weather Scroll Push Button Switch
  if (digitalRead(BLUE_PIN)) {
    if (dateScroll == 0) {
        LEDMatrix_Date_action();
    }
  #ifdef   USE_DHT
    if (indoorScroll == 0) {
        LEDMatrix_DHT_action();
    }
  #endif
  #ifdef   USE_DHT12
    if (outdoorScroll == 0) {
        LEDMatrix_DHT12_action();
    }
  #endif
    if (weatherScroll == 0) {
        LEDMatrix_Weather_action();
    }
  }
#endif
#ifdef DEV_BOARD_Witty
  if (light05on_state == 1) {
    digitalWrite(USE_LIGHT_PIN,   HIGH);
  }
  if (light05on_state == 0) { 
    digitalWrite(USE_LIGHT_PIN,    LOW);
  }
  }
  if (digitalRead(USE_RELAY_PIN)) {
    digitalWrite(RGB_LED_RED_PIN, HIGH); digitalWrite(RGB_LED_GREEN_PIN, HIGH); digitalWrite(RGB_LED_BLUE_PIN, HIGH);
  }
  if (!digitalRead(USE_RELAY_PIN)) {
    digitalWrite(RGB_LED_RED_PIN,  LOW); digitalWrite(RGB_LED_GREEN_PIN,  LOW); digitalWrite(RGB_LED_BLUE_PIN,  LOW);
  }
#endif
#ifdef DEV_BOARD_WiFi_RELAY_MODULE
  if (light05on_state == 1) {
    digitalWrite(USE_INPUT_PIN,   HIGH);
  }
  if (light05on_state == 0) { 
    digitalWrite(USE_INPUT_PIN,    LOW);
  }
#endif
#ifdef USE_TIMER
  // Timer start
  if (m == timer_m_start && timer_action_start == 1) {
    // 12-hour clock
    if (am_pm == 12 && ampm == timer_start_ampm_select && h == timer_h_start && m == timer_m_start && timer_state == 1) {
    #ifdef DEV_BOARD_Witty
      digitalWrite(USE_LIGHT_PIN, HIGH);
      light05on_state = 1;
    #endif
      digitalWrite(GREEN_PIN, HIGH);
    #ifdef USE_RGB_LED_DIRECT
      digitalWrite(RGB_LED_RED_PIN, HIGH); digitalWrite(RGB_LED_GREEN_PIN, HIGH); digitalWrite(RGB_LED_BLUE_PIN, HIGH);
    #endif
    #ifdef USE_RGB_LED_STRIP_DRIVER
      ledStripColorWipe();
      led.setColor(255, 255, 255);
    #endif
    #ifdef USE_LED_MATRIX
      printStringWithShift("                ", stringShiftDelay);
      printStringWithShift(timerStartMessage.c_str(), stringShiftDelay);
      delay(1000);
      printStringWithShift("                ", stringShiftDelay);
      delay(waitScroll);
    #endif
      timer_action_start = 0;
      timer_action_stop  = 1;
    // 24-hour clock
    } else if (am_pm == 24 && h == timer_h_start && m == timer_m_start && timer_state == 1) {
    #ifdef DEV_BOARD_Witty
      digitalWrite(USE_LIGHT_PIN, HIGH);
      light05on_state = 1;
    #endif
      digitalWrite(GREEN_PIN, HIGH);
    #ifdef USE_RGB_LED_DIRECT
      digitalWrite(RGB_LED_RED_PIN, HIGH); digitalWrite(RGB_LED_GREEN_PIN, HIGH); digitalWrite(RGB_LED_BLUE_PIN, HIGH);
    #endif
    #ifdef USE_RGB_LED_STRIP_DRIVER
      ledStripColorWipe();
      led.setColor(255, 255, 255);
    #endif
    #ifdef USE_LED_MATRIX
      printStringWithShift("                ", stringShiftDelay);
      printStringWithShift(timerStartMessage.c_str(), stringShiftDelay);
      delay(1000);
      printStringWithShift("                ", stringShiftDelay);
      delay(waitScroll);
    #endif
      timer_action_start = 0;
      timer_action_stop  = 1;
    }
  }
  // Timer stop
  if (m == timer_m_stop && timer_action_stop == 1) {
    // 12-hour clock
    if (am_pm == 12 && ampm == timer_stop_ampm_select && h == timer_h_stop && m == timer_m_stop && timer_state == 1) {
    #ifdef DEV_BOARD_Witty
      digitalWrite(USE_LIGHT_PIN,  LOW);
      light05on_state = 0;
    #endif
      digitalWrite(GREEN_PIN,  LOW);
    #ifdef USE_RGB_LED_DIRECT
      digitalWrite(RGB_LED_RED_PIN, LOW); digitalWrite(RGB_LED_GREEN_PIN, LOW); digitalWrite(RGB_LED_BLUE_PIN, LOW);
    #endif
    #ifdef USE_RGB_LED_STRIP_DRIVER
      led.setColor();
    #endif
    #ifdef USE_LED_MATRIX
      printStringWithShift("                ", stringShiftDelay);
      printStringWithShift(timerStopMessage.c_str(), stringShiftDelay);
      delay(1000);
      printStringWithShift("                ", stringShiftDelay);
      delay(waitScroll);
    #endif
      timer_action_stop  = 0;
      timer_action_start = 1;
    // 24-hour clock
    } else if (am_pm == 24 && h == timer_h_stop && m == timer_m_stop && timer_state == 1) {
    #ifdef DEV_BOARD_Witty
      digitalWrite(USE_LIGHT_PIN,  LOW);
      light05on_state = 0;
    #endif
      digitalWrite(GREEN_PIN,  LOW);
    #ifdef USE_RGB_LED_DIRECT
      digitalWrite(RGB_LED_RED_PIN, LOW); digitalWrite(RGB_LED_GREEN_PIN, LOW); digitalWrite(RGB_LED_BLUE_PIN, LOW);
    #endif
    #ifdef USE_RGB_LED_STRIP_DRIVER
      led.setColor();
    #endif
    #ifdef USE_LED_MATRIX
      printStringWithShift("                ", stringShiftDelay);
      printStringWithShift(timerStopMessage.c_str(), stringShiftDelay);
      delay(1000);
      printStringWithShift("                ", stringShiftDelay);
      delay(waitScroll);
    #endif
      timer_action_stop  = 0;
      timer_action_start = 1;
    }
  }
#endif
#ifdef   USE_MQTT
  MQTT_action();
#endif
}
