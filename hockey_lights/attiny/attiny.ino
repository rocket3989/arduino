#include <SoftwareSerial.h>

#define RX    1
#define TX    0

int average = 0;                     
SoftwareSerial Serial(RX, TX);

void passThrough(){
    if (Serial.available()) {
        byte pass_byte = Serial.read() + 1;
        Serial.write(pass_byte);
    }
}

void setup() {
    Serial.begin(9600);
    pinMode(A3, INPUT);
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
    long sum = 0;
    for (int i = 0; i < 128; i++) {
        sum += analogRead(A3);
        delay(5);
    }
    average = sum >> 7;
    //Serial.print("ave: ");
    //Serial.println(average);
}

void loop() {

    delay(10);
    int check = 0;
    while(abs(analogRead(A3) - average) > 20) {
        if(check++ == 1)
            Serial.write(0x30);
    
        passThrough();
        delay(20);
    }
    
    passThrough();
}
