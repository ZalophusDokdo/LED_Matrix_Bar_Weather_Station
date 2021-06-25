/* =========================================================================
 *  Author: Zalophus Dokdo (https://zddh.blogspot.com)
 *  Date: 31/08/2017       (https://zalophus.tistory.com/)
 *  License: GPL v2
 * =========================================================================
 *  LED Matrix Bar Weather Station V1.0.4 (Publish: 2018/01/02)
 * =========================================================================
 *  DHT12 temperature and humidity sensor
 * =========================================================================
 */

#ifndef DHT12_H
#define DHT12_H

#include "DHT12Driver.h"

// Set dht12 i2c comunication on default Wire pin
DHT12 dht12;
int timeSinceLastRead   =  0;
int dht12get            =  0;

void DHT12_setup() {
  dht12.begin();
}

void DHT12_action() {
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
}

#endif
