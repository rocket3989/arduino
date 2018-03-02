#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
uint16_t input, mask, previn;
volatile int counter;
byte cursorpos;
char vals[] = {'r','B', 'b', '3', '6', '9', '#', '2', '5', '8', '0', '1', '4', '7', '*'};
void setup() {
  PORTC = 0xFF;
  DDRC = 0;
  lcd.begin(16, 2);
  //lcd.blink();
  attachInterrupt(1,encoder,RISING);
}
void encoder(){
  if(PIND&1<<2)
    counter--;
  else
    counter++;
  counter = ((26+counter)%26);
  
  
}
void loop() {
  delay(5);
  previn = input;
  input = 0;
  for (byte i = 0; i < 4; i++) {
    DDRD &= 0x0F;
    DDRD |= 1 << 4 + i;
    delayMicroseconds(3);//give time for the pins to set
    input = (input << 4) | (PINC & 0xF);
  }
  input = ~input | (PINC >> 4);
  mask = (previn ^ input) & (input);

  char charout;
  if (mask) {
    for (byte i = 0; i < 15; i++) {
      if (mask & (1 << i + 1))
        charout = vals[i];
    }
  }
  if (charout) {
    if(charout != 'b'){
      lcd.print(charout);
      cursorpos++;
    }
    cursorpos++;
    if (charout == '#'){
      lcd.clear();
      cursorpos = 0;
    }
  }
  else{
    lcd.setCursor(cursorpos,0);
    lcd.write(counter+0x41);
  }
}
