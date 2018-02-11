#include <avr/io.h>
#define MAX_ROLL 15
#define MAX_DELAY 100
bool pressed = false;
int rolls = 0;
int current_delay = 0;

byte states[6]=
{
    B0100,
    B0001,
    B0101,
    B1001,
    B1101,
    B1011
}
void setup(){
    delay(3000);
    DDRB |= B00001111;
    DDRB &= B11101111;
    PORTB |= B00010000;
    PORTB &= B11110000;
}
void loop(){
    pressed = (PINB & B00010000) && 1;
    if(rolls){
        if (pressed){
            rolls = MAX_ROLL;
            current_delay = 0;
        }
        if(current_delay++ == MAX_ROLL - 5*rolls){
            rolls--;
            PORTB &= B11110000;
            PORTB |= states[random(0,6)];
        }
    }
    else if(pressed){
        rolls = MAX_ROLL;
        current_delay = 0;
    }
}