#include <avr/io.h>
#define MAX_ROLL 20
#define MAX_DELAY 250
bool pressed = false;
byte rolls = 0;
byte current_delay = 0;
byte newroll = 0;
byte states[6]=
{
    B0100,
    B1000,
    B0101,
    B1001,
    B1101,
    B1011
};
void setup(){
    delay(1000);
    DDRB = B00001111;
    PORTB = B00010000;
}
void loop(){
    pressed = !(PINB & B00010000);
    if (pressed)
        rolls = MAX_ROLL;  
    if(rolls){
      if(current_delay++ >= (MAX_DELAY - (rolls*rolls)/2)){
          rolls--;
          current_delay = 0;
          while(newroll==PORTB-16)  
            newroll = states[random(0,6)];
          PORTB = newroll|16;
      }
    }
    else if(pressed)
        current_delay = 0;
    delay(1);
}
