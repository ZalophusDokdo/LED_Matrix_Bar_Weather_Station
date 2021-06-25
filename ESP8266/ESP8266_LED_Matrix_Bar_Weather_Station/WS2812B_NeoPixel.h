/* =========================================================================
 *  Author: Zalophus Dokdo (https://zddh.blogspot.com)
 *  Date: 31/08/2017       (https://zalophus.tistory.com/)
 *  License: GPL v2
 * =========================================================================
 *  LED Matrix Bar Weather Station V1.0.4 (Publish: 2018/01/02)
 * =========================================================================
 *  WS2812B RGB LED
 * =========================================================================
 */

#ifndef WS2812B_NEOPIXEL_H
#define WS2812B_NEOPIXEL_H

#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
//#define  RGB_LED_PIN    4   // D2 (GPIO04)
// How many NeoPixels are attached to the Arduino?
//#define  NUMPIXELS     1   //16   // 64  // 16
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, USE_RGB_LED_PIN, NEO_GRB + NEO_KHZ800);
int delayval = 200;       // delay for half a second

// WS2812B RGB LED strip
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j = 0; j < 256; j++) {
    for(i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j = 0; j < 256*5; j++) { // 5 cycles of all colors on wheel
    for(i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) {          // do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);    // turn every third pixel on
      }
      strip.show();
      delay(wait);
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);    // turn every third pixel off
      }
    }
  }
}

// Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {       // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel((i + j) % 255));    // turn every third pixel on
      }
      strip.show();
      delay(wait);
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);        // turn every third pixel off
      }
    }
  }
}

void NeoPixel_setup() {
  strip.begin();
  strip.show();  // Initialize all pixels to 'off'
}

void NeoPixel_action() {
  // WS2812B RGB LED
  // Some example procedures showing how to display to the pixels:
#ifdef USE_UART
  Serial.println("WS2812B RGB LED Strip (D2 - 4)");
#endif
  colorWipe(strip.Color(255,   0,   0), 50);     // Red
  colorWipe(strip.Color(  0, 255,   0), 50);     // Green
  colorWipe(strip.Color(  0,   0, 255), 50);     // Blue
  colorWipe(strip.Color(255, 255, 255), 50);     // White
  colorWipe(strip.Color(  0,   0,   0), 50);     // Black
  //colorWipe(strip.Color(  0,   0,   0, 255), 50);  // White RGBW
  // Send a theater pixel chase in...
  theaterChase(strip.Color(127, 127, 127), 50);  // White
  theaterChase(strip.Color(127,   0,   0), 50);  // Red
  theaterChase(strip.Color(  0, 127,   0), 50);  // Green
  theaterChase(strip.Color(  0,   0, 127), 50);  // Blue

  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);
}

#endif
