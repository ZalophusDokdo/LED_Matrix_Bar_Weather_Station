/* =========================================================================
 *  Author: Zalophus Dokdo (https://zddh.blogspot.com)
 *  Date: 31/08/2017
 *  License: GPL v2
 * =========================================================================
 *  LED Matrix Bar Weather Station
 * =========================================================================
 *  WeMos D1 mini DHT temperature and humidity sensor
 * =========================================================================
 */
#ifndef DHT22_H
#define DHT22_H

#include <DHT.h>

#define  DHTPIN         1  // TXD what digital pin we're connected to
#define  DHTTYPE    DHT22  // DHT 22  (AM2302), AM2321
DHT     dht(DHTPIN, DHTTYPE);

int dht22get        = 0;
float dht22Humidity;
float dht22cTemp;
float dht22fTemp;

void dht22_start() {
  dht.begin();
}

void dht22_action() {
  // DHT22 Temperature & Humidity Sensor
  // Wait a few seconds between measurements.
  // delay(2000);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  dht22Humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  dht22cTemp    = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  dht22fTemp    = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(dht22Humidity) || isnan(dht22cTemp) || isnan(dht22fTemp)) {
#ifdef USE_UART
    Serial.println("Failed to read from DHT sensor!");
    Serial.println();
#endif
    dht22get = 1;
    return;
  } else {
    dht22get = 0;

    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(dht22fTemp, dht22Humidity);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(dht22cTemp, dht22Humidity, false);

#ifdef USE_UART
    Serial.println("DHT22 Temperature & Humidity Sensor (D4 - 2)");
    Serial.print("Temperature in Celsius : ");
    Serial.print(dht22cTemp, 1);
    Serial.println(" 'C ");
    Serial.print("Temperature in Fahrenheit : ");
    Serial.print(dht22fTemp, 1);
    Serial.println(" 'F\t");
    Serial.print("Relative Humidity: ");
    Serial.print(dht22Humidity, 1);
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
