/* =========================================================================
 *  Author: Zalophus Dokdo (https://zddh.blogspot.com)
 *  Date: 31/08/2017       (https://zalophus.tistory.com/)
 *  License: GPL v2
 * =========================================================================
 *  LED Matrix Bar Weather Station V1.0.4 (Publish: 2018/01/02)
 * =========================================================================
 *  DHT11 temperature and humidity sensor
 *  DHT22(AM2302) temperature and humidity sensor
 *  DHT21(AM2301) temperature and humidity sensor
 * =========================================================================
 */

#ifndef DHT22_H
#define DHT22_H

#include <DHT.h>

DHT   dht(DHTPIN, DHTTYPE);

int   dhtGet       = 0;
float dhtHumidity;
float dhtCTemp;
float dhtFTemp;

void DHT_setup() {
  dht.begin();
}

void DHT_action() {
  // DHT22 Temperature & Humidity Sensor
  // Wait a few seconds between measurements.
  delay(2000);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  dhtHumidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  dhtCTemp    = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  dhtFTemp    = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(dhtHumidity) || isnan(dhtCTemp) || isnan(dhtFTemp)) {
#ifdef USE_UART
    Serial.println("Failed to read from DHT sensor!");
    Serial.println();
#endif
    dhtGet = 1;
    return;
  } else {
    dhtGet = 0;

    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(dhtFTemp, dhtHumidity);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(dhtCTemp, dhtHumidity, false);

#ifdef USE_UART
    Serial.println("DHT Temperature & Humidity Sensor (D4 - 2)");
    Serial.print("Temperature in Celsius : ");
    Serial.print(dhtCTemp, 1);
    Serial.println(" 'C ");
    Serial.print("Temperature in Fahrenheit : ");
    Serial.print(dhtFTemp, 1);
    Serial.println(" 'F\t");
    Serial.print("Relative Humidity: ");
    Serial.print(dhtHumidity, 1);
    Serial.println(" %\t");
    Serial.print("Heat index: ");
    Serial.print(hic);
    Serial.print(" 'C ");
    Serial.print(hif);
    Serial.println(" 'F");
    Serial.println();
#endif
  }
}

#endif
