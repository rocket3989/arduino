#include "font.h"

int clockPin = 2;
int latchPin = 1;
int dataPin = 0;

void setup(){
  pinMode(clockPin,OUTPUT);
  pinMode(latchPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
  pinMode(4,OUTPUT);
  digitalWrite(clockPin,HIGH);
  digitalWrite(latchPin,LOW);
}


void registerWrite(byte output){
  
  for(int i=0; i<8; i++){
    
    delayMicroseconds(6);
    digitalWrite(clockPin, LOW);
    
    digitalWrite(dataPin,(output >> i) & 2); 
  
    delayMicroseconds(6);
    digitalWrite(clockPin, HIGH);
    
  }

  digitalWrite(latchPin, HIGH);
  delayMicroseconds(12);
  digitalWrite(latchPin, LOW);
}


void displayString(char* input){
  for (int i = 0; i < strlen(input); ++i){
    for(int j = 0; j < 5; ++j){
      registerWrite(font_5x7[input[i] - 0x20][j]);
      delayMicroseconds(500);;
    }
    registerWrite(0);
    delay(3);
  }
  delay(5);
  
}

void loop(){

  /*
  for(int i = 0; i < 5; ++i){
    
    registerWrite(font_5x7['A' - 0x20][i]);
    delay(3);
  }
  registerWrite(0);
  delay(10);
  
byte disp[7] = {
  0b00010000,
  0b00111000,
  0b01101100,
  0b11000110,
  0b01101100,
  0b00111000,
  0b00010000
};

  
  for(int i = 0;g i < 7; i++){
    registerWrite(disp[i]);
    delay(1);
  }
  registerWrite(0);
  delay(3);
*/
  displayString("WOW");


}
