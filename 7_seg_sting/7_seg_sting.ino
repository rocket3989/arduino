/*
  Shift Register Example
 for 74HC595 shift register

 This sketch turns reads serial input and uses it to set the pins
 of a 74HC595 shift register.

 Hardware:
 * 74HC595 shift register attached to pins 8, 12, and 11 of the Arduino,
 as detailed below.
 * LEDs attached to each of the outputs of the shift register.


 Created 22 May 2009
 Created 23 Mar 2010
 by Tom Igoe

 */
#include <SPI.h>

//Pin connected to latch pin (ST_CP) of 74HC595
const int latchPin = 8;
//Pin connected to clock pin (SH_CP) of 74HC595
const int clockPin = 13;
////Pin connected to Data in (DS) of 74HC595
const int dataPin = 11;
const byte nums[10] = {
  0x3F,
  0x06,
  0x5B,
  0x4F,
  0x66,
  0x6D,
  0x7D,
  0x07,
  0x7F,
  0x6F
};
void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  SPI.begin();
}

void loop() {
  
  for(int i = 0; i<10;i++){
    for(int j = 0; j<10;j++){
      for(int k = 0; k<10;k++){
        for(int l = 0; l<10;l++){
          registerWrite(nums[l]<<1);
          registerWrite(nums[k]<<1);
          registerWrite(nums[j]<<1); 
          registerWrite(nums[i]<<1);
          delay(1);
        }
      }
    }
  }
}

// This method sends bits to the shift register:

void registerWrite(byte val) {
  SPI.transfer(val);


}
