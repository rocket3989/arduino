#include <SoftwareSerial.h>
#include <FastLED.h>


#define RX 2            //RX/TX for Nodes 
#define TX 3

#define POT_PIN A1      //input pin for brightness control
#define DATA_PIN 5      //pin the control LED line

#define NUM_NODES 10     //number of nodes
#define NUM_LEDS 2

#define MIN_BRIGHTNESS 5
#define MAX_BRIGHTNESS 160

CRGB leds[NUM_NODES * NUM_LEDS];   //create array of fastLEDs

SoftwareSerial stringSerial(RX,TX);


class Node {
public:
  Node(int _state){
    state = _state;
  }
  int getState(){
    return state;
  }
  void changeState(){
    state = (state + 1) % 2;
  }
private:
  int state;
  
};


Node nodes[NUM_NODES];



void setBrightness(){
    int reading = analogRead(POT_PIN);
    FastLED.setBrightness(map(1023, 0, 1023, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
}


void updateLights(){
  for(int i = 0; i < NUM_NODES; i++){
    if (nodes[i].getState() == 0){
      leds[NUM_LEDS * i] = CRGB::Red;
      leds[NUM_LEDS * i + 1] = CRGB::Red;
    }
    else{
      leds[NUM_LEDS * i] = CRGB::Green;
      leds[NUM_LEDS * i + 1] = CRGB::Green;
    }
  }
}

void setup() {
    Serial.begin(9600);
    stringSerial.begin(9600);

    delay(1000);

    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_NODES * 2);

    setBrightness();

    for(int i = 0; i < NUM_NODES; i++)
      nodes[i] = Node(i % 2);


}


void loop(){
  if(stringSerial.available()){
    int lit = stringSerial.read() - 0x30;

    if(lit >= 0 || lit < NUM_NODES - 1)
      nodes[lit].changeState();
    updateLights();

    winner = true;
    for(int i = 1; i < NUM_NODES; ++i)
      if(nodes[i].getState() != nodes[i - 1].getState())
        winner = false;
    
    if(winner){
      for(int j = 0; j < 5; ++j){
        for(int i = 0; i < 20; ++i){
          fadeToBlackBy(leds,NUM_NODES,16);
          delay(10);
        }
        updateLights();
      }
      for(int i = 0; i < NUM_NODES; i++)
        nodes[i] = Node(i % 2);
      updateLights();

    }
    
  }
  delay(20);
  fadeToBlackBy(leds,5,16);

  FastLED.show();

}
