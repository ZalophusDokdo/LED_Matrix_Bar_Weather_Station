/* =========================================================================
 *  Author: Zalophus Dokdo (https://zddh.blogspot.com)
 *  Date: 31/08/2017
 *  License: GPL v2
 * =========================================================================
 *  LED Matrix Bar Weather Station V1.0.1 (Publish: 01/02/2018)
 *  -> Change DHT12 Library (03/12/2018)
 * =========================================================================
 *  Parts List:
 *  - 6 x MAX7219 8x8 Matrix LED
 *  - 1 x WeMos D1 mini
 *  - 1 x Interface shield
 *  Additional Parts:
 *  - N x DS18B20 temperature sensor
 *  - 1 x DHT22 temperature and humidity sensor
 *  - 1 x DHT12 temperature and humidity sensor
 *  - N x WS2812B RGB LED
 *  - 1 x Push button switch
 *  - 1 x On/Off button switxh
 *  - 1 x Power On/Off switxh
 * -------------------------------------------------------------------------
 *  Fuctions:
 *  - OTA
 *  - mDNS
 *  - WiFi Manager
 *  - Web Server (WebUI)
 *  - Google Clock
 *  - Open Weather Map Weather Forecast
 *  - Scrolling message
 *  - State storage (EEPROM)
 *  - Scroll Push Switch
 *  - Lamp Push Switch (?)
 *  - Alarm (?)
 *  - Alarm On/Off Switch (?)
 * =========================================================================
 */
#include <FS.h>                  // this needs to be first, or it all crashes and burns...

#include <ESP8266WiFi.h>
#include "Config.h"
#include "WiFi_Setup.h"
#include "ESP_I2C.h"

// Shields: ================================================================
#ifdef   USE_DS18B20
  #include "DS18B20.h"
#endif
#ifdef   USE_DHT22
  #include "DHT22.h"
#endif
#ifdef   USE_DHT12
  #include "DHT12.h"
  // Set dht12 i2c comunication on default Wire pin
  DHT12 dht12;
  int timeSinceLastRead = 0;
  int dht12get          = 0;
#endif
#ifdef   USE_RGB_LED
  #include "RGB_LED.h"
#endif
// =========================================================================
#ifdef   USE_LEDMATRIX_CLOCKWEATHER
  #include "LEDMatrix_ClockWeather.h"
#endif
#ifdef   USE_WEB_SERVER
  #include "Web_Server.h"
#endif

#define BATTERY_PIN          17  // A0, ADC Wemos: 3.2V max.
int ScrollingMessage = 0;

int    freq[] = {1047, 1175, 1319, 1397, 1568, 1760, 1976, 2093};
String note[] = {"C6", "D6", "E6", "F6", "G6", "A6", "B6", "C7"};

void setup() {  // #########################################################
  // put your setup code here, to run once:
  Serial.begin(SERIAL_SPEED);
  Serial.println("LED Matrix Bar Weather Station");
  Serial.println();
  delay(10);
  pinMode( 1,  INPUT);  // TXD --> DHT22
  pinMode( 2,  INPUT);  // DS18B20
  pinMode( 3, OUTPUT);  // RXD --> Buzzer & LED
  pinMode(16, OUTPUT);  // Alarm ON/OFF SW (Red)
  pinMode( 0, OUTPUT);  // Lamp Push Button SW (Green)
  pinMode(12, OUTPUT);  // Weather Scroll Push Button SW (Blue)
  digitalWrite( 3,  LOW);
  digitalWrite(16,  LOW);
  digitalWrite( 0,  LOW);
  digitalWrite(12,  LOW);

  wifi_start();
  i2c_begin(SDA_PIN, SCL_PIN, I2C_SPEED);

// Shields =================================================================
#ifdef   USE_DS18B20
  ds18b20_start();
#endif
#ifdef   USE_DHT22
  dht22_start();
#endif
#ifdef   USE_DHT12
  dht12.begin();
#endif
#ifdef   USE_RGB_LED
  rgb_led_start();
#endif
// =========================================================================
#ifdef   USE_WEB_SERVER
  webserver_start();
#endif
#ifdef   USE_LEDMATRIX_CLOCKWEATHER
  LEDMatrix_ClockWeather_Start();
#endif
}

void loop() {  // ##########################################################
#ifdef   USE_OTA
  if (OTAmode == 1) {
    ota_action();
  }
#endif
// Shields =================================================================
#ifdef   USE_RGB_LED
  rgb_led_action();
#endif
// =========================================================================
#ifdef   USE_WEB_SERVER
  webserver_action();
#endif
#ifdef   USE_LEDMATRIX_CLOCKWEATHER
  if (ledMatrix == 1) {
    LEDMatrix_Action();
  }
  if(millis() - clkTime > 15000 && !del && dots) {  // clock for 15s, then scrolls for about 30s
#ifdef   USE_DS18B20
    ds18b20_action();
#endif
#ifdef   USE_DHT22
    dht22_action();
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
      LEDMatrix_Date_Action();
    }
    if (indoorScroll == 1) {
      LEDMatrix_DHT22_Action();
    }
    if (outdoorScroll == 1) {
      LEDMatrix_DHT12_Action();
    }
    if (weatherScroll == 1) {
      LEDMatrix_Weather_Action();
    }
    updCnt--;
    clkTime = millis();
  }
  if (digitalRead(16)) {
    for (int i = 0; i < 8; i++) {
      analogWriteRange(freq[i]);
      analogWrite( 3,  512);
      delay(1000);
      analogWrite( 3,  0);
      pinMode( 3, OUTPUT);
      digitalWrite( 3,  LOW);
      delay(1000);
    }
  }
  if (digitalRead(0)) {
    digitalWrite( 3,  HIGH);
  }
  if (digitalRead(12)) {
    LEDMatrix_Date_Action();
    if (indoorScroll == 0) {
        LEDMatrix_DHT22_Action();
    }
    if (outdoorScroll == 0) {
        LEDMatrix_DHT12_Action();
    }
    LEDMatrix_Weather_Action();
  }
  if (gClock == 1) {
    LEDMatrix_Clock_Action();
  }
#endif
}
