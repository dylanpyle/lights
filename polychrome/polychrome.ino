#include "LPD8806.h"
#include "SPI.h"

int nLEDs = 32;

int dataPin  = 13;
int clockPin = 12;

LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);

void setup() {
  strip.begin();
  strip.show();
  Serial.begin(9600);
}

void loop() {
  if(Serial.available() > 0){
    char buffer[] = {' ',' ',' '};
    Serial.readBytesUntil('$', buffer, 3);
    Serial.println("Read something");
    colorWipe(strip.Color(buffer[0], buffer[1], buffer[2]), 20);
  }
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
   
  for (j=0; j < 384; j++) {     // 3 cycles of all 384 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 384));
    }
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  
  for (j=0; j < 384 * 5; j++) {     // 5 cycles of all 384 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 384-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 384 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 384 / strip.numPixels()) + j) % 384) );
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// Fill the dots progressively along the strip.
void colorWipe(uint32_t c, uint8_t wait) {
  int i;

  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void pixelHaze(uint8_t wait) {
  int i;

  for (i=0; i < 200; i++) {
      int pin = random(0, strip.numPixels()-1);
      int brightness = random(0,127);
      uint32_t color = strip.Color(random(0, brightness), random(0,brightness), random(0,brightness));
      
      strip.setPixelColor(pin, color);
      strip.show();
      delay(wait);
  }
}


// Chase one dot down the full strip.
void colorChase(uint32_t c, uint8_t wait) {
  int i;

  // Start by turning all pixels off:
  for(i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);

  // Then display one pixel at a time:
  for(i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c); // Set new pixel 'on'
    strip.show();              // Refresh LED states
    strip.setPixelColor(i, 0); // Erase pixel, but don't refresh!
    delay(wait);
  }

  strip.show(); // Refresh to turn off last pixel
}

/* Helper functions */

//Input a value 0 to 384 to get a color value.
//The colours are a transition r - g -b - back to r

uint32_t Wheel(uint16_t WheelPos)
{
  byte r, g, b;
  switch(WheelPos / 128)
  {
    case 0:
      r = 127 - WheelPos % 128;   //Red down
      g = WheelPos % 128;      // Green up
      b = 0;                  //blue off
      break; 
    case 1:
      g = 127 - WheelPos % 128;  //green down
      b = WheelPos % 128;      //blue up
      r = 0;                  //red off
      break; 
    case 2:
      b = 127 - WheelPos % 128;  //blue down 
      r = WheelPos % 128;      //red up
      g = 0;                  //green off
      break; 
  }
  return(strip.Color(r,g,b));
}
