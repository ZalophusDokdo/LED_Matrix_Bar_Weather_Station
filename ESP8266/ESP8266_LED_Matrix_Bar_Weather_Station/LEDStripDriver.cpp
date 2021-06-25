/* =========================================================================
 *  Author: Zalophus Dokdo (https://zddh.blogspot.com)
 *  Date: 31/08/2017       (https://zalophus.tistory.com/)
 *  License: GPL v2
 * =========================================================================
 *  LED Matrix Bar Weather Station V1.0.4a (Publish: 2018/01/02)
 * =========================================================================
 *  RGB LED Driver library
 *  MIT license
 *  written by Renzo Mischianti
 * =========================================================================
 */

#include <Arduino.h>

#include "LEDStripDriver.h"

/**************************************************************************/
/*!
    @brief class constructor
    @param din the data pin to use
    @param cin the clock pin to use
*/
/**************************************************************************/
LEDStripDriver::LEDStripDriver(uint8_t din, uint8_t cin) : _din(din), _cin(cin), _delay(20) {
    pinMode(din, OUTPUT);
    pinMode(cin, OUTPUT);
}

/**************************************************************************/
/*!
    @brief class constructor
    @param din the data pin to use
    @param cin the clock pin to use
    @param delay the clock delay in microseconds
*/
/**************************************************************************/
LEDStripDriver::LEDStripDriver(uint8_t din, uint8_t cin, uint16_t delay) : _din(din), _cin(cin), _delay(delay) {
    pinMode(din, OUTPUT);
    pinMode(cin, OUTPUT);
}

/**************************************************************************/
/*!
    @brief set color from r,g,b parameter
    @param red amount of red color [0-255]
    @param green amount of green color [0-255]
    @param blue amount of blue color [0-255]
*/
/**************************************************************************/
void LEDStripDriver::setColor(uint8_t red, uint8_t green, uint8_t blue) {
    uint32_t dx = 0;
    dx |= (uint32_t)0x03 << 30;
    dx |= (uint32_t)getColorCode(blue) << 28;
    dx |= (uint32_t)getColorCode(green) << 26;
    dx |= (uint32_t)getColorCode(red) << 24;

    dx |= (uint32_t)blue << 16;
    dx |= (uint32_t)green << 8;
    dx |= red;

    senddata(dx);
}

/**************************************************************************/
/*!
    @brief set color from hex parameter
    @param hex hex number [0x000000-0xFFFFFF]
*/
/**************************************************************************/
void LEDStripDriver::setColor(uint32_t hex) {
    uint8_t red   = (hex >> 16) & 0xFF;
    uint8_t green = (hex >> 8) & 0xFF;
    uint8_t blue  = (hex) & 0xFF;
    setColor(red, green, blue);
}

/**************************************************************************/
/*!
    @brief set color from hex string
    @param str string of hex color, e.g. "#00FF00"
*/
/**************************************************************************/
void LEDStripDriver::setColor(String str) {
    uint32_t hex = (uint32_t)strtol(&str[1], NULL, 16);
    setColor(hex);
}

/**************************************************************************/
/*!
    @brief set color to off
*/
/**************************************************************************/
void LEDStripDriver::setColor() {
    setColor(0, 0, 0);
}

/**************************************************************************/
/*!
    @brief send data over data pin
    @param dx data to transmitt
*/
/**************************************************************************/
void LEDStripDriver::senddata(uint32_t dx) {
    sendzero();
    for (uint8_t i = 0; i < 32; i++) {
        if ((dx & 0x80000000) != 0) {
            digitalWrite(_din, HIGH);
        } else {
            digitalWrite(_din, LOW);
        }
        dx <<= 1;
        sendclock();
    }
    sendzero();
}

/**************************************************************************/
/*!
    @brief send a sequence of zeros
*/
/**************************************************************************/
void LEDStripDriver::sendzero() {
    for (uint8_t i = 0; i < 32; i++) {
        digitalWrite(_din, LOW);
        sendclock();
    }
}

/**************************************************************************/
/*!
    @brief send one clock impulse
*/
/**************************************************************************/
void LEDStripDriver::sendclock() {
    digitalWrite(_cin, LOW);
    delayMicroseconds(_delay);
    digitalWrite(_cin, HIGH);
    delayMicroseconds(_delay);
}

/**************************************************************************/
/*!
    @brief convert color to code
*/
/**************************************************************************/
uint8_t LEDStripDriver::getColorCode(uint8_t color) {
    uint8_t tmp = 0;
    if ((color & 0x80) == 0) {
        tmp |= 0x02;
    }
    if ((color & 0x40) == 0) {
        tmp |= 0x01;
    }
    return tmp;
}
