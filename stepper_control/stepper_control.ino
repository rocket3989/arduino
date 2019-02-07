#include <SPI.h>
int latchpin = 4;

#define MOTOR_IDLE 0
#define MOTOR_HOLD 1
#define MOTOR_STEP 2
#define MOTOR_MOVE 3
#define FORWARD 1
#define BACKWARD -1
#define CLOCKWISE 1
#define ANTICLOCKWISE -1

class Motor
{
    int state, step_count, step_target, timer, timer_interval, dir, address;
    int steps[8] = {
      B1000,
      B1100,
      B0100,
      B0110,
      B0010,
      B0011,
      B0001,
      B1001
    };

  public:

    Motor(int addr, long timing) {
      address = addr;
      timer_interval = timing;
      dir = FORWARD;
      state = MOTOR_IDLE;
      step_count = 10000;
    }

    void regOut(int output) {
      digitalWrite(latchpin, LOW);
      SPI.transfer((address << 4) + output);
      digitalWrite(latchpin, HIGH);
    }
    void Step(int direc, int steps) {
      step_target = (steps * direc) + step_count;
      dir = direc;
      state = MOTOR_STEP;
      timer = 0;
    }
    void Move(int direc) {
      dir = direc;
      state = MOTOR_MOVE;
      timer = 0;
    }
    void Idle() {
      state = MOTOR_IDLE;
    }
    void Hold() {
      state = MOTOR_HOLD;
    }
    void SetInterval(int interval) {
      timer_interval = interval;
    }
    void Update() {
      switch (state) {
        case MOTOR_IDLE:
          regOut(B0000);
          break;
        case MOTOR_HOLD:
          break;
        case MOTOR_STEP:
          if (timer++ == timer_interval) {
            step_count += dir;
            timer = 0;
            if (step_target = step_count)
              state = MOTOR_HOLD;
          }
          break;
        case MOTOR_MOVE:
          if (timer++ == timer_interval) {
            step_count = step_count + dir;
            timer = 0;
          }
          break;
      }
      if (state)
        regOut(steps[(step_count + 8) % 8]);
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
