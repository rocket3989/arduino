#include "animations.h"
/*  This is telling the program to use the file 'animations' which is in the same folder as this program. 
    That is where the data for each state of the cube is held
    
    The data is stored in hexadecimal numbers that represent the binary states of the leds in the cube.
    Lets figure out what that means.
    
    First, how the LEDs are organized on each layer

    0----3----6
    |    |    |
    |    |    |
    1----4----7
    |    |    |
    |    |    |
    2----5----8

    the layers are stacked vertically, starting at the bottom. 
    So to turn on an led in the top layer, in the upper right corner, column 6 and layer 2 should be used.

    For each state of the cube, there are 27 leds that have to be controlled, so we need 27 bits of information for each state. 
    I chose to store each state in an 'unsigned long' which has 32 bits of information stored in it.
    
    The information for each led is stored sequentially inside of the long, in binary.
    The location each led's state is stored in the binary figure is determined by both the positional numbers shown above,
    and by which layer it is in.
    
    So the state of an led in position 6 on the bottom layer (layer 0) would be stored in the 6th bit,
    or the state of led 5 on the top layer (layer 2) would be stored in the 23rd(5 + 2*9) bit 
    For example, to turn on the left side of the bottom layer, and the rest of the bottom unlit, the binary info would look like this:

    1 1 1 0 0 0 0 0 0 X X X X X X....

    with the ones and zeros representing the first layer and the 'X's representing where the info for the second layer starts.

    so if instead the pattern was an "X" shape through all the layers, like this:

    1----0----1
    |    |    |
    |    |    |
    0----1----0
    |    |    |
    |    |    |
    1----0----1

    where the '1's mean the light is on, and the '0's mean the light is off, the info would be written out like this in binary:

    1 0 1 0 1 0 1 0 1 1 0 1 0 1 0 1 0 1 1 0 1 0 1 0 1 0 1
    \    layer 1    / \    layer 2    / \    layer 3    /
    
    This form of holding the data is not very clean, easy to read, or compact. 
    Instead, the data is then converted to a more concise form: hexadecimal.

    Hexadecimal means base 16, so there are 16 unique characters in this counting scheme, 1-9 followed by a-f
    Here is a table for converting the numbers:

       BIN  |  DEC | HEX  
      ------+------+-----
       0000 |   00 |  00
       0001 |   01 |  01
       0010 |   02 |  02
       0011 |   03 |  03
       0100 |   04 |  04
       0101 |   05 |  05
       0110 |   06 |  06
       0111 |   07 |  07
       1000 |   08 |  08
       1001 |   09 |  09
       1010 |   10 |  0a
       1011 |   11 |  0b
       1100 |   12 |  0c
       1101 |   13 |  0d
       1110 |   14 |  0e
       1111 |   15 |  0f
      10000 |   16 |  10
      10001 |   17 |  11


      There is a nice trick when converting from binary to hex, you can look at groups of 4 binary bits
      inside of a larger number and convert those to hex individually, then combine them into the full hex-converted number

      The decimal number 87 is 1010111 in binary. 
      To convert it to hex, first group the binary bits into groups of 4, 
      starting at the LSB (least significant bit, or the ones place). Then convert those groups into hexadecimal:

          1 0 1 0 1 1 1
          \ 5 / \  7  / 
          
      So 87 in decimal is 57 in hexadecimal.
      But that doesn't look so different from the 87- how does the compiler know which is which?
      Adding a flag in front of every number not written in decimal makes it clear for the program.
      the hexadecimal flag is "0x" so writing the number as 0x57 tells the computer it should read '57' in hex.

      Now, to store the binary data for the leds.
      The way the data is being stored, a long, has more bits to fill than we need. 
      So those empty spots have to go either at the front or back of the number
      
      For consistency and ease of reading, the data bits will start at the front of the number, and the empty bits at the end.
      So before the X pattern is stored, those extra 5 bits are added to the end of our data to make it an even 32 bits:

      1 0 1 0 1 0 1 0 1 1 0 1 0 1 0 1 0 1 1 0 1 0 1 0 1 0 1 0 0 0 0 0
      \                    Data                           / \ extra /

      now it can be converted to hexadecimal:

      1 0 1 0 1 0 1 0 1 1 0 1 0 1 0 1 0 1 1 0 1 0 1 0 1 0 1 0 0 0 0 0
      \  A  / \  A  / \  D  / \  5  / \  6  / \  A  / \  A  / \  0  / 

      So now the number is AAD56AA0, which the flag is then added to:

      0xAAD56AA0

      And the data is ready to go!
     
      These Hexadecimal values are then stored in an 'array' which is held in the animation file as a comma-delimited list,
      meaning that you list the hex values on after another with commas in between like this:
      
      unsigned long animation[] = {0xAAD56AA0,0xAAAAAAAA,0x55555555,0xFF800000,0x007FC000,0x00003FFF};
      
      For readability, I prefer to put each state of the animation on its own line as such:
      
      unsigned long animation[] = {
          0xAAD56AA0,
          0xAAAAAAAA,
          0x55555555,
          0xFF800000,
          0x007FC000,
          0x00003FFF
      };
      
      These two methods of writing out an array are interchangeable. 
      
      Now that the data has been prepared for the program, it must be able to interpret the data,
      and control the leds such that they recreate the pattern visibly for a person.
*/
 

int layers[3] = {14, 15, 16};
int columns[9] = {0,1,2,3,4,5,6,7,8};

/*  'layers' are the ports connected to the side of the cube. 
    Each port connects to all of the negative side of the leds on each layer.
    
    //ADDED NOTE: This should not be done. This is probably getting fairly close to the max sink current of the nano
    //the layers should be sunk to ground through transistors

    'columns' are the ports connected to the underside of the leds. 
    Each port connects to 3 leds vertically through their positive side. 
    These ports first pass through a 330 ohm current limiting resistor.
*/


const int num_layers = 3;
const int num_columns = 9;

//   did not end up using these variables in the program





void setup() {

//to start, all ports are given a pinmode and initialized

  for (int i : columns) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  for (int i : layers) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  
/*  In order to have all leds off, the columns are turned low,
    and the layers are turned high.
    The leds are configured like this:

    Column pin    Resistor    LED     Layer pin

        O---------/\/\/\------>|----------O

    So when the column is low and the layer is high, no current will flow,
    and all of the leds will be turned off.
*/


}
void loop() {
//    The code is pretty messy- sorry! I'll fix it up for v2


/*  if you aren't familiar with this form of for loop:
   
      for (item:array){
          code;
          }

    it is functionally identical to 
      for(int index = 0 ; index < sizeOf(array); index++){
          item = array[index];
          code;
          }
          
    this form of for loop iterates through the array you give it, 
    storing each value in the array to the 'item' variable you give it
    for one pass of the loop. This is a very handy shortcut!
 
 */

  for (unsigned long state : animation) {
    
    //This first for loop iterates through the states that are stored in the animation file, and holds it in the 'state' variable

    for (int i = 0; i < 20; i++) {
      /*  okay this is lazy, guess and check programing. 
          This is the number of times that each state will be displayed
          before moving onto the next frame. The number 20 is arbitrary.
      */
      for (int layer : layers) {
        //Time to start drawing the cube! this loop iterates through the layers of the cube, starting at the bottom of the cube
        
        digitalWrite(layer, HIGH);

        //Turn the layer low, this makes the layer ready to have leds turn on for it
        //Now, if any of the columns are turned high, the intersection of the two will be powered on

        
        for (int column : columns) {
          //This loop iterates through the led columns 

          
          digitalWrite(column, ((state << (9 * (layer - 14) + column)) & 0x80000000) && 1);
         
          /*  Uh oh. this looks scary. How are we going to break this down

              Starting in the middle of the statement, lets check this out:
              
                    (9 * (layer - 2) + column - 5)
                    
              This was poorly done and should be changed. Because I used the array for loops, 
              I did not have access to the index value of the elements, so I had to create it. 
              Luckily, I had my columns and layers in order in the array, so I just did some subtraction. 
              I should have just used a non-array for loop and called the index here. 
              The constant 9 should also be replaced with num_columns - the variable I didn't use
              let's look a the code now:
              
                    (num_columns * (layer) + column)

              Thats better! What this does is take the layer and column given, and turn it into an index for an led in the cube, 0-26.
              Now we can replace that with "led" in our code:
              
                    ((state << led) & 0x80000000) && 1
                    
              So the 'led' in the code is representing the index of the led at the intersection of the current layer and column.
              Now the program has to figure out whether or not that led should be lit.
              
              First, the program is going to perform an operation called 'bit shifting', represented by the << operator.
              This operator takes the number on the left side of the operator and 'shifts' its binary bits
              however many times the number on the right of the operator says to, in the direction the arrows indicate 
              this is the operator demonstrated on a 1 byte number (meaning it takes 8 binary bits in memory):
              
              1 1 0 1 1 1 0 1  << 2    // the number will be binarily shifted 2 times to the left,
                 / / / / / /           // dumping the bits that go beyond the size of the number in memory
                / / / / / /
               / / / / / / 
              0 1 1 1 0 1 0 0         // the result of the bit shift will also have trailing or leading zeros added,
                                      // depending on the direction of the shift

              The total size of the number in memory stays constant, only the position of the bits change.
              
              So now looking at the the part of the line:
              
                    state << led 
                    
              State represents one entry in the 'animations' header file that was pulled in the first for loop of the program.
              This operation will bit shift the number that describes the desired state of the cube by the index of the led.
              
              So if the data is 'X' shape described before, again showed here:
              
              1 0 1 0 1 0 1 0 1 1 0 1 0 1 0 1 0 1 1 0 1 0 1 0 1 0 1 0 0 0 0 0
              
              And the program is at layer 2, column 5, or led 23 (2*9 + 5)
              Then the 23rd bit (which describes led 23) will be at the front of the number after performing the operation:
              
              0 1 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
              ^
              bit 23
              
              Now the program needs to strip away all the superfluous bits and just check the leading bit if it is on or off.
              To accomplish this, the following operation is done
              
              (shifted_data & 0x80000000) && 1
              
              The single ampersand (&) is the 'bitwise and' operation. The operation goes bit by bit through the two numbers given,
              and if both bits are a '1', then it will put a '1' in that position in the output number.
              If either or both of the bits are '0', then instead a '0' will be stored in that position of the output number.
              Here is a demonstration with some 2-bit numbers:
              
                0 0         0 1         1 0         1 1
              & 1 1       & 0 1       & 0 0       & 1 0
                0 0         0 1         0 0         1 0 
                
              So looking again at the line using our example input again:
              
              (shifted_data & 0x80000000) // this is equivalent to:
              
                0 1 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
              & 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
                0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
              
              Or in a more general sense, here is the same operation, but with any input:
             
                X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
              & 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
                X 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
              
              So every bit in the input data is turned to a zero- except for the bit at the front, 
              Which was the bit selected with the led index found earlier!             
              In the case of the example, that bit was a '0', so all the bits in the number are now 0.
              
              The final step is to turn that into boolean logic so the column can be turned HIGH or LOW to match the chosen bit.
              There are many ways to perform this, the method used here is to take the logical AND operation (&&), 
              using the output from the previous operation and a '1' as arguments:
              
              bitwise_output && 1 
                
              This will return FALSE if the bitwise_output had a leading bit of zero (and therefore all bits are zero), 
              and it will return TRUE if the output had a leading 1.
              
              So now looking at the whole formula again:
              
              ((state << (9 * (layer - 2) + column - 5)) & 0x80000000) && 1
              
              This statement will determine if the led at the intersection of the given layer and column, 
              in the given state of the cube, should be on or off.
              
              The program then modifies the current column to match with the given data:
              
              digitalWrite(column,formula);
              
              */   
        }
        // At this point, all of the columns have been activated for the current layer  
          
        delay(4); 
        // now the program lets the current state of columns and layer stay lit 
        // This could be omitted and the cube would almost be the same, but the program would spend more time, on average,
        // with the lights off and it would look dimmer. The number 4 is again, arbitrary.
          
        digitalWrite(layer, LOW);
        // To prepare for the next layer being controlled, first the current layer must be turned off
          
        for (int column : columns) {
          digitalWrite(column, LOW);
            // turn off all of the columns before controlling the next layer
        }
      }
        // loop through the layers in the cube
    }
      // show each 'state' of the cube 20 times
  }
    // loop through the 'states' of the cube in the animations file
}
// main loop!
