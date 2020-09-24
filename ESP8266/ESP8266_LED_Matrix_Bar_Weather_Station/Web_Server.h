/* =========================================================================
 *  Author: Zalophus Dokdo (https://zddh.blogspot.com)
 *  Date: 31/08/2017       (https://zalophus.tistory.com/)
 *  License: GPL v2
 * =========================================================================
 *  LED Matrix Bar Weather Station V1.0.4 (Publish: 2018/01/02)
 * =========================================================================
 *  Web Server
 * =========================================================================
 */

#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <ESP8266WebServer.h>

// Set the port you wish to use, a browser default is 80, but any port can be used, if you set it to 5555 then connect with http://nn.nn.nn.nn:5555/
ESP8266WebServer server(80);

String webTitle          = WEB_TITLE;
String webSubTitle       = WEB_SUBTITLE;
int    webPageRefresh    = WEB_PAGE_REFRESH;

int    OTAmode           = OTA_MODE;
int    ledMatrix         = LED_MATRIX;
int    Clock             = CLOCK;
int    dateScroll        = DATE_SCROLL;
int    weatherScroll     = WEATHER_SCROLL;
int    indoor            = INDOOR;
int    outdoor           = OUTDOOR;
int    indoorScroll      = INDOOR_SCROLL;
int    outdoorScroll     = OUTDOOR_SCROLL;
int    waitScroll        = WAIT_SCROLL;

extern "C" {
  uint16 readvdd33(void);
}

String deg               = String(char('~'+25));
String webpage           = "";
String printMsg;

int    onTimeAlarm = 0;
int    alarm_state = ALARM_STATE;
int    alarm_h_set = ALARM_H_SET;
int    alarm_m_set = ALARM_M_SET;
String alarm_ampm_select = "AM";

void append_webpage_header() {
  // webpage is a global variable
  webpage = ""; // A blank string variable to hold the web page
  webpage += "<!DOCTYPE HTML>";
  webpage += "<html>";
  webpage += "<head>";
  webpage += "<meta http-equiv='refresh' content='" + String(webPageRefresh) + "; url=./' />";
  webpage += "<meta name='apple-mobile-web-app-capable' content='yes' />";
  webpage += "<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />";
  webpage += "<link rel='apple-touch-icon' href='https://raw.githubusercontent.com/ZalophusDokdo/LED_Matrix_Bar_Weather_Station/master/images/electronic_clock_1200_811483_512.png' />";
  webpage += "<link rel='shortcut icon' href='favicon.ico' />";
  webpage += "  <title>LED Matrix Bar - Zalophus's DesignHouse</title>";
  webpage += "  <meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  webpage += "  <style>";
  webpage += "    #jb-container { width: 940px; margin: 0px auto; padding: 10px; border: 1px solid #bcbcbc; }";
  webpage += "    #jb-header { padding: 0px; margin-bottom: 10px; border: 1px solid #bcbcbc; }";
  webpage += "    #jb-content   { width: 580px; padding: 10px; margin-bottom: 10px; float: left; border: 1px solid #bcbcbc;";
  webpage += "                    background-image: url(https://raw.githubusercontent.com/ZalophusDokdo/LED_Matrix_Bar_Weather_Station/master/images/weather_2_2682849_512.png);";
  webpage += "                    background-repeat: no-repeat; background-attachment: fixed; background-size: 100%;";
  webpage += "                  }";
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
  webpage += "      <center><a style='TEXT-DECORATION: none' href='./'><h1>" + webTitle + "<br>" + webSubTitle + "</h1></a></center>";
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
  webpage += "          <a href=\"/weatherSCROLL\"\">";
  webpage += "<button type='submit' class='ct-btn white en-01' style='font-size: 14px'>";
  webpage += String(country) + "<br>";
  webpage += date + "<br>";
  if (am_pm == 12) {
    webpage += "[ " + String(ampm) + " " + String(int(h)) + ":" + String(int(m)) + " ]<br>";
  } else {
    webpage += "[ " + String(int(h)) + ":" + String(int(m)) + " ]<br>";
  }
  webpage += "Temp     : " + String(temp,1)      + " 'C ( " + String(tempMin,1) + " 'C - " + String(tempMax,1) + " 'C ) <br>";
  webpage += "Humidity : " + String(humidity)    + " %  <br>";
  webpage += "Pressure : " + String(pressure)    + " hPa  <br>";
  webpage += "Clouds   : " + String(clouds)      + " %  <br>";
  webpage += "Wind     : " + String(windSpeed,1) + " m/s <br>";
  webpage += "</button></a><br>";
  webpage += "<hr>";  // ===========================================================
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
  webpage += "      <table align='center' width='100%'>";
  webpage += "        <tr>";
  webpage += "          <td>";
  webpage += "<hr>";  // ===========================================================
  webpage += "          </td>";
  webpage += "        </tr>";
  webpage += "      </table>";
#ifdef USE_DHT || USE_DHT12
  webpage += "      <table align='center' width='100%'>";
  webpage += "        <tr align='center'>";
#ifdef USE_DHT
  if (indoor == 1) {
    if(dht22get == 0) {
      webpage += "      <td>";
      webpage += " < ";
      webpage += dht22cTemp;
      webpage += " 'C >";
      webpage += "      </td>";
    }
  }
#endif
#ifdef USE_DHT12
  if (outdoor == 1) {
    if(dht12get == 0) {
      webpage += "      <td>";
      webpage += " < ";
      webpage += dht12.readTemperature();
      webpage += " 'C >";
      webpage += "      </td>";
    }
  }
#endif
#ifdef USE_DHT
  if (indoor == 1) {
    webpage += "        <td><b>INDOOR</b>";
    webpage += "        </td>";
  }
#endif
#ifdef USE_DHT12
  if (outdoor == 1) {
    webpage += "        <td><b>OUTDOOR</b>";
    webpage += "        </td>";
  }
#endif
#ifdef USE_DHT
  if (indoor == 1) {
    if(dht22get == 0) {
      webpage += "      <td>";
      webpage += " < ";
      webpage += dht22Humidity;
      webpage += " % >";
      webpage += "      </td>";
    }
  }
#endif
#ifdef USE_DHT12
  if (outdoor == 1) {
    if(dht12get == 0) {
      webpage += "      <td>";
      webpage += " < ";
      webpage += dht12.readHumidity();
      webpage += " % >";
      webpage += "      </td>";
    }
  }
#endif
  webpage += "        </tr>";
  webpage += "      </table>";
  webpage += "      <table align='center' width='100%'>";
  webpage += "        <tr align='center'>";
  webpage += "          <td>";
  webpage += "            <table align='center' width='100%'>";
  webpage += "              <tr align='center'>";
#ifdef USE_DHT
  if (indoor == 1) {
#ifndef USE_DHT12
    webpage += "              <td>";
    if (dht.readTemperature() > 35 && dht.readHumidity() > 60) { 
      webpage += "<button type='submit' class='ct-btn red en-01' style='font-size: 44px'>-- Hot --</button>";
    } else if (dht.readTemperature() > 29 && dht.readHumidity() > 40) { 
      webpage += "<button type='submit' class='ct-btn yellow en-01' style='font-size: 44px'>- Warm -</button>";
    } else if (dht.readTemperature() < 20 && dht.readHumidity() < 30) {
      webpage += "<button type='submit' class='ct-btn blue en-01' style='font-size: 44px'>- Cold -</button>";
    } else {
      webpage += "<button type='submit' class='ct-btn green en-01' style='font-size: 44px'>- Good -</button>";
    }
    webpage += "              </td>";
#else
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
#endif
  }
#endif
#ifdef USE_DHT12
  if (outdoor == 1) {
#ifndef USE_DHT
    webpage += "              <td>";
    if (dht12.readTemperature() > 35 && dht12.readHumidity() > 60) { 
      webpage += "<button type='submit' class='ct-btn red en-01' style='font-size: 44px'>-- Hot --</button>";
    } else if (dht12.readTemperature() > 29 && dht12.readHumidity() > 40) { 
      webpage += "<button type='submit' class='ct-btn yellow en-01' style='font-size: 44px'>- Warm -</button>";
    } else if (dht12.readTemperature() < 20 && dht12.readHumidity() < 30) {
      webpage += "<button type='submit' class='ct-btn blue en-01' style='font-size: 44px'>- Cold -</button>";
    } else {
      webpage += "<button type='submit' class='ct-btn green en-01' style='font-size: 44px'>- Good -</button>";
    }
    webpage += "              </td>";
#else
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
#endif
  }
#endif
  webpage += "              </tr>";
  webpage += "            </table>";
  webpage += "          </td>";
  webpage += "        </tr>";
  webpage += "      </table>";
  webpage += "      <table align='center' width='100%'>";
  webpage += "        <tr>";
  webpage += "          <td>";
  webpage += "<hr>";  // ===========================================================
  webpage += "          </td>";
  webpage += "        </tr>";
  webpage += "      </table>";
#endif
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
  if (Clock == 0) {
    webpage += "<a href=\"/ClockOn\"\"><button class='ct-btn red en-01' style='font-size: 12px'>----------<br>Clock<br>On<br>----------</button></a>";
  } else {
    webpage += "<a href=\"/ClockOff\"\"><button class='ct-btn green en-01' style='font-size: 12px'>----------<br>Clock<br>Off<br>----------</button></a>";
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
  webpage += "      </table>";
#ifdef USE_DHT || USE_DHT12
  webpage += "      <table align='center' width='100%'>";
  webpage += "        <tr align='center'>";
#ifdef USE_DHT
  webpage += "          <td>";
  if (indoorScroll == 0) {
    webpage += "<a href=\"/inDoorScrollOn\"\"><button class='ct-btn blue en-01' style='font-size: 12px'>----------<br>Scrolling<br>INDOOR<br>On<br>----------</button></a>";
  } else {
    webpage += "<a href=\"/inDoorScrollOff\"\"><button class='ct-btn yellow en-01' style='font-size: 12px'>----------<br>Scrolling<br>INDOOR<br>Off<br>----------</button></a>";
  }
  webpage += "          </td>";
#endif
#ifdef USE_DHT12
  webpage += "          <td>";
  if (outdoorScroll == 0) {
    webpage += "<a href=\"/outDoorScrollOn\"\"><button class='ct-btn blue en-01' style='font-size: 12px'>----------<br>Scrolling<br>OUTDOOR<br>On<br>----------</button></a>";
  } else {
    webpage += "<a href=\"/outDoorScrollOff\"\"><button class='ct-btn yellow en-01' style='font-size: 12px'>----------<br>Scrolling<br>OUTDOOR<br>Off<br>----------</button></a>";
  }
  webpage += "          </td>";
#endif
  webpage += "        </tr>";
  webpage += "      </table>";
#endif
  webpage += "      <table align='center' width='100%'>";
  webpage += "        <tr>";
  webpage += "          <td>";
  webpage += "<hr>";  // ===========================================================
  webpage += "          </td>";
  webpage += "        </tr>";
  webpage += "      </table>";
/*
  webpage += "      <table align='center' width='100%'>";
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
  webpage += "      </table>";
  webpage += "      <table align='center' width='100%'>";
  webpage += "        <tr>";
  webpage += "          <td>";
  webpage += "<hr>";  // ===========================================================
  webpage += "          </td>";
  webpage += "        </tr>";
  webpage += "      </table>";
 */
  webpage += "      <table align='center' width='100%'>";
  webpage += "        <tr align='center'>";
  webpage += "          <td>";
  if (digitalRead(RED_PIN)) { 
    webpage += "<a href=\"/alarm16off\"\"><button type='submit' class='ct-btn yellow en-01' style='font-size: 14px'>----------<br>Alarm<br>Off<br>----------</button></a>";
  } else {
    webpage += "<a href=\"/alarm16on\"\"><button type='submit' class='ct-btn red en-01' style='font-size: 14px'>----------<br>Alarm<br>On<br>----------</button></a>";
  };
  webpage += "          </td>";
  webpage += "          <td>";
  if (digitalRead(GREEN_PIN)) { 
    webpage += "<a href=\"/lamp00off\"\"><button type='submit' class='ct-btn yellow en-01' style='font-size: 14px'>----------<br>Lamp<br>Off<br>----------</button></a>";
  } else {
    webpage += "<a href=\"/lamp00on\"\"><button type='submit' class='ct-btn green en-01' style='font-size: 14px'>----------<br>Lamp<br>On<br>----------</button></a>";
  }
  webpage += "          </td>";
  webpage += "          <td>";
  if (digitalRead(BLUE_PIN)) { 
    webpage += "<a href=\"/scroll12off\"\"><button type='submit' class='ct-btn yellow en-01' style='font-size: 14px'>----------<br>Scroll<br>Off<br>----------</button></a>";
  } else {
    webpage += "<a href=\"/scroll12on\"\"><button type='submit' class='ct-btn blue en-01' style='font-size: 14px'>----------<br>Scroll<br>On<br>----------</button></a>";
  };
  webpage += "          </td>";
  webpage += "        </tr>";
  webpage += "      </table>";
  webpage += "      <table align='center'>";
  webpage += "        <tr align='center'>";
  webpage += "          <td>";
  webpage += "    Alarm Setting<br>";
  webpage += "            <form action='alarmTime'>";
  if (am_pm == 12) {
    webpage += "          <label for='ampm'>Choose a AM/PM:</label><br>";
    webpage += "          <select id='ampm' name='ampm'>";
    if (alarm_ampm_select == "AM") {
      webpage += "          <option value='AM' selected='selected'>AM</option>";
    } else {
      webpage += "          <option value='AM'>AM</option>";
    }
    if (alarm_ampm_select == "PM") {
      webpage += "          <option value='PM' selected='selected'>PM</option>";  
    } else {
      webpage += "          <option value='PM'>PM</option>";  
    }
    webpage += "          </select>";
    webpage += "          <input type='number' min='01' max='12' name='alarm_h_time' value='";
    webpage += String(alarm_h_set);
    webpage += "' size=10% autofocus>:";
  } else {
    webpage += "          <input type='number' min='00' max='23' name='alarm_h_time' value='";
    webpage += String(alarm_h_set);
    webpage += "' size=10% autofocus>:";
  }
  webpage += "            <input type='number' min='00' max='59' name='alarm_m_time' value='";
  webpage += String(alarm_m_set);
  webpage += "' size=10% autofocus> ";
  webpage += "            <input class='ct-btn blue en-01' style='font-size: 12px' type='submit' value='Submit'>";
  webpage += "            </form>";
  webpage += "          </td>";
  webpage += "        </tr>";
  webpage += "      </table>";
  webpage += "      <table align='center'>";
  webpage += "        <tr align='center'>";
  webpage += "          <td><a href=\"/ampmOnOff\"\"><button class='ct-btn blue en-01' style='font-size: 14px'>AM-PM On/Off</button></a></td>";
  webpage += "          <td></td>";
  webpage += "          <td><a href=\"/onTimeALARM\"\"><button class='ct-btn green en-01' style='font-size: 14px'>On-time Alarm</button></a></td>";
  webpage += "          <td></td>";
  webpage += "          <td><a href=\"/alarmOnOff\"\"><button class='ct-btn blue en-01' style='font-size: 14px'>Alarm</button></a></td>";
  webpage += "        </tr>";
  webpage += "      </table>";
  webpage += "      <table align='center' width='100%'>";
  webpage += "        <tr>";
  webpage += "          <td>";
  webpage += "<hr>";  // ===========================================================
  webpage += "          </td>";
  webpage += "        </tr>";
  webpage += "      </table>";
  webpage += "      <table align='center'>";
  webpage += "        <tr align='center'>";
  webpage += "          <td><a href=\"/updateDATA\"\"><button class='ct-btn green en-01' style='font-size: 14px'>Update Data</button></a></td>";
  webpage += "          <td></td>";
  webpage += "          <td><a href=\"/ESPrestart\"\"><button class='ct-btn blue en-01' style='font-size: 14px'>Restart</button></a></td>";
  webpage += "          <td></td>";
  webpage += "          <td><a href=\"/resetAP\"\"><button class='ct-btn red en-01' style='font-size: 14px'>Reset AP</button></a></td>";
  webpage += "        </tr>";
  webpage += "      </table>";
  webpage += "    </div>";
  webpage += "    <div id='jb-sidebar'>";
  webpage += "      <table>";
  webpage += "        <tr><td><H3>Hello, you successfully connected to " + webTitle + "!</H3></td></tr>";
  webpage += "        <tr>";
  webpage += "          <td>";
  webpage += "Project: ";
  webpage += projectName;
  webpage += "          </td>";
  webpage += "        </tr>";
  webpage += "        <tr>";
  webpage += "          <td>";
  webpage += "Version: ";
  webpage += projectVersion;
  webpage += "          <br>";
  webpage += "IP address: ";
  webpage += ipStr;
  webpage += "          <br>";
  webpage += "MAC address: ";
  webpage += MAC_char;
  webpage += "          <br>";
  webpage += "mDNS: <br>";
  webpage += "<a href='http://";
  webpage += APname;
  webpage += ".local/\'>";
  webpage += "http://";
  webpage += APname;
  webpage += ".local/";
  webpage += "</a>";
  webpage += "          <br>";
  webpage += "<hr>";  //==================================
#ifdef USE_DS18B20
  webpage += "[ DS18B20 ] - INSIDE<br>";
  if (ds18b20get == 0) {
  webpage += "    - ";
  webpage += "Temperature: ";
  webpage += sensors.getTempCByIndex(0);
  webpage += " 'C ( ";
  webpage += sensors.getTempFByIndex(0);
  webpage += " 'F )<br>";
  } else if (ds18b20get == 1){
  webpage += "    - <font color='red'>Sensor error or disconnected</font><br>";
  }
#endif
#ifdef USE_DHT
  webpage += "[ DHT";
  webpage += DHTTYPE;
  webpage += " ] - INDOOR<br>";
  if (dht22get == 0) {
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
  webpage += "    - <font color='red'>Sensor error or disconnected</font><br>";
  }
#endif
#ifdef USE_DHT12
  webpage += "[ DHT12 ] - OUTDOOR<br>";
  if (dht12get == 0) {
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
  webpage += "    - <font color='red'>Sensor error or disconnected</font><br>";
  }
#endif
  int   iA00 = A0;  // Battery Monitor: ADC0(Analog Input)
  float voltage;
  voltage = analogRead(iA00) / 92.25;  // 11.1V(92.25), 9V(113.77), 7.2V(142.22), 5V(204.8), 3.3V(310.3)
  webpage += "[ Power ]<br>";
  webpage += "- Voltage: ";
  webpage += voltage;
  webpage += " Volt (External)<br>";
  float bat_level_int = readvdd33() / 1000.0;  // Internal Voltage Monitor: A0(Analog Input)
  webpage += "- Voltage: ";
  webpage += bat_level_int;
  webpage += " Volt (Internal)<br>";
  webpage += "<hr>";  // ===========================================================
  webpage += "[ LED Matrix Status ]<br>";
  webpage += "    - Clock: ";
  if (Clock == 1) {
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
  webpage += "<hr width='70%'>";  // ===========================================================
  webpage += "    - Number of MAX7219's: ";
  webpage += NUM_MAX;
  webpage += "<br>";
  webpage += "    - Initial LED Matrix intensity(1-15): ";
  webpage += SET_INTENSITY;
  webpage += "<br>";
  webpage += "    - Weather scroll interval: ";
  int weatherScrollInterval = WEATHER_SCROLL_INTERVAL / 1000;
  webpage += weatherScrollInterval;
  webpage += " sec<br>";
  webpage += "    - String shift delay time: ";
  int stringShiftDelayTime = STRING_SHIFT_DELAY;
  webpage += stringShiftDelayTime;
  webpage += " msec<br>";
  webpage += "    - Update time interval: ";
  int updateTimeCount = UPDATE_TIME_COUNT * 0.75;
  webpage += updateTimeCount;
  webpage += " minute<br>";
  webpage += "<hr>";  // ===========================================================
  webpage += "[ Clock ]<br>";
  webpage += "    - Coordinated Universal Time(UTC): ";
  webpage += utcOffset;
  webpage += "<br>";
  webpage += "    - AM-PM Mode: ";
  if (am_pm == 12) {
  webpage += "On, 12-hour clock<br>";
  } else {
  webpage += "Off, 24-hour clock<br>";
  }
  webpage += "    - Alarm Setting Status: ";
  if (alarm_state == 1) {
  webpage += "On<br>";
  } else {
  webpage += "Off<br>";
  }
  webpage += "    - Alarm Time: ";
  if (onTimeAlarm == 1) {
    webpage += "On-Time";
  } else {
    if (am_pm == 12) {
      webpage += String(alarm_ampm_select) + " " + String(alarm_h_set) + ":" + String(alarm_m_set);
    } else {
      webpage += String(alarm_h_set) + ":" + String(alarm_m_set);
    }
  }
  webpage += "<br>";
  webpage += "          </td>";
  webpage += "        </tr>";
  webpage += "      </table>";
  webpage += "    </div>";
  webpage += "    <div id='jb-footer'>";
  webpage += "      <a style='TEXT-DECORATION: none' href='https://zalophus.tistory.com/' target='_blank'>Copyright &copy; 2012 Zalophus's DesignHouse All rights reserved.</a>";
  webpage += "    </div>";
  webpage += "  </div>";
  webpage += "</body>";
  webpage += "</html>";
}

void home_page() { 
  update_webpage();
  server.send(200, "text/html", webpage); 
} 

void weatherSCROLL() {
  LEDMatrix_Date_action();
  LEDMatrix_Weather_action();
  home_page();
  delay(waitScroll);
}

void Clock_On() {
  printStringWithShift("                ", stringShiftDelay);
  printStringWithShift("Clock On", stringShiftDelay);
  delay(waitScroll);
  printStringWithShift("                ", stringShiftDelay);
  Clock = 1;
  home_page();
  delay(waitScroll);
}
void Clock_Off() {
  printStringWithShift("                ", stringShiftDelay);
  printStringWithShift("Clock Off", stringShiftDelay);
  delay(waitScroll);
  printStringWithShift("                ", stringShiftDelay);
  Clock = 0;
  home_page();
  delay(waitScroll);
}
void showAnimClock_On() {
  printStringWithShift("                ", stringShiftDelay);
  printStringWithShift("Show Animation Clock On", stringShiftDelay);
  delay(waitScroll);
  printStringWithShift("                ", stringShiftDelay);
  showAnimClockSW = 1;
  home_page();
  delay(waitScroll);
}
void showAnimClock_Off() {
  printStringWithShift("                ", stringShiftDelay);
  printStringWithShift("Show Simple Clock On", stringShiftDelay);
  delay(waitScroll);
  printStringWithShift("                ", stringShiftDelay);
  showAnimClockSW = 0;
  home_page();
  delay(waitScroll);
}

void dateScroll_On() {
  printStringWithShift("                ", stringShiftDelay);
  printStringWithShift("Date Scroll On", stringShiftDelay);
  delay(waitScroll);
  printStringWithShift("                ", stringShiftDelay);
  dateScroll = 1;
  home_page();
  delay(waitScroll);
}
void dateScroll_Off() {
  printStringWithShift("                ", stringShiftDelay);
  printStringWithShift("Date Scroll Off", stringShiftDelay);
  delay(waitScroll);
  printStringWithShift("                ", stringShiftDelay);
  dateScroll = 0;
  home_page();
  delay(waitScroll);
}

void weatherScroll_On() {
  printStringWithShift("                ", stringShiftDelay);
  printStringWithShift("Weather Scroll On", stringShiftDelay);
  delay(waitScroll);
  printStringWithShift("                ", stringShiftDelay);
  weatherScroll = 1;
  home_page();
  delay(waitScroll);
}
void weatherScroll_Off() {
  printStringWithShift("                ", stringShiftDelay);
  printStringWithShift("Weather Scroll Off", stringShiftDelay);
  delay(waitScroll);
  printStringWithShift("                ", stringShiftDelay);
  weatherScroll = 0;
  home_page();
  delay(waitScroll);
}

void updateDATA() {
  printStringWithShift("   Getting data", 15);
  ntpTime();
  getWeatherData();
  delay(waitScroll);
  LEDMatrix_Clock_action();
  home_page();
  delay(waitScroll);
}

void ESPrestart() {
  ESP.restart();
  home_page();
  delay(waitScroll);
}

void resetAP() {
  WiFiManager wifiManager;
  wifiManager.resetSettings();
}

void OTAmodeOn() {
  printStringWithShift("                ", stringShiftDelay);
  printStringWithShift("OTA mode On", stringShiftDelay);
  delay(waitScroll);
  printStringWithShift("                ", stringShiftDelay);
  OTAmode = 1;
  home_page();
  delay(waitScroll);
}
void OTAmodeOff() {
  printStringWithShift("                ", stringShiftDelay);
  printStringWithShift("OTA mode Off", stringShiftDelay);
  delay(waitScroll);
  printStringWithShift("                ", stringShiftDelay);
  OTAmode = 0;
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
  printStringWithShift("                ", stringShiftDelay);
  printStringWithShift(decodedMsg.c_str(), stringShiftDelay);
  printMsg = decodedMsg;
  home_page();
  delay(waitScroll);
}

void repeatprintMsg() {
  printStringWithShift("                ", stringShiftDelay);
  printStringWithShift(printMsg.c_str(), stringShiftDelay);
  home_page();
  delay(waitScroll);
}

void inDoor() {
#ifdef USE_DHT
  char dht22cTemp[10]    = "";
  char dht22Humidity[10] = "";
  dtostrf(dht.readTemperature(), 3, 1, dht22cTemp);
  dtostrf(dht.readHumidity(), 3, 1, dht22Humidity);
  if(dht22get == 0) {
    printStringWithShift("                ", stringShiftDelay);
    printStringWithShift("INDOOR", stringShiftDelay);
    printStringWithShift("   ", stringShiftDelay);
    printStringWithShift("Temp: ", stringShiftDelay);
    printStringWithShift(dht22cTemp, stringShiftDelay);
    printStringWithShift(deg.c_str(), stringShiftDelay);
    printStringWithShift("C", stringShiftDelay);
    printStringWithShift("   ", stringShiftDelay);
    printStringWithShift("Humidity: ", stringShiftDelay);
    printStringWithShift(dht22Humidity, stringShiftDelay);
    printStringWithShift("%", stringShiftDelay);
    printStringWithShift("                ", stringShiftDelay);
  }
#endif
  indoor  = 1;
  outdoor = 0;
  home_page();
  delay(waitScroll);
}

void outDoor() {
#ifdef USE_DHT12
  char dht12cTemp[10]    = "";
  char dht12Humidity[10] = "";
  dtostrf(dht12.readTemperature(), 3, 1, dht12cTemp);
  dtostrf(dht12.readHumidity(), 3, 1, dht12Humidity);
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
    printStringWithShift("%", stringShiftDelay);
    printStringWithShift("                ", stringShiftDelay);
  }
#endif
  indoor  = 0;
  outdoor = 1;
  home_page();
  delay(waitScroll);
}

void inDoorScrollOn() {
#ifdef USE_DHT
  if (dht22get == 0) {
  printStringWithShift("                ", stringShiftDelay);
  printStringWithShift("INDOOR Scroll On", stringShiftDelay);
  delay(waitScroll);
  printStringWithShift("                ", stringShiftDelay);
  }
  indoorScroll  = 1;
  home_page();
  delay(waitScroll);
#endif
}
void inDoorScrollOff() {
#ifdef USE_DHT
  if (dht22get == 0) {
  printStringWithShift("                ", stringShiftDelay);
  printStringWithShift("INDOOR Scroll Off", stringShiftDelay);
  delay(waitScroll);
  printStringWithShift("                ", stringShiftDelay);
  }
  indoorScroll  = 0;
  home_page();
  delay(waitScroll);
#endif
}

void outDoorScrollOn() {
#ifdef USE_DHT12
  if (dht12get == 0) {
  printStringWithShift("                ", stringShiftDelay);
  printStringWithShift("OUTDOOR Scroll On", stringShiftDelay);
  delay(waitScroll);
  printStringWithShift("                ", stringShiftDelay);
  }
  outdoorScroll = 1;
  home_page();
  delay(waitScroll);
#endif
}
void outDoorScrollOff() {
#ifdef USE_DHT12
  if (dht12get == 0) {
  printStringWithShift("                ", stringShiftDelay);
  printStringWithShift("OUTDOOR Scroll Off", stringShiftDelay);
  delay(waitScroll);
  printStringWithShift("                ", stringShiftDelay);
  }
  outdoorScroll = 0;
  home_page();
  delay(waitScroll);
#endif
}

void alarm16on() {
  printStringWithShift("                ", stringShiftDelay);
  printStringWithShift("Alarm Turn On", stringShiftDelay);
  digitalWrite(RED_PIN, HIGH);
  delay(waitScroll);
  printStringWithShift("                ", stringShiftDelay);
  home_page();
  delay(waitScroll);
}
void alarm16off() {
  printStringWithShift("                ", stringShiftDelay);
  printStringWithShift("Alarm Turn Off", stringShiftDelay);
  digitalWrite(RED_PIN,  LOW);
  delay(waitScroll);
  printStringWithShift("                ", stringShiftDelay);
  home_page();
  delay(waitScroll);
}

void lamp00on() {
  printStringWithShift("                ", stringShiftDelay);
  printStringWithShift("Lamp Turn On", stringShiftDelay);
  digitalWrite(GREEN_PIN, HIGH);
  delay(waitScroll);
  printStringWithShift("                ", stringShiftDelay);
  home_page();
  delay(waitScroll);
}
void lamp00off() {
  printStringWithShift("                ", stringShiftDelay);
  printStringWithShift("Lamp Turn Off", stringShiftDelay);
  digitalWrite(GREEN_PIN,  LOW);  
  delay(waitScroll);
  printStringWithShift("                ", stringShiftDelay);
  home_page();
  delay(waitScroll);
}

void scroll12on() {
  printStringWithShift("                ", stringShiftDelay);
  printStringWithShift("Scroll Turn On", stringShiftDelay);
  digitalWrite(BLUE_PIN, HIGH);
  delay(waitScroll);
  printStringWithShift("                ", stringShiftDelay);
  home_page();
  delay(waitScroll);
}
void scroll12off() {
  printStringWithShift("                ", stringShiftDelay);
  printStringWithShift("Scroll Turn Off", stringShiftDelay);
  digitalWrite(BLUE_PIN,  LOW);
  delay(waitScroll);
  printStringWithShift("                ", stringShiftDelay);
  home_page();
  delay(waitScroll);
}

void ampmOnOff() {  // AM-PM Mode, 24: 24-hour clock, 12: 12-hour clock
  printStringWithShift("                ", stringShiftDelay);
  if (am_pm == 12) {
    am_pm = 24;
    printStringWithShift("AM-PM Mode Off", stringShiftDelay);
  } else {
    am_pm = 12;
    printStringWithShift("AM-PM Mode On", stringShiftDelay);
  }
  if (am_pm == 12) {
    if (alarm_h_set > 12) {
      alarm_h_set = alarm_h_set - 12;
      alarm_ampm_select = "PM";
    } else {
      alarm_ampm_select = "AM";
    }
  } else {
    if (alarm_ampm_select == "PM") {
      if (alarm_h_set == 12) {
        alarm_h_set = alarm_h_set - 12;
      } else {
        alarm_h_set = alarm_h_set + 12;
      }
    }
  }
  delay(waitScroll);
  printStringWithShift("                ", stringShiftDelay);
  ntpTime();
  home_page();
  delay(waitScroll);
}

void alarmTime() {
  String ampm_select;
  if (am_pm == 12) {
    ampm_select  = server.arg("ampm");
  }
  String alarm_h_time = server.arg("alarm_h_time");
  String alarm_m_time = server.arg("alarm_m_time");
  printStringWithShift("                ", stringShiftDelay);
  printStringWithShift("The alarm is set to ", stringShiftDelay);
  if (am_pm == 12) {
    printStringWithShift(ampm_select.c_str(), stringShiftDelay);
    printStringWithShift(" ", stringShiftDelay);
  }
  printStringWithShift(alarm_h_time.c_str(), stringShiftDelay);
  printStringWithShift(":", stringShiftDelay);
  printStringWithShift(alarm_m_time.c_str(), stringShiftDelay);
  if (am_pm == 12) {
    alarm_ampm_select = ampm_select;
  }
  alarm_h_set = alarm_h_time.toInt();
  alarm_m_set = alarm_m_time.toInt();
  onTimeAlarm = 0;
  printStringWithShift("                ", stringShiftDelay);
  home_page();
  delay(waitScroll);
}

void onTimeALARM() {
  printStringWithShift("                ", stringShiftDelay);
  printStringWithShift("On-time Alarm Setting", stringShiftDelay);
  onTimeAlarm = 1;
  alarm_h_set = 00;
  alarm_m_set = 00;
  delay(waitScroll);
  printStringWithShift("                ", stringShiftDelay);
  home_page();
  delay(waitScroll);
}

void alarmOnOff() {
  printStringWithShift("                ", stringShiftDelay);
  printStringWithShift("Alarm Setting", stringShiftDelay);
  if (alarm_state == 0) {
    alarm_state = 1;
  } else {
    alarm_state = 0;
  }
  delay(waitScroll);
  printStringWithShift("                ", stringShiftDelay);
  home_page();
  delay(waitScroll);
}

void button00() { printStringWithShift("                ", stringShiftDelay); printStringWithShift("I love you",                        stringShiftDelay); home_page(); delay(waitScroll); }
void button01() { printStringWithShift("                ", stringShiftDelay); printStringWithShift("Don't get upset",                   stringShiftDelay); home_page(); delay(waitScroll); }
void button02() { printStringWithShift("                ", stringShiftDelay); printStringWithShift("I'm upset",                         stringShiftDelay); home_page(); delay(waitScroll); }
void button03() { printStringWithShift("                ", stringShiftDelay); printStringWithShift("Catch me if you can",               stringShiftDelay); home_page(); delay(waitScroll); }
void button04() { printStringWithShift("                ", stringShiftDelay); printStringWithShift("You will never know until you try", stringShiftDelay); home_page(); delay(waitScroll); }
void button05() { printStringWithShift("                ", stringShiftDelay); printStringWithShift("Pain past is pleasure",             stringShiftDelay); home_page(); delay(waitScroll); }

// Web Server Start ========================================================
void Webserver_setup() {
  server.begin();
#ifdef USE_UART
  Serial.println("Server started");
  Serial.println();
  delay(10);
#endif

  server.on("/",                         home_page);

  server.on("/ClockOn",                   Clock_On);  // Define what happens when a client requests attention
  server.on("/ClockOff",                 Clock_Off);  // Define what happens when a client requests attention
  server.on("/showAnimClockOn",   showAnimClock_On);  // Define what happens when a client requests attention
  server.on("/showAnimClockOff", showAnimClock_Off);  // Define what happens when a client requests attention
  server.on("/dateScrollOn",         dateScroll_On);  // Define what happens when a client requests attention
  server.on("/dateScrollOff",       dateScroll_Off);  // Define what happens when a client requests attention
  server.on("/weatherScrollOn",   weatherScroll_On);  // Define what happens when a client requests attention
  server.on("/weatherScrollOff", weatherScroll_Off);  // Define what happens when a client requests attention
  server.on("/weatherSCROLL",        weatherSCROLL);  // Define what happens when a client requests attention
  server.on("/msg",                     handle_msg);  // And as regular external functions:
  server.on("/repeatprintMsg",      repeatprintMsg);  // And as regular external functions:
  server.on("/OTAmodeOn",                OTAmodeOn);  // And as regular external functions:
  server.on("/OTAmodeOff",              OTAmodeOff);  // And as regular external functions:
  server.on("/updateDATA",              updateDATA);  // And as regular external functions:
  server.on("/ESPrestart",              ESPrestart);  // And as regular external functions:
  server.on("/resetAP",                    resetAP);  // And as regular external functions:

  server.on("/ampmOnOff",                ampmOnOff);
  server.on("/alarmTime",                alarmTime);
  server.on("/onTimeALARM",            onTimeALARM);
  server.on("/alarmOnOff",              alarmOnOff);
  
  server.on("/inDoor",                      inDoor);
  server.on("/outDoor",                    outDoor);
  server.on("/inDoorScrollOn",      inDoorScrollOn);
  server.on("/inDoorScrollOff",    inDoorScrollOff);
  server.on("/outDoorScrollOn",    outDoorScrollOn);
  server.on("/outDoorScrollOff",  outDoorScrollOff);

  server.on("/alarm16on",                           alarm16on);
  server.on("/alarm16off",                         alarm16off);
  server.on("/lamp00on",                             lamp00on);
  server.on("/lamp00off",                           lamp00off);
  server.on("/scroll12on",                         scroll12on);
  server.on("/scroll12off",                       scroll12off);

  server.on("/button00",                  button00);
  server.on("/button01",                  button01);
  server.on("/button02",                  button02);
  server.on("/button03",                  button03);
  server.on("/button04",                  button04);
  server.on("/button05",                  button05);
}

void Webserver_action() {
  update_webpage();
  server.handleClient();  // Wait for a client to connect and when they do process their requests
}

#endif
