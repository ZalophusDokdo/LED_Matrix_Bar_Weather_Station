/* =========================================================================
 *  Author: Zalophus Dokdo (https://zddh.blogspot.com)
 *  Date: 31/08/2017
 *  License: GPL v2
 * =========================================================================
 *  LED Matrix Bar Weather Station
 * =========================================================================
 *  WeMos D1 mini Web Server
 * =========================================================================
 */
#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <ESP8266WebServer.h>

ESP8266WebServer server(80);  // Set the port you wish to use, a browser default is 80, but any port can be used, if you set it to 5555 then connect with http://nn.nn.nn.nn:5555/

int OTAmode              =   OTA_MODE;
int ledMatrix            =   LED_MATRIX;
int gclockWeatherScroll  =   GCLOCK_WEATHER_SCROLL;
int gClock               =   GCLOCK;
int dateScroll           =   DATE_SCROLL;
int weatherScroll        =   WEATHER_SCROLL;
int indoor               =   INDOOR;
int outdoor              =   OUTDOOR;
int indoorScroll         =   INDOOR_SCROLL;
int outdoorScroll        =   OUTDOOR_SCROLL;
int waitScroll           =   WAIT_SCROLL;

extern "C" {
  uint16 readvdd33(void);
}

String deg               = String(char('~'+25));
String webpage           = "";
String printMsg;

void append_webpage_header() {
  // webpage is a global variable
  webpage = ""; // A blank string variable to hold the web page
  webpage += "<!DOCTYPE HTML> ";
  webpage += "<html> ";
  webpage += "<head> ";
  //webpage += "<meta http-equiv='refresh' content='300;url='/'' />";  // 300 = 5 min
  webpage += "<meta name='apple-mobile-web-app-capable' content='yes' />";
  webpage += "<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />";
  webpage += "  <title>LED Matrix Bar - Zalophus's DesignHouse</title>";
  webpage += "  <meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  webpage += "  <style>";
  webpage += "    #jb-container { width: 940px; margin: 0px auto; padding: 10px; border: 1px solid #bcbcbc; }";
  webpage += "    #jb-header { padding: 0px; margin-bottom: 10px; border: 1px solid #bcbcbc; }";
  webpage += "    #jb-content { width: 580px; padding: 10px; margin-bottom: 10px; float: left; border: 1px solid #bcbcbc; }";
  webpage += "    #jb-sidebar { width: 310px; padding: 10px; margin-bottom: 10px; float: right; border: 1px solid #bcbcbc; }";
  webpage += "    #jb-footer { clear: both; padding: 10px; border: 1px solid #bcbcbc; }";
  webpage += "    @media screen and (max-width:480px) {";
  webpage += "      #jb-container { width: auto; }";
  webpage += "      #jb-content { float: none; width: auto; }";
  webpage += "      #jb-sidebar { float: none; width: auto; }";
  webpage += "    }";
  //* Button style */
  webpage += "    button::-moz-focus-inner,";
  webpage += "    input::-moz-focus-inner { border: 0; padding: 0; }";
  webpage += "    .ct-btn { display: inline-block; margin: 5px 0; padding: .5em .75em; border-radius: .25em; box-sizing: content-box; -moz-box-sizing: content-box; background: transparent; outline: 0; vertical-align: middle; font-family: inherit; font-size: 18px; text-decoration: none; white-space: nowrap; cursor: pointer }";
  webpage += "    .en-01 { font-family: 'Helvetica Neue', Helvetica, Arial, sans-serif; font-size: 14px; font-weight: bold; }";
  //* Red, Green, Blue, Yellow */
  webpage += "    .ct-btn.red,";
  webpage += "    .ct-btn.red:link,";
  webpage += "    .ct-btn.red:visited { color: #fff; text-shadow: 0 1px 0 #991e1a; border: 1px solid #991e1a; border-top: 1px solid #c06f01; box-shadow: 0px 2px 2px 0px rgba(0, 0, 0, 0.20); background: #b22520; background: linear-gradient(to bottom, #e8605a 0%, #b22520 100%); filter: progid:DXImageTransform.Microsoft.gradient( startColorstr='#e8605a', endColorstr='#b22520', GradientType=0 ); }";
  webpage += "    .ct-btn.red:hover { background: #ca3631; background: linear-gradient(to bottom, #f17771 0%, #ca3631 100%); filter: progid:DXImageTransform.Microsoft.gradient( startColorstr='#ffdc78', endColorstr='ffb618', GradientType=0 ); }";
  webpage += "    .ct-btn.red:active,";
  webpage += "    .ct-btn.red.active { border-color: #a0211d; border-top-color: #7f1b17; background: #b22520; box-shadow: none; text-shadow: none; }";
  webpage += "    .ct-btn.green,";
  webpage += "    .ct-btn.green:link,";
  webpage += "    .ct-btn.green:visited { color: #fff; text-shadow: 0 1px 0px #135fa4; border: 1px solid #368321; border-top-color: #439f2a; box-shadow: 0px 2px 2px 0px rgba(0, 0, 0, 0.22); background: #76ac41; background: linear-gradient(to bottom, #76ac41 0%, #489215 100%); filter: progid:DXImageTransform.Microsoft.gradient( startColorstr='#76ac41', endColorstr='#489215', GradientType=0 ); }";
  webpage += "    .ct-btn.green:hover { background: #4c9b16; background: linear-gradient(to bottom, #7ab243 0%, #4c9b16 100%); filter: progid:DXImageTransform.Microsoft.gradient( startColorstr='#7ab243', endColorstr='#4c9b16', GradientType=0 ); }";
  webpage += "    .ct-btn.green:active,";
  webpage += "    .ct-btn.green.active { border-color: #6c9d3c; border-top-color: #368321; background: #4c9b16; box-shadow: none; text-shadow: none; }";
  webpage += "    .ct-btn.blue,";
  webpage += "    .ct-btn.blue:link,";
  webpage += "    .ct-btn.blue:visited { color: #fff; text-shadow: 0 1px 0px #135fa4; border: 1px solid #135fa4; border-top-color: #2d9acf; box-shadow: 0px 2px 2px 0px rgba(0, 0, 0, 0.22); background: #3aaee7; background: linear-gradient(to bottom, #3aaee7 0%, #1a80d9 100%); filter: progid:DXImageTransform.Microsoft.gradient( startColorstr='#3aaee7', endColorstr='#1a80d9', GradientType=0 ); }";
  webpage += "    .ct-btn.blue:hover { background: #50bae9; background: linear-gradient(to bottom, #50bae9 0%, #3392de 100%); filter: progid:DXImageTransform.Microsoft.gradient( startColorstr='#50bae9', endColorstr='#3392de', GradientType=0 ); }";
  webpage += "    .ct-btn.blue:active,";
  webpage += "    .ct-btn.blue.active { border-color: #2585d9; border-top-color: #0f4678; background: #2d90e7; box-shadow: none }";
  webpage += "    .ct-btn.yellow,";
  webpage += "    .ct-btn.yellow:link,";
  webpage += "    .ct-btn.yellow:visited { color: #000; text-shadow: 0 1px 0 #fcda7c; border: 1px solid #dfac23; border-bottom: 1px solid #c06f01; box-shadow: 0px 2px 2px 0px rgba(0, 0, 0, 0.10); background: #f5a802; background: linear-gradient(to bottom, #fcd054 0%, #f5a802 100%); filter: progid:DXImageTransform.Microsoft.gradient( startColorstr='#fcd054', endColorstr='#f5a802', GradientType=0 ); }";
  webpage += "    .ct-btn.yellow:hover { background: #ffb618; background: linear-gradient(to bottom, #ffdc78 0%, #ffb618 100%); filter: progid:DXImageTransform.Microsoft.gradient( startColorstr='#ffdc78', endColorstr='ffb618', GradientType=0 ); }";
  webpage += "    .ct-btn.yellow:active,";
  webpage += "    .ct-btn.yellow.active { border-color: #efa80d; border-top-color: #cf8e00; background: #ffb618; box-shadow: none; text-shadow: none; }";
  webpage += "  </style>";
  webpage += "</head>";
}

void update_webpage() {
  append_webpage_header();
  IPAddress ip = WiFi.localIP();
  String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
  // Return the response
  webpage += "<body>";
  webpage += "  <div id='jb-container'>";
  webpage += "    <div id='jb-header'>";
  webpage += "      <center><a style='TEXT-DECORATION: none' href='./'><h1>LED Matrix Bar<br>Weather Station</h1></a></center>";
  webpage += "    </div>";
  webpage += "    <div id='jb-content'>";
  webpage += "      <table align='center' width='100%'>";
  webpage += "        <tr align='center'>";
  webpage += "          <td>";
  webpage += "<b>[ Weather Forecast ]</b><br>";
  webpage += "<a href=\"/\"\"><button type='submit' class='ct-btn blue en-01' style='font-size: 26px'>" + weatherDescription + "</button></a>";
  webpage += "          </td>";
  webpage += "        </tr>";
  webpage += "        <tr align='center'>";
  webpage += "          <td>";
  webpage += "<button type='submit' class='ct-btn white en-01' style='font-size: 14px'>";
  webpage += date + "<br>";
  webpage += "[ " + String(int(h)) + ":" + String(int(m)) + " ]<br>";
  webpage += "Temp    : " + String(temp,1) + "'C ( " + String(tempMin,1) + "'C -" + String(tempMax,1) + "'C ) <br>";
  webpage += "Humidity: " + String(humidity) + "%  <br>";
  webpage += "Pressure: " + String(pressure) + "hPa  <br>";
  webpage += "Clouds  : " + String(clouds) + "%  <br>";
  webpage += "Wind    : " + String(windSpeed,1) + "m/s <br>";
  webpage += "</button><br>";
  webpage += "<a href=\"/repeatprintMsg\"\"><button class='ct-btn white en-01' style='font-size: 12px'>";
  webpage += printMsg;
  webpage += "</button></a>";
  webpage += "            <br>";
  webpage += "            Type your message<br>";
  webpage += "            <form action='msg'>";
  webpage += "            <input type='text' name='msg' size=35% autofocus><br>";
  webpage += "            <input class='ct-btn blue en-01' style='font-size: 26px' type='submit' value='Submit'>";
  webpage += "            </form>";
  webpage += "          </td>";
  webpage += "        </tr>";
  webpage += "      </table>";
  if(dht12get == 0 || dht22get == 0) {
    webpage += "    <table align='center' width='100%'>";
    webpage += "      <tr align='center'>";
#ifdef USE_DHT22
    if (indoor == 1) {
      if(dht22get == 0) {
        webpage += "    <td>";
        webpage += " < ";
        webpage += dht22cTemp;
        webpage += " 'C >";
        webpage += "    </td>";
      }
    }
#endif
#ifdef USE_DHT12
    if (outdoor == 1) {
      if(dht12get == 0) {
        webpage += "    <td>";
        webpage += " < ";
        webpage += dht12.readTemperature();
        webpage += " 'C >";
        webpage += "    </td>";
      }
    }
#endif
#ifdef USE_DHT22
    if (indoor == 1) {
      webpage += "      <td><b>INDOOR</b>";
      webpage += "      </td>";
    }
#endif
#ifdef USE_DHT12
    if (outdoor == 1) {
      webpage += "      <td><b>OUTDOOR</b>";
      webpage += "      </td>";
    }
#endif
#ifdef USE_DHT22
    if (indoor == 1) {
      if(dht22get == 0) {
        webpage += "    <td>";
        webpage += " < ";
        webpage += dht22Humidity;
        webpage += " % >";
        webpage += "    </td>";
      }
    }
#endif
#ifdef USE_DHT12
    if (outdoor == 1) {
      if(dht12get == 0) {
        webpage += "    <td>";
        webpage += " < ";
        webpage += dht12.readHumidity();
        webpage += " % >";
        webpage += "    </td>";
      }
    }
#endif
    webpage += "      </tr>";
    webpage += "    </table>";
    webpage += "    <table align='center' width='100%'>";
    webpage += "      <tr align='center'>";
    webpage += "        <td>";
    webpage += "          <table align='center' width='100%'>";
    webpage += "            <tr align='center'>";
#ifdef USE_DHT22
    if (indoor == 1) {
      webpage += "              <td>";
      if (dht.readTemperature() > 35 && dht.readHumidity() > 60) { 
        webpage += "<a href=\"/outDoor\"\"><button type='submit' class='ct-btn red en-01' style='font-size: 44px'>-- Hot --</button></a>";
      } else if (dht.readTemperature() > 29 && dht.readHumidity() > 40) { 
        webpage += "<a href=\"/outDoor\"\"><button type='submit' class='ct-btn yellow en-01' style='font-size: 44px'>- Warm -</button></a>";
      } else if (dht.readTemperature() < 20 && dht.readHumidity() < 30) {
        webpage += "<a href=\"/outDoor\"\"><button type='submit' class='ct-btn blue en-01' style='font-size: 44px'>- Cold -</button></a>";
      } else {
        webpage += "<a href=\"/outDoor\"\"><button type='submit' class='ct-btn green en-01' style='font-size: 44px'>- Good -</button></a>";
      }
      webpage += "              </td>";
    }
#endif
#ifdef USE_DHT12
    if (outdoor == 1) {
      webpage += "              <td>";
      if (dht12.readTemperature() > 35 && dht12.readHumidity() > 60) { 
        webpage += "<a href=\"/inDoor\"\"><button type='submit' class='ct-btn red en-01' style='font-size: 44px'>-- Hot --</button></a>";
      } else if (dht12.readTemperature() > 29 && dht12.readHumidity() > 40) { 
        webpage += "<a href=\"/inDoor\"\"><button type='submit' class='ct-btn yellow en-01' style='font-size: 44px'>- Warm -</button></a>";
      } else if (dht12.readTemperature() < 20 && dht12.readHumidity() < 30) {
        webpage += "<a href=\"/inDoor\"\"><button type='submit' class='ct-btn blue en-01' style='font-size: 44px'>- Cold -</button></a>";
      } else {
        webpage += "<a href=\"/inDoor\"\"><button type='submit' class='ct-btn green en-01' style='font-size: 44px'>- Good -</button></a>";
      }
      webpage += "              </td>";
    }
#endif
    webpage += "            </tr>";
    webpage += "          </table>";
    webpage += "        </td>";
    webpage += "      </tr>";
    webpage += "    </table>";
  } else if(dht12get == 1 && dht22get == 1) {
  }
  webpage += "      <table align='center' width='100%'>";
  webpage += "        <tr align='center'>";
  webpage += "          <td>";
  if (showAnimClockSW == 0) {
    webpage += "<a href=\"/showAnimClockOn\"\"><button class='ct-btn green en-01' style='font-size: 12px'>- Show Animation Clock -</button></a>";
  } else {
    webpage += "<a href=\"/showAnimClockOff\"\"><button class='ct-btn yellow en-01' style='font-size: 12px'>- Show Simple Clock -</button></a>";
  }
  webpage += "          </td>";
  webpage += "        </tr>";
  webpage += "      </table>";
  webpage += "      <table align='center' width='100%'>";
  webpage += "        <tr align='center'>";
  webpage += "          <td>";
  if (dateScroll == 0) {
    webpage += "<a href=\"/dateScrollOn\"\"><button class='ct-btn blue en-01' style='font-size: 12px'>----------<br>Scrolling<br>Date<br>----------</button></a>";
  } else {
    webpage += "<a href=\"/dateScrollOff\"\"><button class='ct-btn yellow en-01' style='font-size: 12px'>----------<br>Scrolling<br>Date<br>----------</button></a>";
  }
  webpage += "          </td>";
  webpage += "          <td>";
  if (gClock == 0) {
    webpage += "<a href=\"/gClockOn\"\"><button class='ct-btn red en-01' style='font-size: 12px'>----------<br>gClock<br>On<br>----------</button></a>";
  } else {
    webpage += "<a href=\"/gClockOff\"\"><button class='ct-btn green en-01' style='font-size: 12px'>----------<br>gClock<br>Off<br>----------</button></a>";
  }
  webpage += "          </td>";
  webpage += "          <td>";
  if (weatherScroll == 0) {
    webpage += "<a href=\"/weatherScrollOn\"\"><button class='ct-btn blue en-01' style='font-size: 12px'>----------<br>Scrolling<br>Weather<br>----------</button></a>";
  } else {
    webpage += "<a href=\"/weatherScrollOff\"\"><button class='ct-btn yellow en-01' style='font-size: 12px'>----------<br>Scrolling<br>Weather<br>----------</button></a>";
  }
  webpage += "          </td>";
  webpage += "        </tr>";
  if(dht12get == 0 || dht22get == 0) {
    webpage += "      <tr align='center'>";
    webpage += "        <td>";
    if (indoorScroll == 0) {
      webpage += "<a href=\"/inDoorScrollOn\"\"><button class='ct-btn blue en-01' style='font-size: 12px'>----------<br>Scrolling<br>INDOOR<br>On<br>----------</button></a>";
    } else {
      webpage += "<a href=\"/inDoorScrollOff\"\"><button class='ct-btn yellow en-01' style='font-size: 12px'>----------<br>Scrolling<br>INDOOR<br>Off<br>----------</button></a>";
    }
    webpage += "        </td>";
    webpage += "        <td>";
    webpage += "        </td>";
    webpage += "        <td>";
    if (outdoorScroll == 0) {
      webpage += "<a href=\"/outDoorScrollOn\"\"><button class='ct-btn blue en-01' style='font-size: 12px'>----------<br>Scrolling<br>OUTDOOR<br>On<br>----------</button></a>";
    } else {
      webpage += "<a href=\"/outDoorScrollOff\"\"><button class='ct-btn yellow en-01' style='font-size: 12px'>----------<br>Scrolling<br>OUTDOOR<br>Off<br>----------</button></a>";
    }
    webpage += "        </td>";
    webpage += "      </tr>";
  } else if(dht12get == 1 && dht22get == 1) {
  }
  webpage += "        <tr align='center'>";
  webpage += "          <td><a href=\"/button00\"\"><button class='ct-btn white en-01' style='font-size: 12px'>----------<br>I<br>love<br>you<br>----------</button></a></td>";
  webpage += "          <td><a href=\"/button01\"\"><button class='ct-btn white en-01' style='font-size: 12px'>----------<br>Don't<br>get<br>upset<br>----------</button></a></td>";
  webpage += "          <td><a href=\"/button02\"\"><button class='ct-btn white en-01' style='font-size: 12px'>----------<br>I'm<br>upset<br><br>----------</button></a></td>";
  webpage += "        </tr>";
  webpage += "        <tr align='center'>";
  webpage += "          <td><a href=\"/button03\"\"><button class='ct-btn white en-01' style='font-size: 12px'>----------<br>Catch me<br>if<br>you can<br>----------</button></a></td>";
  webpage += "          <td><a href=\"/button04\"\"><button class='ct-btn white en-01' style='font-size: 12px'>----------<br>You will<br>never know<br>until you try<br>----------</button></a></td>";
  webpage += "          <td><a href=\"/button05\"\"><button class='ct-btn white en-01' style='font-size: 12px'>----------<br>Pain past<br>is<br>pleasure<br>----------</button></a></td>";
  webpage += "        </tr>";
  webpage += "        <tr align='center'>";
  webpage += "          <td>";
  if (digitalRead(16)) { 
    webpage += "<a href=\"/light16off\"\"><button type='submit' class='ct-btn yellow en-01' style='font-size: 14px'>----------<br>Red<br>Off<br>----------</button></a>";
  } else {
    webpage += "<a href=\"/light16on\"\"><button type='submit' class='ct-btn red en-01' style='font-size: 14px'>----------<br>Red<br>On<br>----------</button></a>";
  };
  webpage += "          </td>";
  webpage += "          <td>";
  if (digitalRead(0)) { 
    webpage += "<a href=\"/light00off\"\"><button type='submit' class='ct-btn yellow en-01' style='font-size: 14px'>----------<br>Green<br>Off<br>----------</button></a>";
  } else {
    webpage += "<a href=\"/light00on\"\"><button type='submit' class='ct-btn green en-01' style='font-size: 14px'>----------<br>Green<br>On<br>----------</button></a>";
  }
  webpage += "          </td>";
  webpage += "          <td>";
  if (digitalRead(12)) { 
    webpage += "<a href=\"/light12off\"\"><button type='submit' class='ct-btn yellow en-01' style='font-size: 14px'>----------<br>Blue<br>Off<br>----------</button></a>";
  } else {
    webpage += "<a href=\"/light12on\"\"><button type='submit' class='ct-btn blue en-01' style='font-size: 14px'>----------<br>Blue<br>On<br>----------</button></a>";
  };
  webpage += "          </td>";
  webpage += "        </tr>";
  webpage += "      </table>";
  webpage += "    </div>";
  webpage += "    <div id='jb-sidebar'>";
  webpage += "      <table>";
  webpage += "        <tr><td><H3>Hello, you successfully connected to LED Matrix Bar!</H3></td></tr>";
  webpage += "        <tr>";
  webpage += "          <td>";
  webpage += "IP address: ";
  webpage += ipStr;
  webpage += "          </td>";
  webpage += "        </tr>";
  webpage += "        <tr>";
  webpage += "          <td>";
  webpage += "MAC address: ";
  webpage += MAC_char;
  webpage += "          </td>";
  webpage += "        </tr>";
  webpage += "        <tr>";
  webpage += "          <td>";
  webpage += "mDNS: <br>";
  webpage += "<a href='http://";
  webpage += APname;
  webpage += ".local/\'>";
  webpage += "http://";
  webpage += APname;
  webpage += ".local/";
  webpage += "</a>";
  webpage += "          </td>";
  webpage += "        </tr>";
#ifdef USE_DS18B20
  webpage += "        <tr>";
  webpage += "          <td>";
  webpage += "[ DS18B20 ] - INSIDE<br>";
  if (ds18b20get == 0) {
  webpage += "  -------------------- Celsius ( Fahrenheit )<br>";
  webpage += "    - ";
  webpage += "Temperature: ";
  webpage += sensors.getTempCByIndex(0);
  webpage += " 'C ( ";
  webpage += sensors.getTempFByIndex(0);
  webpage += " 'F )<br>";
  } else if (ds18b20get == 1){
  webpage += "    - <font color='red'>Sensor error or disconnected</font>";
  }
  webpage += "          </td>";
  webpage += "        </tr>";
#endif
#ifdef USE_DHT22
  webpage += "        <tr>";
  webpage += "          <td>";
  webpage += "[ DHT22 ] - INDOOR<br>";
  if(dht22get == 0) {
  webpage += "  -------------------- Celsius ( Fahrenheit )<br>";
  webpage += "    - ";
  webpage += "Temperature: ";
  webpage += dht22cTemp;
  webpage += " 'C ( ";
  webpage += dht22fTemp;
  webpage += " 'F )<br>";
  webpage += "    - ";
  webpage += "Humidity: ";
  webpage += dht22Humidity;
  webpage += " %<br>";
  } else {
  webpage += "    - <font color='red'>Sensor error or disconnected</font>";
  }
  webpage += "          </td>";
  webpage += "        </tr>";
#endif
#ifdef USE_DHT12
  webpage += "        <tr>";
  webpage += "          <td>";
  webpage += "[ DHT12 ] - OUTDOOR<br>";
  if(dht12get == 0) {
  webpage += "  -------------------- Celsius ( Fahrenheit )<br>";
  webpage += "    - ";
  webpage += "Temperature: ";
  webpage += dht12.readTemperature();
  webpage += " 'C ( ";
  webpage += dht12.readTemperature(true);
  webpage += " 'F )<br>";
  webpage += "    - ";
  webpage += "Humidity: ";
  webpage += dht12.readHumidity();
  webpage += " %<br>";
  } else {
  webpage += "    - <font color='red'>Sensor error or disconnected</font>";
  }
  webpage += "          </td>";
  webpage += "        </tr>";
#endif
  webpage += "        <tr>";
  webpage += "          <td>";
  // Battery Monitor: ADC0(Analog Input)
  int   iA00 = A0;
  float voltage;
  voltage = analogRead(iA00) / 92.25;  // 11.1V(92.25), 9V(113.77), 7.2V(142.22), 5V(204.8), 3.3V(310.3)
  webpage += "  --------------------<br>";
  webpage += "Voltage: ";
  webpage += voltage;
  webpage += " Volt (External)";
  webpage += "          </td>";
  webpage += "        </tr>";
  webpage += "        <tr>";
  webpage += "          <td>";
  // Internal Voltage Monitor: A0(Analog Input)
  float bat_level_int = readvdd33() /1000.0;
  webpage += "Voltage: ";
  webpage += bat_level_int;
  webpage += " Volt (Internal)";
  webpage += "          </td>";
  webpage += "        </tr>";
  webpage += "        <tr>";
  webpage += "          <td>";
  webpage += "  ----------------------------------------<br>";
  webpage += "LED Matrix Status<br>";
  webpage += "    - gClock: ";
  if (gClock == 1) {
    webpage += "On<br>";
  } else {
    webpage += "Off<br>";
  }
  webpage += "    - Date Scroll: ";
  if (dateScroll == 1) {
    webpage += "On<br>";
  } else {
    webpage += "Off<br>";
  }
  webpage += "    - Weather Scroll: ";
  if (weatherScroll == 1) {
    webpage += "On<br>";
  } else {
    webpage += "Off<br>";
  }
  webpage += "    - INDOOR Scroll: ";
  if (indoorScroll == 1) {
    webpage += "On<br>";
  } else {
    webpage += "Off<br>";
  }
  webpage += "    - OUTDOOR Scroll: ";
  if (outdoorScroll == 1) {
    webpage += "On<br>";
  } else {
    webpage += "Off<br>";
  }
  webpage += "          </td>";
  webpage += "        </tr>";
  webpage += "      </table>";
  webpage += "      <table align='center'>";
  webpage += "        <tr align='center'>";
  webpage += "          <td>";
  if (OTAmode == 0) {
    webpage += "[- Over-The-Air Updates mode Off -]";
  } else {
    webpage += "[<b>- Over-The-Air Updates mode On -</b>]";
  }
  webpage += "          </td>";
  webpage += "        </tr>";
  webpage += "      </table>";
  webpage += "      <table align='center'>";
  webpage += "        <tr align='center'>";
  webpage += "          <td>";
  if (OTAmode == 0) {
    webpage += "<a href=\"/OTAmodeOn\"\"><button class='ct-btn blue en-01' style='font-size: 12px'>OTA mode</button></a>";
  } else {
    webpage += "<a href=\"/OTAmodeOff\"\"><button class='ct-btn yellow en-01' style='font-size: 12px'>OTA mode</button></a>";
  }
  webpage += "          </td>";
  webpage += "          <td>";
  webpage += "          </td>";
  webpage += "          <td><a href=\"/resetAP\"\"><button class='ct-btn blue en-01' style='font-size: 12px'>Reset AP</button></a></td>";
  webpage += "        </tr>";
  webpage += "      </table>";
  webpage += "    </div>";
  webpage += "    <div id='jb-footer'>";
  webpage += "      <a style='TEXT-DECORATION: none' href='http://zalophus.tistory.com/' target='_blank'>Copyright &copy; 2012 Zalophus's DesignHouse All rights reserved.</a>";
  webpage += "    </div>";
  webpage += "  </div>";
  webpage += "</body>";
  webpage += "</html>";
}

void home_page() { 
  update_webpage();
  server.send(200, "text/html", webpage); 
} 

void gclockWeatherScrollOn() {
  gclockWeatherScroll = 1;
  printStringWithShift("                ", 35);
  printStringWithShift("   Google Clock On", 35);
  home_page();
  delay(waitScroll);
}
void gclockWeatherScrollOff() {
  gclockWeatherScroll = 0;
  printStringWithShift("                ", 35);
  printStringWithShift("   Google Clock Off", 35);
  home_page();
  delay(waitScroll);
}

void gClock_On() {
  printStringWithShift("                ", 35);
  printStringWithShift("gClock On", 35);
  gClock = 1;
  home_page();
  delay(waitScroll);
}
void gClock_Off() {
  printStringWithShift("                ", 35);
  printStringWithShift("gClock Off", 35);
  gClock = 0;
  home_page();
  delay(waitScroll);
}
void showAnimClock_On() {
  printStringWithShift("                ", 35);
  printStringWithShift("Show Animation Clock On", 35);
  showAnimClockSW = 1;
  home_page();
  delay(waitScroll);
}
void showAnimClock_Off() {
  printStringWithShift("                ", 35);
  printStringWithShift("Show Simple Clock On", 35);
  showAnimClockSW = 0;
  home_page();
  delay(waitScroll);
}

void dateScroll_On() {
  printStringWithShift("                ", 35);
  printStringWithShift("Date Scroll On", 35);
  dateScroll = 1;
  home_page();
  delay(waitScroll);
}
void dateScroll_Off() {
  printStringWithShift("                ", 35);
  printStringWithShift("Date Scroll Off", 35);
  dateScroll = 0;
  home_page();
  delay(waitScroll);
}

void weatherScroll_On() {
  printStringWithShift("                ", 35);
  printStringWithShift("Weather Scroll On", 35);
  weatherScroll = 1;
  home_page();
  delay(waitScroll);
}
void weatherScroll_Off() {
  printStringWithShift("                ", 35);
  printStringWithShift("Weather Scroll Off", 35);
  weatherScroll = 0;
  home_page();
  delay(waitScroll);
}

void resetAP() {
  WiFiManager wifiManager;
  wifiManager.resetSettings();
}

void OTAmodeOn() {
  OTAmode = 1;
  printStringWithShift("                ", 35);
  printStringWithShift("OTA mode On", 35);
  home_page();
  delay(waitScroll);
}
void OTAmodeOff() {
  OTAmode = 0;
  printStringWithShift("                ", 35);
  printStringWithShift("OTA mode Off", 35);
  home_page();
  delay(waitScroll);
}

void handle_msg() {
  String msg           = server.arg("msg");
  Serial.println(msg);
  String decodedMsg    = msg;
  // Restore special characters that are misformed to %char by the client browser
  decodedMsg.replace("+", " ");
  decodedMsg.replace("%21", "!");
  decodedMsg.replace("%22", "");
  decodedMsg.replace("%23", "#");
  decodedMsg.replace("%24", "$");
  decodedMsg.replace("%25", "%");
  decodedMsg.replace("%26", "&");
  decodedMsg.replace("%27", "'");
  decodedMsg.replace("%28", "(");
  decodedMsg.replace("%29", ")");
  decodedMsg.replace("%2A", "*");
  decodedMsg.replace("%2B", "+");
  decodedMsg.replace("%2C", ",");
  decodedMsg.replace("%2F", "/");
  decodedMsg.replace("%3A", ":");
  decodedMsg.replace("%3B", ";");
  decodedMsg.replace("%3C", "<");
  decodedMsg.replace("%3D", "=");
  decodedMsg.replace("%3E", ">");
  decodedMsg.replace("%3F", "?");
  decodedMsg.replace("%40", "@");
  printStringWithShift("                ", 35);
  printStringWithShift(decodedMsg.c_str(), 35);
  printMsg = decodedMsg;
  home_page();
  delay(waitScroll);
}

void repeatprintMsg() {
  printStringWithShift("                ", 35);
  printStringWithShift(printMsg.c_str(), 35);
  home_page();
  delay(waitScroll);
}

void inDoor() {
  indoor  = 1;
  outdoor = 0;
  char dht22cTemp[10]    = "";
  char dht22Humidity[10] = "";
  dtostrf(dht.readTemperature(), 3, 1, dht22cTemp);
  dtostrf(dht.readHumidity(), 3, 1, dht22Humidity);
  if(dht22get == 0) {
    printStringWithShift("                ", 35);
    printStringWithShift("INDOOR", 35);
    printStringWithShift("   ", 35);
    printStringWithShift("Temp: ", 35);
    printStringWithShift(dht22cTemp, 35);
    printStringWithShift(deg.c_str(), 35);
    printStringWithShift("C", 35);
    printStringWithShift("   ", 35);
    printStringWithShift("Humidity: ", 35);
    printStringWithShift(dht22Humidity, 35);
    printStringWithShift("%", 35);
  }
  home_page();
  delay(waitScroll);
}

void outDoor() {
  indoor  = 0;
  outdoor = 1;
  char dht12cTemp[10]    = "";
  char dht12Humidity[10] = "";
  dtostrf(dht12.readTemperature(), 3, 1, dht12cTemp);
  dtostrf(dht12.readHumidity(), 3, 1, dht12Humidity);
  if(dht12get == 0) {
    printStringWithShift("                ", 35);
    printStringWithShift("OUTDOOR", 35);
    printStringWithShift("   ", 35);
    printStringWithShift("Temp: ", 35);
    printStringWithShift(dht12cTemp, 35);
    printStringWithShift(deg.c_str(), 35);
    printStringWithShift("C", 35);
    printStringWithShift("   ", 35);
    printStringWithShift("Humidity: ", 35);
    printStringWithShift(dht12Humidity, 35);
    printStringWithShift("%", 35);
  }
  home_page();
  delay(waitScroll);
}

void inDoorScrollOn() {
  indoorScroll  = 1;
  if(dht22get == 0) {
  printStringWithShift("                ", 35);
  printStringWithShift("INDOOR Scroll On", 35);
  }
  home_page();
  delay(waitScroll);
}
void inDoorScrollOff() {
  indoorScroll  = 0;
  if(dht22get == 0) {
  printStringWithShift("                ", 35);
  printStringWithShift("INDOOR Scroll Off", 35);
  }
  home_page();
  delay(waitScroll);
}

void outDoorScrollOn() {
  outdoorScroll = 1;
  if(dht12get == 0) {
  printStringWithShift("                ", 35);
  printStringWithShift("OUTDOOR Scroll On", 35);
  }
  home_page();
  delay(waitScroll);
}
void outDoorScrollOff() {
  outdoorScroll = 0;
  if(dht12get == 0) {
  printStringWithShift("                ", 35);
  printStringWithShift("OUTDOOR Scroll Off", 35);
  }
  home_page();
  delay(waitScroll);
}

void light16on() {
  digitalWrite(16, HIGH);
  printStringWithShift("                ", 35);
  printStringWithShift("16 Turn On", 35);
  home_page();
  delay(waitScroll);
}
void light16off() {
  digitalWrite(16,  LOW);
  printStringWithShift("                ", 35);
  printStringWithShift("16 Turn Off", 35);
  home_page();
  delay(waitScroll);
}

void light00on() {
  digitalWrite( 0, HIGH);
  printStringWithShift("                ", 35);
  printStringWithShift("00 Turn On", 35);
  home_page();
  delay(waitScroll);
}
void light00off() {
  digitalWrite( 0,  LOW);  
  printStringWithShift("                ", 35);
  printStringWithShift("00 Turn Off", 35);
  home_page();
  delay(waitScroll);
}

void light12on() {
  digitalWrite(12, HIGH);
  printStringWithShift("                ", 35);
  printStringWithShift("12 Turn On", 35);
  home_page();
  delay(waitScroll);
}
void light12off() {
  digitalWrite(12,  LOW);
  printStringWithShift("                ", 35);
  printStringWithShift("12 Turn Off", 35);
  home_page();
  delay(waitScroll);
}

void button00() { printStringWithShift("                ", 35); printStringWithShift("I love you", 35); home_page(); delay(waitScroll); }
void button01() { printStringWithShift("                ", 35); printStringWithShift("Don't get upset", 35); home_page(); delay(waitScroll); }
void button02() { printStringWithShift("                ", 35); printStringWithShift("I'm upset", 35); home_page(); delay(waitScroll); }
void button03() { printStringWithShift("                ", 35); printStringWithShift("Catch me if you can", 35); home_page(); delay(waitScroll); }
void button04() { printStringWithShift("                ", 35); printStringWithShift("You will never know until you try", 35); home_page(); delay(waitScroll); }
void button05() { printStringWithShift("                ", 35); printStringWithShift("Pain past is pleasure", 35); home_page(); delay(waitScroll); }

// Web Server Start ========================================================
void webserver_start() {
  server.begin();
#ifdef USE_UART
  Serial.println("Server started");
  Serial.println();
  delay(10);
#endif

  server.on("/", home_page);
  server.on("/gClockOn",                 gClock_On);  // Define what happens when a client requests attention
  server.on("/gClockOff",               gClock_Off);  // Define what happens when a client requests attention
  server.on("/showAnimClockOn",   showAnimClock_On);  // Define what happens when a client requests attention
  server.on("/showAnimClockOff", showAnimClock_Off);  // Define what happens when a client requests attention
  server.on("/dateScrollOn",         dateScroll_On);  // Define what happens when a client requests attention
  server.on("/dateScrollOff",       dateScroll_Off);  // Define what happens when a client requests attention
  server.on("/weatherScrollOn",   weatherScroll_On);  // Define what happens when a client requests attention
  server.on("/weatherScrollOff", weatherScroll_Off);  // Define what happens when a client requests attention
  server.on("/msg",                     handle_msg);  // And as regular external functions:
  server.on("/repeatprintMsg",      repeatprintMsg);  // And as regular external functions:
  server.on("/OTAmodeOn",                OTAmodeOn);  // And as regular external functions:
  server.on("/OTAmodeOff",              OTAmodeOff);  // And as regular external functions:
  server.on("/resetAP",                    resetAP);  // And as regular external functions:
  
  server.on("/inDoor",                      inDoor);
  server.on("/outDoor",                    outDoor);
  server.on("/inDoorScrollOn",      inDoorScrollOn);
  server.on("/inDoorScrollOff",    inDoorScrollOff);
  server.on("/outDoorScrollOn",    outDoorScrollOn);
  server.on("/outDoorScrollOff",  outDoorScrollOff);

  server.on("/light16on",  light16on);
  server.on("/light16off", light16off);
  server.on("/light00on",  light00on);
  server.on("/light00off", light00off);
  server.on("/light12on",  light12on);
  server.on("/light12off", light12off);

  server.on("/button00", button00);
  server.on("/button01", button01);
  server.on("/button02", button02);
  server.on("/button03", button03);
  server.on("/button04", button04);
  server.on("/button05", button05);
}

void webserver_action() {
  update_webpage();
  server.handleClient();  // Wait for a client to connect and when they do process their requests
}

#endif
