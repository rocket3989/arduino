#include <avr/io.h>
#define DEL_TIME 10
#define SEQ_LEN 10
byte seq[SEQ_LEN];
byte len;
byte prog;
const byte scale[] PROGMEM = {239,226,213,201,190,179,169,160,151,142,134,127};

void note(byte octave,byte n){
    int scaler = 8 - (octave + n/12);
    if(scaler&B11110000||octave == 0)//scaler out of bounds
        scaler = 0;
    OCR1C = pgm_read_byte(&scale[n%12])-1;
    GTCCR = B0001000; //use pin 4
    TCCR1 = B1000000|scaler;
}



void setup(){

}
void loop(){
    if (len++ == 1){//generate a list
        for (byte i = 0; i<SEQ_LEN;i++)
            seq[i] = random(0,4);
    }
    DDRB = B00011111;
    for(byte i = 0; i<len;i++){
        PORTB = 1<<seq[i];
        tone(3,seq[i]<<1);
        delay(DEL_TIME);
    }
    tone(0,0);
    DDRB = B00010000;
    PORTB = B00001111;
    while(prog<len){
        if(PINB & B00001111){
            if(PINB == 1 << seq[prog])
                tone(3,seq[prog++]<<1);//correct note
            else{
                tone(2,9);
                len = 0;
            }
            while(PINB & B00001111){}
            delay(10);
        }
    }
    if(len == 0){
        //you lost fool

    }
    else{
        //you won
        len = 0;
    }


}
