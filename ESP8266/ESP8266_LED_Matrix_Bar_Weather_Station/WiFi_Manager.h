/* =========================================================================
 *  Author: Zalophus Dokdo (https://zddh.blogspot.com)
 *  Date: 31/08/2017       (https://zalophus.tistory.com/)
 *  License: GPL v2
 * =========================================================================
 *  LED Matrix Bar Weather Station V1.0.4 (Publish: 2018/01/02)
 * =========================================================================
 *  WiFi Manager
 * =========================================================================
 */

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>           // https://github.com/tzapu/WiFiManager

uint8_t    MAC_array[6];                  // MAC address of Wifi module
char       MAC_char[18];
char       *APname;
char       *mDNS_Name = mDNS_NAME;

void WiFiManager_setup() {
  // Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  // exit after config instead of connecting
  wifiManager.setBreakAfterConfig(true);
  // reset settings - for testing
  //wifiManager.resetSettings();
  // tries to connect to last known settings
  // if it does not connect it starts an access point with the specified name
  // here "AutoConnectAP" with password "password"
  // and goes into a blocking loop awaiting configuration

  // Generate host name. SSID=LEDMatrixBar-XXXXXX, PASS=0123456789
#ifdef USE_UART
  Serial.println("\nStarting WIFI in SOFTAP mode");
#endif
  APname = mDNS_Name;
  WiFi.macAddress(MAC_array);
  for (int i = 0; i < sizeof(MAC_array); ++i){
    sprintf(MAC_char, "%s%02x:", MAC_char, MAC_array[i]);
  }
  APname[13] = MAC_char[9];
  APname[14] = MAC_char[10];
  APname[15] = MAC_char[12];
  APname[16] = MAC_char[13];
  APname[17] = MAC_char[15];
  APname[18] = MAC_char[16];
#ifdef USE_UART
  Serial.println("");
  Serial.print("MAC:");
  Serial.println(MAC_char);
  Serial.print("SSID:");
  Serial.println(APname);
#endif
  const char *ssid       = APname;
  const char *password   = AP_PASSWORD;
#ifdef USE_UART
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("Password: ");
  Serial.println(password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
#endif

  if (!wifiManager.autoConnect(ssid, password)) {
#ifdef USE_UART
    Serial.println("failed to connect, we should reset as see if it connects");
#endif
    delay(3000);
    ESP.reset();
    delay(5000);
  }
  //if you get here you have connected to the WiFi
#ifdef USE_UART
  Serial.println("connected...yeey :)");

  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
#endif
}

#endif
