#include <FastLED.h>
#include <pixeltypes.h>

// How many leds in your strip?
#define NUM_LEDS 24

#define DATA_PIN 10
#define BRIGHTNESS 100

// Define the array of leds
CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

const int microphonePin = A0;
int MODE = 0;

void setup() {
  Serial.begin(9600);
  Serial.println(F("Initialize System"));
  pinMode(microphonePin, INPUT);
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS );
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;

}

void loop() {
  FastLED.clear();
  int mn = 1024;
  int mx = 0;

  for (int i = 0; i < 100; ++i) {
    int val = analogRead(microphonePin);
    mn = min(mn, val);
    mx = max(mx, val);
  }
  int delta = mx - mn;

  Serial.println(delta);

  // Turn the LED on, then pause
  if (delta > 20) {
    switch (MODE) {
      case 0:
        MODE = 1;
        Serial.println("In Neon");
        for (int i = 0; i < NUM_LEDS; i++) {
          leds[i] = CRGB::Blue;
          FastLED.show();
          delay(100);
        }
        break;
      case 1:
        MODE = 2;
        Serial.println("In Warm");
        for (int i = 0; i < NUM_LEDS; i++) {
          leds[i].setRGB( 150, 255, 50);
          FastLED.show();
          delay(100);
        }
        break;
      case 2:
        MODE = 3;
        Serial.println("In RGB");
        ChangePalettePeriodically();
        static uint8_t startIndex = 0;
        startIndex = startIndex + 1; /* motion speed */
        FillLEDsFromPaletteColors( startIndex);
        FastLED.show();
        FastLED.delay(1000 / 100);
        break;
      case 3:
        MODE = 0;
        Serial.println("In off");
        for (int i = 24; i > 0; i--) {
          leds[i] = CRGB::Black;
          FastLED.show();
          delay(100);
        }
        break;
      default:
        Serial.println("In default");
        FastLED.clear();
    }
  }
  else {
    Serial.println("In clear");

    FastLED.clear();
  }



  // Now turn the LED off, then pause
  //  leds[0] = CRGB::Black;
  //  FastLED.show();
  //  delay(500);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = 255;

  for ( int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}

void ChangePalettePeriodically()
{
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;

  if ( lastSecond != secondHand) {
    lastSecond = secondHand;
    if ( secondHand ==  0)  {
      currentPalette = RainbowColors_p;
      currentBlending = LINEARBLEND;
    }
  }
}
