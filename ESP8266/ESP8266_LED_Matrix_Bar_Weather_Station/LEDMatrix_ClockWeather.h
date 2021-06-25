/* =========================================================================
 *  Author: Zalophus Dokdo (https://zddh.blogspot.com)
 *  Date: 31/08/2017       (https://zalophus.tistory.com/)
 *  License: GPL v2
 * =========================================================================
 *  LED Matrix Bar Weather Station V1.0.4 (Publish: 2018/01/02)
 * =========================================================================
 *  LED Matrix Bar Interface
 * =========================================================================
 */

#ifndef LEDMATRIX_CLOCKWEATHER_H
#define LEDMATRIX_CLOCKWEATHER_H

#include <ArduinoJson.h>
#include "NTPClient.h"

WiFiClient client;

const char *projectName     = PROJECT_NAME;
const char *projectVersion  = PROJECT_VERSION;

int    stringShiftDelay     = STRING_SHIFT_DELAY;
int    updateTimeCount      = UPDATE_TIME_COUNT;

String weatherMain          = "";
String weatherDescription   = "";
String weatherLocation      = "";
String country;
int    humidity;
int    pressure;
float  temp;
float  tempMin, tempMax;
int    clouds;
float  windSpeed;

String date;
String weatherString;

#include "MAX7219.h"
#include "Fonts.h"

// =======================================================================
String weatherKey           = WEATHER_KEY;
String weatherLang          = WEATHER_LANG;
String cityID               = CITY_ID;
// read OpenWeather api description for more info
// =======================================================================

// =======================================================================
int    setIntensity         =   SET_INTENSITY;
byte   dig[MAX_DIGITS]      =  {0};
byte   digold[MAX_DIGITS]   =  {0};
byte   digtrans[MAX_DIGITS] =  {0};
int    updCnt               =   0;
int    dots                 =   0;
long   dotTime              =   0;
long   clkTime              =   0;
int    dx                   =   0;
int    dy                   =   0;
byte   del                  =   0;
int    h,m,s;
// =======================================================================
int    am_pm                =   AM_PM;
float  utcOffset            =   UTC_OFFSET;
long   localEpoc            =   0;
long   localMillisAtUpdate  =   0;
// =======================================================================
int    showAnimClockSW      =   SHOW_ANIM_CLOCK_SW;
// =======================================================================
int showChar(char ch, const uint8_t *data) {
  int len  = pgm_read_byte(data);
  int i, w = pgm_read_byte(data + 1 + ch * len);
  for (i = 0; i < w; i++)
    scr[NUM_MAX*8 + i] = pgm_read_byte(data + 1 + ch * len + 1 + i);
    scr[NUM_MAX*8 + i] = 0;
  return w;
}

// =======================================================================
int dualChar = 0;

unsigned char convertPolish(unsigned char _c) {
  unsigned char c = _c;
  if(c == 196 || c == 197 || c == 195) {
    dualChar = c;
    return 0;
  }
  if(dualChar) {
    switch(_c) {
      case 133: c =  1+'~'; break; // 'ą'
      case 135: c =  2+'~'; break; // 'ć'
      case 153: c =  3+'~'; break; // 'ę'
      case 130: c =  4+'~'; break; // 'ł'
      case 132: c = dualChar==197 ? 5+'~' : 10+'~'; break; // 'ń' and 'Ą'
      case 179: c =  6+'~'; break; // 'ó'
      case 155: c =  7+'~'; break; // 'ś'
      case 186: c =  8+'~'; break; // 'ź'
      case 188: c =  9+'~'; break; // 'ż'
    //case 132: c = 10+'~'; break; // 'Ą'
      case 134: c = 11+'~'; break; // 'Ć'
      case 152: c = 12+'~'; break; // 'Ę'
      case 129: c = 13+'~'; break; // 'Ł'
      case 131: c = 14+'~'; break; // 'Ń'
      case 147: c = 15+'~'; break; // 'Ó'
      case 154: c = 16+'~'; break; // 'Ś'
      case 185: c = 17+'~'; break; // 'Ź'
      case 187: c = 18+'~'; break; // 'Ż'
      default:  break;
    }
    dualChar = 0;
    return c;
  }    
  switch(_c) {
    case 185: c =  1+'~'; break;
    case 230: c =  2+'~'; break;
    case 234: c =  3+'~'; break;
    case 179: c =  4+'~'; break;
    case 241: c =  5+'~'; break;
    case 243: c =  6+'~'; break;
    case 156: c =  7+'~'; break;
    case 159: c =  8+'~'; break;
    case 191: c =  9+'~'; break;
    case 165: c = 10+'~'; break;
    case 198: c = 11+'~'; break;
    case 202: c = 12+'~'; break;
    case 163: c = 13+'~'; break;
    case 209: c = 14+'~'; break;
    case 211: c = 15+'~'; break;
    case 140: c = 16+'~'; break;
    case 143: c = 17+'~'; break;
    case 175: c = 18+'~'; break;
    default:  break;
  }
  return c;
}

// =======================================================================
void showDigit(char ch, int col, const uint8_t *data) {
  if(dy < -8 | dy > 8) return;
  int len = pgm_read_byte(data);
  int w   = pgm_read_byte(data + 1 + ch * len);
  col += dx;
  for (int i = 0; i < w; i++)
    if(col + i >= 0 && col + i < 8 * NUM_MAX) {
      byte v = pgm_read_byte(data + 1 + ch * len + 1 + i);
      if(!dy) scr[col + i] = v; else scr[col + i] |= dy > 0 ? v >> dy : v << -dy;
    }
}

// =======================================================================
void setCol(int col, byte v) {
  if(dy < -8 | dy > 8) return;
  col += dx;
  if(col >= 0 && col < 8 * NUM_MAX)
    if(!dy) scr[col] = v; else scr[col] |= dy > 0 ? v >> dy : v << -dy;
}

// =======================================================================
void showSimpleClock() {
  dx = dy = 0;
  clr();
  showDigit(h/10,  0, dig6x8); 
  showDigit(h%10,  8, dig6x8);
  showDigit(m/10, 17, dig6x8);
  showDigit(m%10, 25, dig6x8);
  showDigit(s/10, 34, dig6x8);
  showDigit(s%10, 42, dig6x8);
  setCol(15,dots ? B00100100 : 0);
  setCol(32,dots ? B00100100 : 0);
/*  Number of MAX7219's connected
  showDigit(h/10,  8, dig6x8); 
  showDigit(h%10, 16, dig6x8);
  showDigit(m/10, 25, dig6x8);
  showDigit(m%10, 33, dig6x8);
  showDigit(s/10, 42, dig6x8);
  showDigit(s%10, 50, dig6x8);
  setCol(23,dots ? B00100100 : 0);
  setCol(40,dots ? B00100100 : 0);
 */
  refreshAll();
}

// =======================================================================
void showAnimClock() {
  byte digPos[6] = {0,8,17,25,34,42};
  int digHt      = 12;
  int num        =  6; 
  int i;
  if(del == 0) {
    del = digHt;
    for(i = 0; i < num; i++) digold[i] = dig[i];
    dig[0] = h/10 ? h/10 : 10;
    dig[1] = h%10;
    dig[2] = m/10;
    dig[3] = m%10;
    dig[4] = s/10;
    dig[5] = s%10;
    for(i = 0; i < num; i++)  digtrans[i] = (dig[i] == digold[i]) ? 0 : digHt;
  } else
    del--;
  
  clr();
  for(i = 0; i < num; i++) {
    if(digtrans[i] == 0) {
      dy = 0;
      showDigit(dig[i], digPos[i], dig6x8);
    } else {
      dy = digHt-digtrans[i];
      showDigit(digold[i], digPos[i], dig6x8);
      dy = -digtrans[i];
      showDigit(dig[i], digPos[i], dig6x8);
      digtrans[i]--;
    }
  }
  dy = 0;
  setCol(15,dots ? B00100100 : 0);
  setCol(32,dots ? B00100100 : 0);
  refreshAll();
  delay(30);
}

// =======================================================================
void printCharWithShift(unsigned char c, int shiftDelay) {
  c = convertPolish(c);
  if (c < ' ' || c > '~' + 25) return;
  c -= 32;
  int w = showChar(c, font);
  for (int i = 0; i < w + 1; i++) {
    delay(shiftDelay);
    scrollLeft();
    refreshAll();
  }
}

// =======================================================================
void printStringWithShift(const char* s, int shiftDelay) {
  while (*s) {
    printCharWithShift(*s, shiftDelay);
    s++;
  }
}

// =======================================================================
const char *ntpHost = "pool.ntp.org";
// =======================================================================
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpHost);
//Week Days
String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//Month names
String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
String ampm;

void ntpTime_start() {
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600, GMT +9 = 32400, GMT -1 = -3600, GMT 0 = 0
  timeClient.setTimeOffset(3600 * utcOffset);  // 
}

void ntpTime() {
  timeClient.update();

  int day = timeClient.getDay();
  String date_time; 
  date_time = timeClient.getFormattedDate();
  int index_date = date_time.indexOf("T");
  date = "     " + date_time.substring(0, index_date) +  " " + weekDays[day] + "     ";

  h = timeClient.getHours();
  m = timeClient.getMinutes();
  s = timeClient.getSeconds();
  if (am_pm == 12) {
    if (h > 12) {
      h = h - 12;
      ampm = "PM";
    } else {
      ampm = "AM";
    }
  } else {
    ampm = "";
  }
  localMillisAtUpdate = millis();
  localEpoc = (h * 60 * 60 + m * 60 + s);
}

// =======================================================================
const char *weatherHost = "api.openweathermap.org";
// =======================================================================
void getWeatherData() {
#ifdef USE_UART
  Serial.print("connecting to "); Serial.println(weatherHost);
#endif
  if (client.connect(weatherHost, 80)) {
    client.println(String("GET /data/2.5/weather?id=") +
                   cityID + "&units=metric&appid=" +
                   weatherKey + weatherLang + "\r\n" +
                   "Host: " + weatherHost + "\r\n" +
                   "User-Agent: ArduinoWiFi/1.1\r\n" +
                   "Connection: close\r\n\r\n");
  } else {
#ifdef USE_UART
    Serial.println("connection failed");
#endif
    return;
  }
  int repeatCounter = 0;
  while (!client.available() && repeatCounter < 10) {
    delay(500);
#ifdef USE_UART
    Serial.println("w.");
#endif
    repeatCounter++;
  }
  String line;
  while (client.connected() && client.available()) {
    char c = client.read(); 
    if (c == '[' || c == ']') c = ' ';
    line += c;
  }
  client.stop();

  DynamicJsonBuffer jsonBuf;
  JsonObject &root = jsonBuf.parseObject(line);
  if (!root.success())
  {
#ifdef USE_UART
    Serial.println("parseObject() failed");
#endif
    return;
  }
  //weatherMain = root["weather"]["main"].as<String>();
  weatherDescription = root["weather"]["description"].as<String>();
  weatherDescription.toLowerCase();
  //weatherLocation = root["name"].as<String>();
  country        = root["sys"]["country"].as<String>();
  temp           = root["main"]["temp"];
  humidity       = root["main"]["humidity"];
  pressure       = root["main"]["pressure"];
  tempMin        = root["main"]["temp_min"];
  tempMax        = root["main"]["temp_max"];
  windSpeed      = root["wind"]["speed"];
  clouds         = root["clouds"]["all"];
  String deg     = String(char('~'+25));
  weatherString  = "         Temp: " + String(temp,1) + deg + "C (" + String(tempMin,1) + deg + "-" + String(tempMax,1) + deg + ")  ";
  weatherString += weatherDescription;
  weatherString += "  Humidity: " + String(humidity) + "%  ";
  weatherString += "  Pressure: " + String(pressure) + "hPa  ";
  weatherString += "  Clouds: " + String(clouds) + "%  ";
  weatherString += "  Wind: " + String(windSpeed,1) + "m/s                 ";
}

// =======================================================================
void updateTime()
{
  long curEpoch = localEpoc + ((millis() - localMillisAtUpdate) / 1000);
  //long epoch    = round(curEpoch + 3600 * utcOffset + 86400L) % 86400L;  // gClock
  long epoch    = round(curEpoch + 86400L) % 86400L;  // NTP
  h             = ((epoch % 86400L) / 3600) % 24;  // 24-hour clock, 12-hour clock
  m             = (epoch % 3600) / 60;
  s             = epoch % 60;
}
// =======================================================================

void LEDMatrix_ClockWeather_setup() {
  initMAX7219();
  sendCmdAll(CMD_SHUTDOWN,  1);
  sendCmdAll(CMD_INTENSITY, setIntensity);
#ifdef USE_UART
  Serial.print("Connecting WiFi ");
#endif
#ifdef USE_LED_MATRIX
  printStringWithShift(projectName, 15);
  delay(1000);
  printStringWithShift("        ", 15);
  printStringWithShift(projectVersion, 15);
  delay(1000);
  printStringWithShift("                ", 15);
  delay(1000);
  printStringWithShift("Connecting    ", 20);
  char result[16];
  sprintf(result, "%3d.%3d.%1d.%3d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
  printStringWithShift(result, 25);
#endif
  ntpTime_start();
}

#ifdef USE_DHT
void LEDMatrix_DHT_action() {
  char dhtCTemp[10]    = "";
  char dhtHumidity[10] = "";
  dtostrf(dht.readTemperature(), 3, 1, dhtCTemp);
  dtostrf(dht.readHumidity(), 3, 1, dhtHumidity);
  String deg = String(char('~'+25));
  if(dhtGet == 0) {
  printStringWithShift("                ", stringShiftDelay);
  printStringWithShift("INDOOR", stringShiftDelay);
  printStringWithShift("   ", stringShiftDelay);
  printStringWithShift("Temp: ", stringShiftDelay);
  printStringWithShift(dhtCTemp, stringShiftDelay);
  printStringWithShift(deg.c_str(), stringShiftDelay);
  printStringWithShift("C", stringShiftDelay);
  printStringWithShift("   ", stringShiftDelay);
  printStringWithShift("Humidity: ", stringShiftDelay);
  printStringWithShift(dhtHumidity, stringShiftDelay);
  printStringWithShift("%   ", stringShiftDelay);
  printStringWithShift("                ", stringShiftDelay);
  }
}
#endif
#ifdef USE_DHT12
void LEDMatrix_DHT12_action() {
  char dht12cTemp[10]    = "";
  char dht12Humidity[10] = "";
  dtostrf(dht12.readTemperature(), 3, 1, dht12cTemp);
  dtostrf(dht12.readHumidity(), 3, 1, dht12Humidity);
  String deg = String(char('~'+25));
  if(dht12get == 0) {
  printStringWithShift("                ", stringShiftDelay);
  printStringWithShift("OUTDOOR", stringShiftDelay);
  printStringWithShift("   ", stringShiftDelay);
  printStringWithShift("Temp: ", stringShiftDelay);
  printStringWithShift(dht12cTemp, stringShiftDelay);
  printStringWithShift(deg.c_str(), stringShiftDelay);
  printStringWithShift("C", stringShiftDelay);
  printStringWithShift("   ", stringShiftDelay);
  printStringWithShift("Humidity: ", stringShiftDelay);
  printStringWithShift(dht12Humidity, stringShiftDelay);
  printStringWithShift("%   ", stringShiftDelay);
  printStringWithShift("                ", stringShiftDelay);
  }
}
#endif

void LEDMatrix_Date_action() {
  printStringWithShift(date.c_str(),          stringShiftDelay);
}

void LEDMatrix_Weather_action() {
  printStringWithShift(weatherString.c_str(), stringShiftDelay);
}

void LEDMatrix_Clock_action() {
  if(millis() - dotTime > 500) {
    dotTime = millis();
    dots    = !dots;
  }
  updateTime();
  if (showAnimClockSW == 0) {
    showSimpleClock();
  } else {
    showAnimClock();
  }
}

void LEDMatrix_action() {
  if(updCnt <= 0) {
    updCnt = updateTimeCount;  // default: 10, every 10 scrolls, ~450s = 7.5m // 80 scrolls, ~3600s = 60m
#ifdef USE_UART
    Serial.println("Getting data ...");
#endif
#ifdef USE_LED_MATRIX
    printStringWithShift("   Getting data", 15);
#endif
    ntpTime();
    getWeatherData();
#ifdef USE_UART
    Serial.println("Data loaded");
#endif
    clkTime = millis();
  }
}

#endif
