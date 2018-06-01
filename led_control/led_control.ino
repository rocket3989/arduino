
void setup() {
  DDRD = 0xFF;
  PORTD = 0xFF;

  DDRB = 0xFF;
  PORTB = 0;

  DDRC |=  B00011111;
  PORTC = B00011111;

}
void loop() {
  for (int i = 0; i < 5; i++) { 
    PORTC = (1 <<i);
    for (int j = 0; j < 5; j++) {
      PORTB = (1 <<j);
      delay(100);
    }
    // loop through the layers in the cube
  }
  // show each 'state' of the cube 20 times
}
// loop through the 'states' of the cube in the animations file

