/* =========================================================================
 *  Author: Zalophus Dokdo (https://zddh.blogspot.com)
 *  Date: 31/08/2017
 *  License: GPL v2
 * =========================================================================
 *  LED Matrix Bar Weather Station
 * =========================================================================
 *  WeMos D1 mini DS18B20 temperature sensor
 * =========================================================================
 */
#ifndef DS18B20_H
#define DS18B20_H

#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into pin 2 on the Arduino
#define  ONE_WIRE_BUS   2  // D4
// Setup a oneWire instance to communicate with any OneWire devices 
OneWire ourWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&ourWire);

// arrays to hold device addresses
DeviceAddress insideThermometer, outsideThermometer;

int ds18b20get      = 0;

void ds18b20_start() {
  pinMode( 1,  INPUT);  // TXD
  sensors.begin();

  // search for devices on the bus and assign based on an index.
  if (!sensors.getAddress(insideThermometer, 0)) { 
#ifdef USE_UART
    Serial.println("Unable to find address for Device 0");
#endif
    ds18b20get = 1;
    return;
  }
  if (!sensors.getAddress(outsideThermometer, 1)) {
#ifdef USE_UART
    Serial.println("Unable to find address for Device 1");
#endif
    //ds18b20get = 2;
    return;
  }
}

void ds18b20_action() {
  sensors.requestTemperatures();  // Send the command to get temperatures
  if (ds18b20get == 0) {
#ifdef USE_UART
    Serial.println("DS18B20 Temperature Sensor (TX - 1)");
    Serial.print("Temperature: ");
    Serial.print(sensors.getTempCByIndex(0));
    Serial.print(" 'C, ");
    Serial.print(sensors.getTempFByIndex(0));
    Serial.println(" 'F");
    Serial.println();
#endif
  } else if(ds18b20get == 1) {
#ifdef USE_UART
    Serial.println("Unable to find address for Device X");
    Serial.println("Failed to read from DS18B20 sensor!");
    Serial.println();
#endif
    ds18b20get = 1;
    return;
  }
}

#endif
