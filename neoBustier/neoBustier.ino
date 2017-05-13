#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

///////////////////
// CONFIGURATION //
///////////////////
#define NBPIXELS1 51
#define NBPIXELS2 51
#define PIN1 5
#define PIN2 6


// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NBPIXELS1, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NBPIXELS2, PIN2, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code


  strip1.begin();
  strip1.show(); // Initialize all pixels to 'off'
  strip2.begin();
  strip2.show(); // Initialize all pixels to 'off'
}

void loop() {
  // Some example procedures showing how to display to the pixels:
  colorWipe(strip1.Color(25, 0, 0), 50); // Red
  colorWipe(strip1.Color(0, 25, 0), 50); // Green
  colorWipe(strip1.Color(0, 0, 25), 50); // Blue
  // Send a theater pixel chase in...
 // theaterChase(strip1.Color(127, 127, 127), 50); // White
 // theaterChase(strip1.Color(127, 0, 0), 50); // Red
  //theaterChase(strip1.Color(0, 0, 127), 50); // Blue

  rainbow(20);
  rainbowCycle(20);
 // theaterChaseRainbow(50);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip1.numPixels(); i++) {
    strip1.setPixelColor(i, c);
    strip1.show();
    strip2.setPixelColor(i, c);
    strip2.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip1.numPixels(); i++) {
      strip1.setPixelColor(i, Wheel((i+j) & 255));
      strip2.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip1.show();
    strip2.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip1.numPixels(); i++) {
      strip1.setPixelColor(i, Wheel(((i * 256 / strip1.numPixels()) + j) & 255));
      strip2.setPixelColor(i, Wheel(((i * 256 / strip1.numPixels()) + j) & 255));
    }
    strip1.show();
    strip2.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip1.numPixels(); i=i+3) {
        strip1.setPixelColor(i+q, c);    //turn every third pixel on
        strip2.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip1.show();
      strip2.show();

      delay(wait);

      for (uint16_t i=0; i < strip1.numPixels(); i=i+3) {
        strip1.setPixelColor(i+q, 0);        //turn every third pixel off
        strip2.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip1.numPixels(); i=i+3) {
        strip1.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        strip2.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip1.show();
      strip2.show();

      delay(wait);

      for (uint16_t i=0; i < strip1.numPixels(); i=i+3) {
        strip1.setPixelColor(i+q, 0);        //turn every third pixel off
        strip2.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip1.Color((255 - WheelPos * 3)/5, 0, (WheelPos * 3)/5);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip1.Color(0, (WheelPos * 3)/5, (255 - WheelPos * 3)/5);
  }
  WheelPos -= 170;
  return strip1.Color((WheelPos * 3)/5, (255 - WheelPos * 3)/5, 0);
}
