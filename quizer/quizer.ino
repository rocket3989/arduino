#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
uint16_t input, mask, previn;
char vals[] = {'B','b',0,0,'3','6','9','#','2','5','8','0','1','4','7','*'};
void setup(){
    PORTD = B00001111;
    DDRD = 0;
    lcd.begin(16,2);
    // lcd.print("test");
}
void loop(){
    previn = input;
    input = 0;
    for(byte i = 0; i<4; i++){
        DDRD = 1 << 4+i;
        delay(1);//give time for the pins to set
        input = (input << 4) | (PIND & 15);
    }
    mask = (previn ^ input)&~input;
    
    char charout = 'q';
    if(mask){
      for(byte i = 0; i<16;i++){
        if(mask&(1<<i))
          charout = vals[i];
      }
        
      //Serial.print(charout);
      lcd.clear();lcd.print(charout); 
      //lcd.println(mask,BIN); 
    }
    delay(1);
}
