#include "animations.h"
long shiftRight(long shiftee, int shifter){
  for (int i = 0; i<shifter;i++){
    shiftee = shiftee/2;
  }
  return shiftee;
}
unsigned long portout;
unsigned long state;

void setup() {
  DDRD = 0xFF;
  PORTD = 0xFF;

  DDRB |= 1;
  PORTB = 0xFF;
  
  DDRC |=  B00000111;
  PORTC |= B00000111;

}
void loop() {
  delay(1000);
  for (int state_count = 0;state_count<(sizeof(animation)/4);state_count++ ) {
    state = animation[state_count];
    for (int i = 0; i < 20; i++) {
      for (int layer = 0;layer < 3;layer++){
        PORTC |= (1<<2-layer);
        int shift = layer*9 + 5;
        PORTD = state >> shift;
        PORTB = state >> shift + 8;
        delay(4); 
          
        PORTC &= B11111000;
        PORTD = 0; 
        PORTB = 0;
      }
        // loop through the layers in the cube
    }
      // show each 'state' of the cube 20 times
  }
    // loop through the 'states' of the cube in the animations file
}
