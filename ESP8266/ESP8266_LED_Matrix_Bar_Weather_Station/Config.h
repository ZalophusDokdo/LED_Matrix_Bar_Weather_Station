/* =========================================================================
 *  Author: Zalophus Dokdo (https://zddh.blogspot.com)
 *  Date: 31/08/2017       (https://zalophus.tistory.com/)
 *  License: GPL v2
 * =========================================================================
 *  LED Matrix Bar Weather Station V1.0.4b (Publish: 2018/01/02)
 * =========================================================================
 *  Configuration
 * =========================================================================
 */

#ifndef CONFIG_H
#define CONFIG_H

//#define USE_UART                           // debug mode 

// DEV Board ===============================================================
//#define DEV_BOARD_ESP01                    // ESP-01
#define DEV_BOARD_WeMos_D1_mini            // WeMos D1 mini or ESP-12E
//#define DEV_BOARD_Witty                    // Witty Cloud Board
//#define DEV_BOARD_WiFi_RELAY_MODULE        // WiFi Relay Module

// LED Matrix ==============================================================
#define USE_LEDMATRIX_CLOCKWEATHER         // Do not... 
//#define USE_LED_MATRIX_DISABLE             // Does not use LED Matrix

// Sensors =================================================================
#ifndef USE_UART
  //#define USE_DS18B20                      // Use DS18B20 OneWire temperature sensor
  //#define USE_DS18B20_A                    // Use another DS18B20 OneWire temperature sensor
#endif
#define USE_DHT                            // Use DHT11, DHT22, DHT21 OneWire temperature and humidity sensor (or DHT11)
#ifdef USE_DHT
  #define DHTTYPE                   DHT11  // DHT11, DHT22(AM2302), DHT21(AM2301)
#endif
//#define USE_DHT12                          // Use DHT12 I2C temperature and humidity sensor

// RGB LED =================================================================
#define USE_RGB_LED_NEOPIXEL               // Use RGB LED WS2812B
//#define USE_RGB_LED_DIRECT                 // USE RGB LED Strip Direct Control - Witty Cloud
//#define USE_RGB_LED_STRIP_DRIVER           // USE RGB LED Strip Driver Module Shield https://github.com/MrKrabat/LED-Strip-Driver-Module

// Control =================================================================
#define USE_LIGHT                          // Use LED Light
#define USE_RELAY                          // Use Relay

// Clock ===================================================================
#define USE_TIMER                          // Use Timer

// Weather Forcast =========================================================
// CHANGE YOUR CONFIG HERE: Weather Host http://api.openweathermap.org
// =========================================================================
#define WEATHER_KEY                   " "  // API Key
#define WEATHER_LANG           "&lang=en"  // "&lang=en" English
#define CITY_ID                       " "  // 1842030 Gunpo, KR - City ID Number http://openweathermap.org/find
// read OpenWeather api description for more info

// Network =================================================================
#define USE_WEB_SERVER                     // Use Web Server
//#define USE_MQTT                           // Use Adafruit IO MQTT
#ifdef  USE_MQTT
  /************************* Adafruit.io Setup **************************/
  #define AIO_SERVER    "io.adafruit.com"
  #define AIO_SERVERPORT             1883  // use 1883 for SSL
  #define AIO_USERNAME                " "  // Replace it with your username
  #define AIO_KEY                     " "  // Replace with your Project Auth Key
  /************************* Adafruit.io ********************************/
#endif

// =========================================================================
// #########################################################################
// =========================================================================
// Important: The settings below are good to understand and change the code.
// =========================================================================
// #########################################################################
// =========================================================================

#define PROJECT_NAME  "LED Matrix Bar Weather Station"  // Project Name
#define PROJECT_VERSION         "v1.0.4b"  // Project Version

#define USE_OTA                            // Use OTA
#define OTA_MODE                        1

// UART ====================================================================
#define SERIAL_SPEED               115200

// WiFi ====================================================================
#define AP_PASSWORD          "0123456789"  // local AP password
#ifdef  USE_LED_MATRIX_DISABLE
  #define mDNS_NAME "HomeIoTLight-XXXXXX"  // mDNS Name: Change only the first 12 characters and keep "-XXXXXX"
#else
  #define mDNS_NAME "LEDMatrixBar-XXXXXX"  // mDNS Name: Change only the first 12 characters and keep "-XXXXXX"
#endif

// WebApp ==================================================================
#define WEB_TITLE       "LED Matrix Bar"  // "LED Matrix Bar", "Workspace Lights", "Livingroom Lights", "Main Room Lights","RGB LED Lights"  // Web Title Name
#define WEB_SUBTITLE    "Weather Station"  // Web Sub-Title Name
#define WEB_PAGE_REFRESH              180  // web page refresh time: 180 = 3 min, 300 = 5 min, 600 = 10 min
#ifdef  USE_LED_MATRIX_DISABLE
  #define ICON_LINK     "https://raw.githubusercontent.com/ZalophusDokdo/LED_Matrix_Bar_Weather_Station/master/images/google_home_5710541_512.png"
  #define FAVICON_LINK  "https://raw.githubusercontent.com/ZalophusDokdo/LED_Matrix_Bar_Weather_Station/master/images/favicon.ico"
#else
  #define ICON_LINK     "https://raw.githubusercontent.com/ZalophusDokdo/LED_Matrix_Bar_Weather_Station/master/images/electronic_clock_1200_811483_512.png"
  #define FAVICON_LINK  "https://raw.githubusercontent.com/ZalophusDokdo/LED_Matrix_Bar_Weather_Station/master/images/favicon_if_home.ico"
#endif
#define WEB_INTERNALTEMPTITLE  "Internal"  // Intenal Temperature Sensor Web Title Name

// Pin Assignment ==========================================================
// LED Matrix for NodeMCU 1.0 (ESP-12E Module) 
// =========================================================================
#ifdef  USE_RGB_LED_DIRECT
  #define RGB_LED_RED_PIN              15  // D8    (GPIO15) RED
  #define RGB_LED_GREEN_PIN            12  // D6    (GPIO12) GREEN
  #define RGB_LED_BLUE_PIN             13  // D7    (GPIO13) BLUE
  #define DIN_PIN                      10  // Null
  #define CS_PIN                       10  // Null
  #define CLK_PIN                      10  // Null
#endif
#ifndef USE_RGB_LED_DIRECT
  #define DIN_PIN                      13  // D7    (GPIO13) DIN
  #define CS_PIN                       15  // D8    (GPIO15) CS
  #define CLK_PIN                      14  // D5    (GPIO14) CLK
#endif
// Data wire
#ifdef  USE_DS18B20
  #define ONE_WIRE_BUS                  1//1  // TXD   (GPIO01) DS18B20
#endif
#define DHTPIN                          2//2  // D4    (GPIO02)
// Battery Power Monitor
#define BATTERY_PIN                    17  // A0    (ADC) Wemos: 3.2V max.
// RGBW LED or Buzzer or Relay or Switch (Alarm On/Off, Lamp, Scroll)
#define RED_PIN                        16  // D0    (GPIO16) Alarm On/Off Switch
#ifdef   USE_RGB_LED_STRIP_DRIVER
  #define GREEN_PIN                    12  // D6    (GPIO12) Lamp Push Button
#endif
#ifndef   USE_RGB_LED_STRIP_DRIVER
  #define GREEN_PIN                     0  // D3    (GPIO00) Lamp Push Button
#endif
#ifdef  USE_RGB_LED_DIRECT
  #define BLUE_PIN                     14  // D5    (GPIO14) Manual Scroll Push Button
#endif
#ifndef USE_RGB_LED_DIRECT
  #define BLUE_PIN                     12  // D6    (GPIO12) Manual Scroll Push Button
#endif
#define WHITE_PIN                       3  // RXD   (GPIO03) Buzzer or Relay or GPIO Output
#ifndef USE_DHT12
  #if     defined(DEV_BOARD_WeMos_D1_mini) || defined(DEV_BOARD_ESP01)
    #define USE_RELAY_PIN               5  // SCL   (GPIO05) Relay           - WeMos D1 mini
    #define USE_RGB_LED_PIN             4  // SDA   (GPIO04) WS2812B RGB LED - WeMos D1 mini
  #endif
  #ifdef DEV_BOARD_Witty
    #define USE_LIGHT_PIN               5  // SCL   (GPIO05) Light - Witty Cloud
    #define USE_RELAY_PIN               4  // SDA   (GPIO04) Relay - Witty Cloud
    #define USE_RGB_LED_PIN             3  // RXD   (GPIO03) WS2812B RGB LED
  #endif
  #ifdef DEV_BOARD_WiFi_RELAY_MODULE
    #define USE_INPUT_PIN               5  // SCL   (GPIO05) Input - WiFi Relay Module
    #define USE_RELAY_PIN               4  // SDA   (GPIO04) Relay - WiFi Relay Module
    #define USE_RGB_LED_PIN             3  // RXD   (GPIO03) WS2812B RGB LED
  #endif
#endif
#ifdef  USE_DHT12
  #ifdef DEV_BOARD_Witty
    #define USE_LIGHT_PIN               3  // RXD   (GPIO03) Light - Witty Cloud
  #endif
  #define USE_RGB_LED_PIN               3  // RXD   (GPIO03) WS2812B RGB LED
  #define USE_RELAY_PIN                 3  // RXD   (GPIO03) Relay
  #ifndef  USE_RGB_LED_PIN
    #define USE_LED_PIN                 3  // RXD   (GPIO03) LED Light
  #endif
#endif
// RGB LED
#define NUMPIXELS                      64  //1  //16  //64  //256
// LEDStripDriver(DIN, CIN); DIN = GPIO02, CIN = GPIO00 in this example
#ifdef  USE_RGB_LED_STRIP_DRIVER
  #define LEDSTRIPDRIVER_CIN            0  //13  //5  // SCL   (GPIO05) CIN - ESP-01
  #define LEDSTRIPDRIVER_DIN            2  //12  //4  // SDA   (GPIO04) DIN - ESP-01
#endif

// LED Matrix Bar Configulation ============================================
// Default status ==========================================================
// =========================================================================
#if     defined(USE_RGB_LED_DIRECT) || defined(DEV_BOARD_ESP01)
#elif   defined(USE_LED_MATRIX_DISABLE)
#else
  #define USE_LED_MATRIX                   // LED Matrix Enable 
#endif
#define NUM_MAX                         6  // Number of MAX7219's connected
#define MAX_DIGITS                     16
#define SET_INTENSITY                   1  // Initial led matrix intensity, 0-15
#define CLOCK                           1
#define DATE_SCROLL                     0
#define WEATHER_SCROLL                  0
#define INDOOR                          1
#define OUTDOOR                         0
#define INDOOR_SCROLL                   0
#define OUTDOOR_SCROLL                  0
#define WAIT_SCROLL                    50  // Waiting after executing web command  
#define WEATHER_SCROLL_INTERVAL     30000  // default: 15000 clock for 15s, then scrolls for about 30s, 60000 = 1m
#define STRING_SHIFT_DELAY             35  // Scroll Speed

// Clock ===================================================================
#define SHOW_ANIM_CLOCK_SW              0
#define UTC_OFFSET                      9  // UTC+9, Coordinated Universal Time(UTC)
#define AM_PM                          12  // AM-PM Mode, 24: 24-hour clock, 12: 12-hour clock
#define ALARM_STATE                     0  // Alarm state setting, 0: alarm off, 1: alarm on
#define ALARM_H_SET                     6  // Hours unit setting
#define ALARM_M_SET                    30  // Minute unit setting
#define ALARM_MESSAGE "Appointment Time!"  // Alarm scroll message
#define OnTIME_MESSAGE      "Break Time!"  // Alarm On-Time scroll message
#define TIMER_STATE                     0  // Timer state setting, 0: Timer off, 1: Timer on
#define TIMER_H_START                   6  // Start hours unit setting
#define TIMER_M_START                  30  // Start minute unit setting
#define TIMER_H_STOP                   12  // Stop hours unit setting
#define TIMER_M_STOP                   30  // Stop minute unit setting
#define TIMER_START_MESSAGE "Timer Start"  // Timer start scroll message
#define TIMER_STOP_MESSAGE   "Timer Stop"  // Timer stop scroll message

// =========================================================================
#define UPDATE_TIME_COUNT              20  // default: 10 // every 10 scrolls, ~450s = 7.5m
                                           // 40 scrolls, ~1,800s = 30m (about)
                                           // 80 scrolls, ~3,600s = 60m (about)
                                           // 1920 scrolls, ~86,400s = 24h
#endif
