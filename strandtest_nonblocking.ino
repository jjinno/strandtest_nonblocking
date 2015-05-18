#include <Adafruit_NeoPixel.h>

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(50, 13, NEO_GRB + NEO_KHZ800);

#define STRIP_SIZE (strip.numPixels())

int mode = 0;

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  mode=0;

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  
  mode = 6;
  switch (mode) {
    case 1:
      colorWipe(strip.Color(255, 0, 0), 50); // Red
      break;
    case 2:  
      colorWipe(strip.Color(0, 255, 0), 50); // Green
      break;
    case 3:
      colorWipe(strip.Color(0, 0, 255), 50); // Blue
      break;
    case 4:
      theaterChase(strip.Color(127, 127, 127), 50); // White
      break;
    case 5:
      theaterChase(strip.Color(127,   0,   0), 50); // Red
      break;
    case 6:
      theaterChase(strip.Color(  0,   0, 127), 1); // Blue
      break;
    case 7:
//      rainbow(20);
      rainbowCycle(2);
      break;
    case 8:
      theaterChaseRainbow(50);
      break;
    default: // off
      break;
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<STRIP_SIZE; i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}


struct RainbowStruct
{
  uint16_t i;
  uint16_t j;
};
struct RainbowStruct rainbow_s;
void rainbow(uint8_t wait) {
  rainbow_s.i++;
  if (rainbow_s.i == STRIP_SIZE) {
    rainbow_s.i=0; 
    rainbow_s.j++; 
    if (rainbow_s.j == 256)
      rainbow_s.j=0;
  }
  
  strip.setPixelColor(rainbow_s.i, 
    Wheel((rainbow_s.i+rainbow_s.j) & 255));
  strip.show();
  delay(wait);
}

struct RainbowStruct rainbowCycle_s;
void rainbowCycle(uint8_t wait) {
  rainbowCycle_s.i++;
  if (rainbowCycle_s.i == STRIP_SIZE) {
    rainbowCycle_s.i = 0;
    rainbowCycle_s.j++;
    if (rainbowCycle_s.j == 256)
      rainbowCycle_s.j = 0;
  }
  
  strip.setPixelColor(rainbowCycle_s.i, 
    Wheel(((rainbowCycle_s.i * 256 / STRIP_SIZE) + rainbowCycle_s.j) & 255));
  strip.show();
  delay(wait);
}

struct TheaterStruct {
  int phase;
  int j;
  int i;
};
struct TheaterStruct theaterChase_s;
void theaterChase(uint32_t c, uint8_t wait) {
  theaterChase_s.i++;
  if (theaterChase_s.i == STRIP_SIZE) {
    theaterChase_s.i = 0;
    if (theaterChase_s.phase < 3)
      theaterChase_s.phase++;
    if (theaterChase_s.phase == 4) {
      theaterChase_s.phase = 0;
    }
  }
  
  if (theaterChase_s.phase == 3) {
    delay(1);
    theaterChase_s.j++;
    if (theaterChase_s.j == wait) {
      theaterChase_s.phase = 0;
      theaterChase_s.i = 0;
      theaterChase_s.j = 0;
    }
    return;
  }
  
  if (theaterChase_s.i % 3 == theaterChase_s.phase) {
    strip.setPixelColor(theaterChase_s.i, c);
    strip.show();
  }
  else {
    strip.setPixelColor(theaterChase_s.i, 0);
    strip.show();
  }
}

//Theatre-style crawling lights with rainbow effect
struct TheaterStruct theaterChaseRainbow_s;
void theaterChaseRainbow(uint8_t wait) {
  theaterChaseRainbow_s.i++;
  if (theaterChaseRainbow_s.i == STRIP_SIZE) {
    theaterChaseRainbow_s.i = 0;
    if (theaterChaseRainbow_s.phase < 3)
      theaterChaseRainbow_s.phase++;
    if (theaterChaseRainbow_s.phase == 4) {
      theaterChaseRainbow_s.phase = 0;
    }
  }
  
  if (theaterChaseRainbow_s.phase == 3) {
    delay(1);
    theaterChaseRainbow_s.j++;
    if (theaterChaseRainbow_s.j == wait) {
      theaterChaseRainbow_s.phase = 0;
      theaterChaseRainbow_s.i = 0;
      theaterChaseRainbow_s.j = 0;
    }
    return;
  }
  
  if (theaterChaseRainbow_s.i % 3 == theaterChaseRainbow_s.phase) {
    strip.setPixelColor(theaterChaseRainbow_s.i, Wheel( theaterChaseRainbow_s.i % 255));
    strip.show();
  }
  else {
    strip.setPixelColor(theaterChaseRainbow_s.i, 0);
    strip.show();
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color((255 - WheelPos * 3)/5, 0, (WheelPos * 3)/5);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, (WheelPos * 3)/5, (255 - WheelPos * 3)/5);
  } else {
   WheelPos -= 170;
   return strip.Color((WheelPos * 3)/5, (255 - WheelPos * 3)/5, 0);
  }
}
