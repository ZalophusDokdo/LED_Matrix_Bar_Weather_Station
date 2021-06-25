/* =========================================================================
 *  Author: Zalophus Dokdo (https://zddh.blogspot.com)
 *  Date: 31/08/2017       (https://zalophus.tistory.com/)
 *  License: GPL v2
 * =========================================================================
 *  LED Matrix Bar Weather Station V1.0.4a (Publish: 2018/01/02)
 * =========================================================================
 *  RGB LED Driver
 *  MIT license
 *  written by Renzo Mischianti
 * =========================================================================
 */

#ifndef LED_STRIP_DRIVER_H
#define LED_STRIP_DRIVER_H

#include "LEDStripDriver.h"

LEDStripDriver led = LEDStripDriver(LEDSTRIPDRIVER_DIN, LEDSTRIPDRIVER_CIN);

void ledStripColorWipe() {
  for (uint16_t color = 0; color < 256; color++) {
    led.setColor(color,     0,     0);
    delay(10);
  }
  for (uint16_t color = 0; color < 256; color++) {
    led.setColor(    0, color,    0);
    delay(10);
  }
  for (uint16_t color = 0; color < 256; color++) {
    led.setColor(    0,    0, color);
    delay(10);
  }
  for (uint16_t color = 0; color < 256; color++) {
    led.setColor(color, color, color);
    delay(10);
  }
  led.setColor();
}

void ledStrip_action() {
  // put your main code here, to run repeatedly:
  led.setColor(255, 0, 0); // RGB
  delay(1000);
  led.setColor("#00FF00"); // Hex String
  delay(1000);
  led.setColor(0x0000FF);  // Hex
  delay(1000);
  led.setColor();          // turn off
  delay(1000);
}

#endif
