#include "FastLED.h"
#include <avr/interrupt.h>
#define NUM_LEDS 16
#define DATA_PIN 0
CRGB leds[NUM_LEDS];
volatile int pattern = 0;

void RedGreen() {
  if (millis() % 1000 > 500) {
    for (int i = 0; i < NUM_LEDS; i++) {
      if (i % 2)
        leds[i] = CRGB::Red;
      else
        leds[i] = CRGB::Green;
    }
  }
  else
  {
    for (int i = 0; i < NUM_LEDS; i++) {
      if (i % 2)
        leds[i] = CRGB::Green;
      else
        leds[i] = CRGB::Red;

    }
  }
}

void FallingLights(){
  int state = 3-(millis()%2000)/500;
  for (int i = 0; i<4; i++){
    leds[i*4+(state+i)%4] = CRGB::White;
  }
  fadeToBlackBy(leds,16,10);
  

  
}




void setup() {
  delay(2000);
  pinMode(1,INPUT_PULLUP);
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(60);
  attachInterrupt(1,INF,FALLING);
}
void loop() {
  if (pattern == 0)
    FallingLights();
  else
   RedGreen();
  FastLED.show();
  delay(10);
  
}
void INF(){
  pattern++;
  pattern = pattern %2;
}