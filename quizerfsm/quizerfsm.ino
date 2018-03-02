#include <LiquidCrystal.h>
#include <FastLED.h>

#define ROTARY 1
#define NUMPAD 2
#define SELQ 0
#define ANSQROT 1
#define ANSQPAD 2
#define NUM_LEDS 6
#define DATA_PIN 1




class Question{
  String answer = "";
  String solution = "";
  byte num;
  public:
    Question(String answerIn,uint8_t numIn){
      solution = answerIn;
      num = numIn;
    }
    bool isCorrect(){
      return answer.equalsIgnoreCase(solution);
    }
    void addChar(char charIn){
      answer += charIn;
    } 
    char clearLast(){
      char lastChar = answer.charAt(answer.length()-1);
      answer.remove(answer.length()-1);
      return lastChar;
    }
    void clear(){
      answer = "";
    }
    byte getType(){
      return num; 
    }
    String getAns(){
      return answer;
    }
};



Question Questions[6] = {
  Question("first",ROTARY),
  Question("second",ROTARY),
  Question("0123456789",NUMPAD),
  Question("4321",NUMPAD),
  Question("answer",ROTARY),
  Question("something",ROTARY),
};

 


LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
CRGB leds[NUM_LEDS];
char vals[] = {'r','d', 'u', '3', '6', '9', '#', '2', '5', '8', '0', '1', '4', '7', '*'};

uint16_t input, mask, prevIn;
uint8_t state, cursorPos, queSel, currChar, lastChar;
volatile int counter;
char charOut;

void setup() {
  PORTC = 0x1F;
  DDRC = 0;
  lcd.begin(16, 2);
  lcd.print("Select Question");
  attachInterrupt(1,encoder,RISING);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(40);
}

void encoder(){
  if(PIND&1<<2)
    counter--;
  else
    counter++;
}

void loop() {
  delay(5);
  if(millis()<1000){
    state = 0;
  }
  prevIn = input;
  input = 0;
  for (byte i = 0; i < 4; i++) {
    DDRD &= 0x0F;
    DDRD |= 1 << 4 + i;
    delayMicroseconds(3);//give time for the pins to set
    input = (input << 4) | (PINC & 0xF);
  }
  input = ~input | (PINC >> 4);
  mask = (prevIn ^ input) & (input);

  charOut = 0;
  if (mask) {
    for (byte i = 0; i < 15; i++) {
      if (mask & (1 << i + 1))
        charOut = vals[i];
    }
  }
  switch(state){
    case SELQ:
      questionSelect();
      break;
    case ANSQROT:
      answerRotary();
      break;
    case ANSQPAD:
      answerNumpad();
      break;
  }
}

void questionSelect(){
  counter = (6+counter)%6;
  /*
  while(Questions[counter].isCorrect())
    counter++;
  */
  lcd.clear();
  lcd.print("Select Question");
  for(int i; i<6;i++){
    if (Questions[i].isCorrect())
      leds[i] = CRGB::Green;
    else if(i == counter)
      leds[i] = CRGB(sin8(.1*millis()),.75*sin8(.1*millis()),0);
    else
      leds[i] = CRGB::Red;
  }
  if(charOut=='r'||charOut=='d'){
    if(!Questions[counter].isCorrect()){
      leds[counter] = CRGB::Yellow;
      queSel = counter;
      counter = 0;
      state = Questions[queSel].getType();
    }
  }
  FastLED.show();
}



void answerRotary(){
  counter = (26+counter)%26;
  currChar = counter+0x41;
  if(charOut){
    switch(charOut){
      case 'r':
      case 'd':
        Questions[queSel].addChar(currChar);
        counter = 0;
        lastChar = 0;
        break;
      case '*':
        Questions[queSel].clear();
        counter = 0;
        lastChar = 0;
        break;
      case '#':
        counter = Questions[queSel].clearLast() - 0x41;
        lastChar = 0;
        break;
      case 'u':
        state = 0;
        break;
    }
    if(Questions[queSel].isCorrect())
      state = 0;
    if(!state){
      lcd.clear();
      lcd.print("Select Question");
      counter = queSel;
    }
  }
  
  if(lastChar != currChar && state){
    lcd.clear();
    lcd.print(Questions[queSel].getAns());
    lcd.write(currChar);
    lastChar = currChar;
  }
}

void answerNumpad(){
  if(charOut){
    switch(charOut){
      case 'r':
      case 'd':
        //dont press these lmao
        break;
      case '*':
        Questions[queSel].clear();
        break;
      case '#':
        Questions[queSel].clearLast();
        break;
      case 'u':
        state = 0;
        break;
      default:
        Questions[queSel].addChar(charOut);
        lcd.clear();
        lcd.print(Questions[queSel].getAns());
        break;
    }
    if(Questions[queSel].isCorrect())
      state = 0;
    if(!state){
      lcd.clear();
      lcd.print("Select Question");
      counter = queSel;
    }
  }
}