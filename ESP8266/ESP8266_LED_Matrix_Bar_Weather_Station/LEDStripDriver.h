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

#ifndef __LEDStripDriver_H__
#define __LEDStripDriver_H__

#include <Arduino.h>

class LEDStripDriver {
    public:
        // constructors
        LEDStripDriver(uint8_t din, uint8_t cin);
        LEDStripDriver(uint8_t din, uint8_t cin, uint16_t delay);

        // color manipulation
        void setColor(uint8_t red, uint8_t green, uint8_t blue);
        void setColor(uint32_t hex);
        void setColor(String str);
        void setColor();

    private:
        uint8_t _din, _cin;
        uint16_t _delay;

        // private methods
        uint8_t getColorCode(uint8_t color);
        void senddata(uint32_t dx);
        void sendzero();
        void sendclock();
};

#endif
