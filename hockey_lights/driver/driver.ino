#include <SoftwareSerial.h>
#include <FastLED.h>


#define RX 2;           //RX/TX for Nodes 
#define TX 3;

#define POT_PIN A1      //Input pin for brightness control
#define DATA_PIN 5      //pin the control LED line

#define NUM_NODES 7     //number of nodes

#define MIN_BRIGHTNESS 5
#define MAX_BRIGHTNESS 160

CRGB leds[NUM_NODES];   //create array of fastLEDs

SoftwareSerial stringSerial(RX,TX);

void setBrightness(){
    int reading = analogRead(POT_PIN);
    FastLED.setBrightness(map(reading, 0, 1023, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
}

void setup() {
    Serial.begin(9600);
    stringSerial.begin(9600);

    delay(1000);

    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
    setBrightness();


    


    
}


void loop(){




}




FastLED.show();

int reading = analogRead(POT_PIN);
FastLED.setBrightness(map(reading, 0, 1023, MIN_BRIGHTNESS, MAX_BRIGHTNESS));