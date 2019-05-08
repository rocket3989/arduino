#include <SoftwareSerial.h>
#include <FastLED.h>


#define RX 2           //RX/TX for Nodes 
#define TX 3

#define POT_PIN A1      //Input pin for brightness control
#define DATA_PIN 5      //pin the control LED line

#define NUM_NODES 2     //number of nodes

#define MIN_BRIGHTNESS 5
#define MAX_BRIGHTNESS 160

CRGB leds[NUM_NODES * 2];   //create array of fastLEDs

SoftwareSerial stringSerial(RX,TX);

void setBrightness(){
    int reading = analogRead(POT_PIN);
    FastLED.setBrightness(map(1023, 0, 1023, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
}

void setup() {
    Serial.begin(9600);
    stringSerial.begin(9600);

    delay(1000);

    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_NODES * 2);
    setBrightness();


}


void loop(){
  if(stringSerial.available()){
    int lit = stringSerial.read() - 0x30;
    lit = 1 - lit;
    if(lit >= 0 || lit < NUM_NODES -1){
      leds[2 * lit] = CRGB::White;
      leds[2 * lit + 1] = CRGB::White;
    }
    
  }
  delay(20);
  fadeToBlackBy(leds,5,16);

  FastLED.show();

}
