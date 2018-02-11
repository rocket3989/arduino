#include <FastLED.h>

//#include <avr/interrupt.h>
#define NUM_LEDS 16
#define DATA_PIN 0
#define POT_PIN A1
#define BUTTON_PIN 1
#define MIN_BRIGHTNESS 5
#define MAX_BRIGHTNESS 160
#define NUM_PATTERNS 7
CRGB leds[NUM_LEDS];
volatile int pattern = 0;
unsigned long sensorDelay = 40;
unsigned long lastSensor = 0;
bool lastButtonState = HIGH;
int timer;


void redGreen() {
  if (timer%1000>500) {
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

void fallingLights(){
  int state = 3-(timer%2000)/500;
  for (int i = 0; i<4; i++){
    leds[i*4+(state+i)%4] = CRGB::White;
  }
  fadeToBlackBy(leds,16,20);
}

//Sweep around the HSV spectrum once every 5 seconds
void hueSweep() {
  byte hue = timer / 20;
  CRGB currColor = CHSV(hue, 255,200);
  for(int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = currColor;
  }
}
void dark(){
   for (int i = 0; i< NUM_LEDS;i++){
      leds[i]=CRGB::Black;
   }
}
void rainbow(){
   byte hue = timer / 20;
   fill_rainbow(leds,NUM_LEDS,hue,16);
}
void glitter(){
  if( random8() < 50)
    leds[ random16(NUM_LEDS) ] += CRGB::White;
}

void setup() {
  delay(2000);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(40);
}
  
void loop() {
  timer = millis()%10000;
  switch(pattern){
    case 0:
      fallingLights();
    break;
    case 1:
      redGreen();
    break;
    case 2:
      hueSweep();
    break;
    case 3:
      rainbow();
    break;
    case 4:
      rainbow();
      glitter();
    break;
    case 5:
      switch(timer/2000){
          case 0:
            fallingLights();
          break;
          case 1:
            redGreen();
          break;
          case 2:
            hueSweep();
          break;
          case 3:
            rainbow();
          break;
          case 4:
            rainbow();
            glitter();
          break;
      }
    break;
    case 6:
      dark();
    break;
  }
  FastLED.show();
  
  int reading = analogRead(POT_PIN);
  FastLED.setBrightness(map(reading, 0, 1023, MIN_BRIGHTNESS, MAX_BRIGHTNESS));

  bool buttonState = digitalRead(BUTTON_PIN);
  if(buttonState != lastButtonState && buttonState == HIGH) {
    ++pattern;
    pattern = pattern % NUM_PATTERNS;
  }
  lastButtonState = buttonState;
  delay(15);
}


/*
ISR(PCINT0_vect){
//  pattern++;
//  pattern = pattern %2;
  leds[0] = CRGB::Red;
  FastLED.show();
} 
*/
