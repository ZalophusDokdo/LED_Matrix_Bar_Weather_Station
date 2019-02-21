/* =========================================================================
 *  Author: Zalophus Dokdo (https://zddh.blogspot.com)
 *  Date: 31/08/2017
 *  License: GPL v2
 * =========================================================================
 *  LED Matrix Bar Weather Station
 * =========================================================================
 *  WeMos D1 mini WiFi setup
 * =========================================================================
 */
#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "WiFi_Manager.h"

MDNSResponder mdns;

void wifi_start() {
  wifimanager_start();

#ifdef   USE_OTA
  // Port defaults to 8266
  //ArduinoOTA.setPort(8266);
  // Hostname defaults to esp8266-[ChipID]
  //ArduinoOTA.setHostname("myesp8266");
  // No authentication by default
  //ArduinoOTA.setPassword((const char *)"123");
#ifdef USE_UART
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("End");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\n", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
#endif
  ArduinoOTA.begin();
#ifdef USE_UART
  Serial.println("OTA Ready!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
#endif
#endif

  // Set up mDNS responder: ##################################################
  // - first argument is the domain name, in this example
  //   the fully-qualified domain name is "homeiot.local"
  // - second argument is the IP address to advertise
  //   we send our IP address on the WiFi network
  // - dmain name "homeiotlrl: Home IoT Living Room Lamp"
  //   first two character is location - lr: Living Room, mr: Main Room, sr: Second Room, kc: Kitchen, fd: Front Door, vd: Veranda, ...
  //   secound one character is device - l: Lamp, s: Sensor, d: door, w: Window, ...
  if (!MDNS.begin(APname, WiFi.localIP())) {
#ifdef USE_UART
    Serial.println("Error setting up MDNS responder!");
#endif
    while(1) { 
      delay(1000);
    }
  }
#ifdef USE_UART
  Serial.println("mDNS responder started");
#endif

  // Add service to MDNS-SD ################################################
  MDNS.addService("http", "tcp", 80);
}

void ota_action() {
  ArduinoOTA.handle();
}

#endif
