
//#include <SoftwareSerial.h>
#define RX    0
#define TX    1
#define DELAY 500
byte pass_byte;
int average = 0;
//SoftwareSerial Serial(RX, TX);
void setup() {
    //Serial.begin(9600);
    pinMode(A3, INPUT);
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
    long sum = 0;
    for (int i = 0; i < 128; i++) {
        sum += analogRead(A3);
        delay(5);
    }
    average = sum >> 7;
    //Serial.println(average);
}

void loop() {

    delay(10);
    if (abs(analogRead(A3) - average) > 10) {
        delay(5);
        while (abs(analogRead(A3) - average) > 10) {
            //if (Serial.available()) {
               // pass_byte = Serial.read() + 1;
                //Serial.write(pass_byte);
            //}
            delay(20);
        }
        
    }
    //if (Serial.available()) {
        //pass_byte = Serial.read() + 1;
        //Serial.write(pass_byte);
    //}
}
