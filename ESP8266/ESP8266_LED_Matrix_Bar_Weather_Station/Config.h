/* =========================================================================
 *  Author: Zalophus Dokdo (https://zddh.blogspot.com)
 *  Date: 31/08/2017       (https://zalophus.tistory.com/)
 *  License: GPL v2
 * =========================================================================
 *  LED Matrix Bar Weather Station V1.0.4 (Publish: 2018/01/02)
 * =========================================================================
 *  Configuration
 * =========================================================================
 */

#ifndef CONFIG_H
#define CONFIG_H

#define PROJECT_NAME  "LED Matrix Bar Weather Station"  // Project Name
#define PROJECT_VERSION        "v1.0.4"  // Project Version

#define WEB_TITLE      "LED Matrix Bar"  // Web Title Name
#define WEB_SUBTITLE  "Weather Station"  // Web Sub-Title Name
#define WEB_PAGE_REFRESH            180  // web page refresh time: 180 = 3 min, 300 = 5 min, 600 = 10 min

//#define USE_UART                       // debug mode 
#define USE_OTA                          // Use OTA
#define OTA_MODE                      1

#define USE_LEDMATRIX_CLOCKWEATHER

// Network =================================================================
#define USE_WEB_SERVER                   // Use Web Server
#define USE_MQTT                         // Use Adafruit IO MQTT
#ifdef USE_MQTT
  /************************* Adafruit.io Setup ****************************/
  #define AIO_SERVER      "io.adafruit.com"
  #define AIO_SERVERPORT  1883                                // use 1883 for SSL
  #define AIO_USERNAME    "Zalophus"                          // Replace it with your username
  #define AIO_KEY         "aio_BxKq919IhP4duJVrp4hqa82S2Ndy"  // Replace with your Project Auth Key
  /************************* Adafruit.io **********************************/
#endif

// Sensors =================================================================
#ifndef USE_UART
  #define USE_DS18B20                    // Use DS18B20 OneWire temperature sensor
#endif
#define USE_DHT                          // Use DHT11, DHT22, DHT21 OneWire temperature and humidity sensor (or DHT11)
//#define USE_DHT12                        // Use DHT12 I2C temperature and humidity sensor

#define USE_RGB_LED                      // Use RGB LED
#define USE_RELAY                        // Use RELAY

// WiFi ====================================================================
#define AP_PASSWORD        "0123456789"  // local AP password

// UART ====================================================================
#define SERIAL_SPEED             115200

// Pin Assignment ==========================================================
// LED Matrix for NodeMCU 1.0 (ESP-12E Module) 
// =========================================================================
#define DIN_PIN                      13  // D7    (GPIO13)
#define CS_PIN                       15  // D8    (GPIO15)
#define CLK_PIN                      14  // D5    (GPIO14)
// Data wire
#define ONE_WIRE_BUS                  1  // TXD   (GPIO01) DS18B20
#define DHTPIN                        2  // D4    (GPIO02)
#define DHTTYPE                   DHT22  // DHT11, DHT22(AM2302), DHT21(AM2301)
// Battery Power Monitor
#define BATTERY_PIN                  17  // A0    (ADC) Wemos: 3.2V max.
// RGBW LED or Buzzer or Relay or Switch (Alarm On/Off, Lamp, Scroll)
#define RED_PIN                      16  // D0    (GPIO16) Alarm On/Off Switch
#define GREEN_PIN                     0  // D3    (GPIO00) Lamp Push Button
#define BLUE_PIN                     12  // D6    (GPIO12) Manual Scroll Push Button
#define WHITE_PIN                     3  // RXD   (GPIO03) Buzzer or Relay or GPIO Output
#ifndef USE_DHT12
  #define USE_RELAY_PIN               5  // SCL   (GPIO05) Relay
  #define USE_RGB_LED_PIN             4  // SDA   (GPIO04) WS2812B RGB LED
#endif
#ifdef  USE_DHT12
  #define USE_RELAY_PIN               3  // RXD   (GPIO03) Relay
  #define USE_LED_PIN                 3  // RXD   (GPIO03) LED Light
  #define USE_RGB_LED_PIN             3  // RXD   (GPIO03) WS2812B RGB LED
#endif
#define NUMPIXELS                    64  //1  //16  //64  //256

// LED Matrix Bar ==========================================================
// Default status configurations ===========================================
// =========================================================================
#define NUM_MAX                       6  // Number of MAX7219's connected
#define MAX_DIGITS                   16
#define SET_INTENSITY                 1  // Initial led matrix intensity, 0-15
#define LED_MATRIX                    1
#define CLOCK                         1
#define DATE_SCROLL                   0
#define WEATHER_SCROLL                0
#define INDOOR                        1
#define OUTDOOR                       0
#define INDOOR_SCROLL                 0
#define OUTDOOR_SCROLL                0
#define WAIT_SCROLL                  50  // Waiting after executing web command  
#define WEATHER_SCROLL_INTERVAL   30000  // default: 15000 clock for 15s, then scrolls for about 30s, 60000 = 1m
#define STRING_SHIFT_DELAY           35  // Scroll Speed

// Weather Forcast =========================================================
// CHANGE YOUR CONFIG HERE: Weather Host http://api.openweathermap.org
// =========================================================================
#define WEATHER_KEY                 " "  // API Key
#define WEATHER_LANG         "&lang=en"  // "&lang=en" English
#define CITY_ID                     " "  // City ID Number http://openweathermap.org/find
// read OpenWeather api description for more info

// Clock ===================================================================
#define SHOW_ANIM_CLOCK_SW            0
#define UTC_OFFSET                    9  // UTC+9, Coordinated Universal Time(UTC)
#define AM_PM                        12  // AM-PM Mode, 24: 24-hour clock, 12: 12-hour clock
#define ALARM_STATE                   0  // Alarm state setting, 0: alarm off, 1: alarm on
#define ALARM_H_SET                   6  // Hours unit setting
#define ALARM_M_SET                  30  // Minute unit setting
#define ALARM_MESSAGE      "Appointment Time!"  // Alarm scroll message

// =========================================================================
#define UPDATE_TIME_COUNT            20  // default: 10 // every 10 scrolls, ~450s = 7.5m
                                         // 40 scrolls, ~1,800s = 30m (about)
                                         // 80 scrolls, ~3,600s = 60m (about)
                                         // 1920 scrolls, ~86,400s = 24h
#endif
