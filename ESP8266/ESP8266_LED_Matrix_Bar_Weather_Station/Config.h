/* =========================================================================
 *  Author: Zalophus Dokdo (https://zddh.blogspot.com)
 *  Date: 31/08/2017
 *  License: GPL v2
 * =========================================================================
 *  LED Matrix Bar Weather Station
 * =========================================================================
 *  WeMos D1 mini configuration
 * =========================================================================
 */
#ifndef CONFIG_H
#define CONFIG_H

//#define USE_UART            // debug mode 
#define  USE_OTA            // Use OTA

// Sensors =================================================================
#define  USE_DS18B20        // D4(02)          // Use DS18B20 OneWire temperature sensor
#ifndef USE_UART
  #define  USE_DHT22        // TX(01)          // Use DHT22 OneWire temperature and humidity sensor (or DHT11)
#endif
#define  USE_DHT12          // D1(05), D2(04)  // Use DHT12 I2C temperature and humidity sensor
// =========================================================================

#define  USE_WEB_SERVER                        // Use Web Server
#define  USE_LEDMATRIX_CLOCKWEATHER

// WiFi ====================================================================
#define  AP_PASSWORD          "0123456789"     // local AP password


// I2C bus parameters ======================================================
#define  SCL_PIN              5
#define  SDA_PIN              4
#define  I2C_SPEED            400000           // 400000 = 400kHz I2C speed

// UART ====================================================================
#define  SERIAL_SPEED         115200

// Default status configurations ===========================================
#define OTA_MODE              1
#define LED_MATRIX            1
#define GCLOCK_WEATHER_SCROLL 0
#define GCLOCK                1
#define DATE_SCROLL           1
#define WEATHER_SCROLL        1
#define INDOOR                1
#define OUTDOOR               0
#define INDOOR_SCROLL         0
#define OUTDOOR_SCROLL        0
#define WAIT_SCROLL         500

// LED Matrix Bar ==========================================================
// CHANGE YOUR CONFIG HERE: Weather Host http://api.openweathermap.org
// =========================================================================
#define  WEATHER_KEY          "5dc91881c3a26336a86a52add18ee869"              // API Key
#define  WEATHER_LANG         "&lang=en"       // "&lang=en" English
#define  CITY_ID              "1846898"              // City ID Number http://openweathermap.org/help/city_list.txt
// read OpenWeather api description for more info
#define  NUM_MAX              14               // Number of MAX7219's connected
#define  MAX_DIGITS           16
#define  SET_INTENSITY         3               // Initial led matrix intensity, 0-15
#define  UTC_OFFSET            9               // UTC+9 
// =========================================================================
#define  SHOW_ANIM_CLOCK_SW    0

#endif
