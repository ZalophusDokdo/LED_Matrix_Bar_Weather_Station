/* =========================================================================
 *  Author: Zalophus Dokdo (https://zddh.blogspot.com)
 *  Date: 31/08/2017       (https://zalophus.tistory.com/)
 *  License: GPL v2
 * =========================================================================
 *  LED Matrix Bar Weather Station V1.0.4 (Publish: 2018/01/02)
 * =========================================================================
 *  MQTT
 * =========================================================================
 */

#ifndef MQTT_H
#define MQTT_H

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* Adafruit.io Setup *********************************/
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                                // use 1883 for SSL
#define AIO_USERNAME    " "                          // Replace it with your username
#define AIO_KEY         " "  // Replace with your Project Auth Key
/************************* Adafruit.io ***************************************/
// WiFi Client
//WiFiClient client;
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
/****************************** Feeds ****************************************/
// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe weatherForcast = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Forcast"); // FeedName
Adafruit_MQTT_Subscribe indoorForcast  = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Indoor");  // FeedName
Adafruit_MQTT_Subscribe outdoorForcast = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Outdoor"); // FeedName

int weatherForcast_State;
int indoorForcast_State;
int outdoorForcast_State;

void MQTT_connect();

/************************* Adafruit.io ***************************************/
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    //delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");  
}

void MQTT_setup() {
/************************* Adafruit.io ***************************************/
  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&weatherForcast);
  mqtt.subscribe(&indoorForcast);
  mqtt.subscribe(&outdoorForcast);
}

void MQTT_action() {
/************************* Adafruit.io ***************************************/
  MQTT_connect();
/************************* Adafruit.io ***************************************/
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(100))) {  // 5000
    if (subscription == &weatherForcast) {
      Serial.print(F("Got: "));
      Serial.println((char *)weatherForcast.lastread);
      int weatherForcast_State = atoi((char *)weatherForcast.lastread);
      //digitalWrite(12, (weatherForcast_State));
        LEDMatrix_Date_action();
        LEDMatrix_Weather_action();
    }
    if (subscription == &indoorForcast) {
      Serial.print(F("Got: "));
      Serial.println((char *)indoorForcast.lastread);
      int indoorForcast_State = atoi((char *)indoorForcast.lastread);
      //digitalWrite(12, indoorForcast_State);
        LEDMatrix_DHT22_action();
    }
    if (subscription == &outdoorForcast) {
      Serial.print(F("Got: "));
      Serial.println((char *)outdoorForcast.lastread);
      int outdoorForcast_State = atoi((char *)outdoorForcast.lastread);
      //digitalWrite(12, outdoorForcast_State);
        LEDMatrix_DHT12_action();
    }
  }
}

#endif
