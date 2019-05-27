
#include <FastLED.h>

#define POT_PIN A1      //input pin for brightness control
#define DATA_PIN 5      //pin the control LED line

#define NUM_NODES 5     //number of nodes
#define NUM_LEDS 6

#define MIN_BRIGHTNESS 5
#define MAX_BRIGHTNESS 160

#define FRAMES_PER_SECOND  120

CRGB leds[NUM_NODES * NUM_LEDS];   //create array of fastLEDs


class Node {
public:
  Node(int _state = 0){
    state = _state;
  }
  int getState(){
    return state;
  }
  void changeState(){
    state = (state + 1) % 2;
  }
  void setState(int _state){
    state = _state;
  }
private:
  int state;
  
};

int showPatt = 0;
Node nodes[NUM_NODES];



void setBrightness(){
    int reading = analogRead(POT_PIN);
    FastLED.setBrightness(map(1023, 0, 1023, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
}


void updateLights(){
  for(int i = 0; i < NUM_NODES; i++){
    if (nodes[i].getState() == 1 && showPatt){
      for(int j = 0; j < NUM_LEDS; ++j)
        leds[NUM_LEDS*i + j] = CRGB::White;
    }
  }
  //showPatt = 0;
}

void setup() {
    Serial.begin(9600);
    pinMode(3, INPUT_PULLUP);

    delay(1000);

    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_NODES * NUM_LEDS);

    setBrightness();

    for(int i = 0; i < NUM_NODES; i++)
      nodes[i] = Node(0);


}


void loop(){
  for(int i = 0; i < NUM_NODES; i++){
    if (analogRead(A0 + i) < 600){
      nodes[i].setState(1);
    }
  }

  if(digitalRead(3) == 0){
    for(int i = 0; i < NUM_NODES; i++)
      nodes[i].setState(0);
  }
  //EVERY_N_MILLISECONDS( 500 ) { showPatt = 1; }

  updateLights();

  
  
  //delay(20);
  //fadeToBlackBy(leds,8,16);

  FastLED.show();
  FastLED.delay(1000/FRAMES_PER_SECOND); 

}
