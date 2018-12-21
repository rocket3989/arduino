

#define CLOCKWISE 1
#define ANTICLOCKWISE -1

class Motor
{
    int dir, step;

  public:

    Motor(int dir_pin, int step_pin) {
      dir  = dir_pin;
      step = step_pin;
    }

    void Move(double setPos, double currentPos) {
      
    }
};
//Motor motor_test(7,2000,1);
Motor motors[8] = {
  Motor(0, 750),
  Motor(1, 750),
  Motor(2, 1),
  Motor(3, 1),
  Motor(4, 1),
  Motor(5, 70),
  Motor(6, 50),
  Motor(7, 1),
};
void setup() {
  SPI.beginTransaction(SPISettings(20000000, LSBFIRST, SPI_MODE0));
  SPI.begin();
  pinMode(latchpin, OUTPUT);
  pinMode(10, OUTPUT);
  motors[6].Move(FORWARD);
  motors[5].Move(FORWARD);
  
}
unsigned long prevMillis = 0;
void loop() {
  delayMicroseconds(1);
  for (int i = 2; i < 7; i++) {
    motors[i].Update();
    //Serial.println("here1");
  }

}
