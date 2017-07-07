#include "ascii_table.h"//pulls in the ascii lookup table- made from the arrays you had in your program
int charBreak = 1; //char delay time 
int delayTime = 275; //sub-char delay time 
void setup() 
{ 
  //pinMode(13,INPUT_PULLUP); On my demo, I used a digital trigger- you have a free pin on the attiny and I would recommend 
  //having some way for the attiny to know where the word should start- to frequency match the rotation of the spinner.
  //without it the words are going to be a mess.
  DDRB |= B00011111;    // set pins(in relation to port b on the attiny) 0-4 as outputs
}
void displayString(char* input) //borrowed this from you- awesome way to cast a string to an array.
{ 
  for (int i = 0; i<=strlen(input); i++) //Iterate through the chars in the string
  {  
    for(int j = 0; j<5;j++){ //iterate through the slices of the chars
      PORTB |= ascii_chars[input[i]-TABLE_START][j] + 224;//okay so I dont know what is on the upper three bits of port b on the attiny,
                                                          //so I am trying not to mess with them. Here I pull the ascii value from the string,
                                                          //adjust it for where the ascii array starts, and pull the correspoding slice from
                                                          //the step the program is on. Then B11100000 is added to the lookup and it is 
                                                          //bitwise-or'd with port b. this will leave the upper three bit intact, and turn on any
                                                          //of the lower bits that the ascii table called for.
      delayMicroseconds(delayTime); //let the char slice sit for this long 
      PORTB &= B11100000;//bitwise-and to leave the top three alone and turn off the lower 5 bits
      delayMicroseconds(delayTime); // time char slice is off
      }
    PORTD &= B11100000;//turn off all the lights at the end of a char
    delay(charBreak); // space between each char
  }
} 
void loop() 
{    
  if(!digitalRead(13)){ //this is how I make the letters start at the same place each rotation - I have an ir gate hooked up to a digital pin on the 
                        //nano that drives my spinner. I am working on a change to the program so it adjusts the delay values depending on how fast
                        //it is spinning. should be done in a few days.
    displayString("TINKERNUT"); 
  }
} 

//let me know if you have any questions! send me an email @ rocket3989@gmail.com
//good luck with the project! I love your videos =)
