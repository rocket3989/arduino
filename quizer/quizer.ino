u_int16t input, mask, previn;
setup(){
    DDRC = B11110000;
    Serial.begin(9600);
}
loop(){
    previn = input;
    input = 0;
    for(byte i = 0; i<4; i++){
        PORTC  = 1 << 4+i;
        delay(1);//give time for the pins to set
        input = (input << 4) | (PINC & 15);
    }
    mask = previn ^ input;
    Serial.println(input,BIN);
}
