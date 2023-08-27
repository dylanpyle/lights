#include "LPD8806.h"
#include "SPI.h"

int nLEDs = 20;

int dataPin = 13;
int clockPin = 12;
int maxColorValue = 127;

LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);

int currentProgram;
int programCount = 6;

void setup() {
  strip.begin();
  strip.show();

  Serial.begin(9600);

  randomSeed(analogRead(0));
  currentProgram = random(0, programCount - 1);
}

void loop() {
  Serial.println("running program:");
  Serial.println(currentProgram);

  if (currentProgram == 0) {
    for (int i = 0; i < 5; i++) {
      double r = random(0, maxColorValue);
      double g = random(0, maxColorValue);
      double b = random(0, maxColorValue);
      colorChase(strip.Color(r, g, b), 50);
    }
  } else if (currentProgram == 1) {
    for (int i = 0; i < 3; i++) {
      rainbow(20);
    }
  } else if (currentProgram == 2) {
    double r = random(0, maxColorValue);
    double g = random(0, maxColorValue);
    double b = random(0, maxColorValue);
    colorWipe(strip.Color(r, g, b), 50);
  } else if (currentProgram == 3) {
    for (int i = 0; i < 3; i++) {
      rainbowCycle(20);
    }
  } else if (currentProgram == 4) {
    double r = random(0, maxColorValue);
    double g = random(0, maxColorValue);
    double b = random(0, maxColorValue);
    colorWipe(strip.Color(r, g, b), 50);
  } else if (currentProgram == 5) {
    for (int i = 0; i < 3; i++) {
      pixelHaze(10);
    }
  }

  currentProgram = (currentProgram + 1) % programCount;
}

/*
  A small-but-expanding collection of nice patterns. Examples:

  colorChase(strip.Color(127, 127, 127), 50);
  colorWipe(strip.Color(  127,   127, 127), 20);
  rainbowCycle(0);
  pixelHaze(20);
  rainbow(0);
*/

void rainbow(uint8_t wait) {
  int i, j;

  for (j = 0; j < 384; j++) {  // 3 cycles of all 384 colors in the wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) % 384));
    }
    strip.show();  // write all the pixels out
    delay(wait);
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 384 * 5; j++) {  // 5 cycles of all 384 colors in the wheel
    for (i = 0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 384-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 384 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel(((i * 384 / strip.numPixels()) + j) % 384));
    }
    strip.show();  // write all the pixels out
    delay(wait);
  }
}

// Fill the dots progressively along the strip.
void colorWipe(uint32_t c, uint8_t wait) {
  int i;

  for (i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

uint32_t hslToRgb(double h, double s, double l) {
  double r, g, b;

  if (s == 0) {
    r = g = b = l;  // achromatic
  } else {
    double q = l < 0.5 ? l * (1 + s) : l + s - l * s;
    double p = 2 * l - q;
    r = hueToRgb(p, q, h + 1 / 3);
    g = hueToRgb(p, q, h);
    b = hueToRgb(p, q, h - 1 / 3);
  }

  return strip.Color(round(r * maxColorValue), round(g * maxColorValue), round(b * maxColorValue));
}

uint8_t hueToRgb(double p, double q, double t) {
  if (t < 0) t += 1;
  if (t > 1) t -= 1;
  if (t < 1 / 6) return p + (q - p) * 6 * t;
  if (t < 1 / 2) return q;
  if (t < 2 / 3) return p + (q - p) * (2 / 3 - t) * 6;
  return p;
}


void pixelHaze(uint8_t wait) {
  int i;

  for (i = 0; i < 200; i++) {
    int pin = random(0, strip.numPixels());
    double h = random(0, maxColorValue);
    double s = random(4, maxColorValue);
    double l = random(0, maxColorValue);
    uint32_t color = hslToRgb(h, s, l);

    strip.setPixelColor(pin, strip.Color(h, s, l));
    strip.show();
    delay(wait);
  }
}


// Chase one dot down the full strip.
void colorChase(uint32_t c, uint8_t wait) {
  int i;

  // Start by turning all pixels off:
  for (i = 0; i < strip.numPixels(); i++) strip.setPixelColor(i, 0);

  // Then display one pixel at a time:
  for (i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);  // Set new pixel 'on'
    strip.show();               // Refresh LED states
    strip.setPixelColor(i, 0);  // Erase pixel, but don't refresh!
    delay(wait);
  }

  strip.show();  // Refresh to turn off last pixel
}

/* Helper functions */

//Input a value 0 to 384 to get a color value.
//The colours are a transition r - g -b - back to r

uint32_t Wheel(uint16_t WheelPos) {
  byte r, g, b;
  switch (WheelPos / 128) {
    case 0:
      r = maxColorValue - WheelPos % 128;  //Red down
      g = WheelPos % 128;                  // Green up
      b = 0;                               //blue off
      break;
    case 1:
      g = maxColorValue - WheelPos % 128;  //green down
      b = WheelPos % 128;                  //blue up
      r = 0;                               //red off
      break;
    case 2:
      b = maxColorValue - WheelPos % 128;  //blue down
      r = WheelPos % 128;                  //red up
      g = 0;                               //green off
      break;
  }
  return (strip.Color(r, g, b));
}
