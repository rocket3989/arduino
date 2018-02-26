#include "animations.h"
int layers[3] = {2, 3, 4};
int columns[9] = {5, 6, 7, 8, 9, 10, 11, 12, 13};
long portout;

void setup() {
  DDRD = 0xFF;
  PORTD = 0xFF;

  DDRB |= 1;
  PORTB &= 0xFE;
  
  DDRC |=  B00000111;
  PORTC |= B00000111;

}
void loop() {
  for (unsigned long state : animation) {
    for (int i = 0; i < 20; i++) {
      for (int layer = 0;layer < 3;layer++){
        PORTC |= (1<<layer);
        int shift = layer*9 + 5;
        
        portout = state << shift;
        PORTD = portout;
        delay(400); 
          
        PORTC &= B11111000;
        PORTD = 0; 
      }
        // loop through the layers in the cube
    }
      // show each 'state' of the cube 20 times
  }
    // loop through the 'states' of the cube in the animations file
}
