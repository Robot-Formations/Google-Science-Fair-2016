#include <Wire.h>
#include <Zumo32U4.h>
#include <math.h>
Zumo32U4Motors motors;

const int ping1 = 11;
float duration;
int distance;
int turnPolarity = 1;
int previous = 0;
int changes = 0;
void setup() {
  Serial1.begin(9600);
}

void loop() {

  if (Serial1.available())
  {
    char key = (char)Serial1.read();
    if (key == 's') {
      pinMode(ping1, OUTPUT);
      digitalWrite(ping1, LOW);
      delayMicroseconds(2);
      digitalWrite(ping1, HIGH);
      delayMicroseconds(5);
      digitalWrite(ping1, LOW);

      pinMode(ping1, INPUT);
      duration = pulseIn(ping1, HIGH);
      duration /= 29;

      distance = round(duration);
      if (previous - distance < 0) {
        turnPolarity *= -1;
        if (previous != 0)
          changes++;
      }
      delay(50);
      motors.setLeftSpeed((-100 * turnPolartiy) - (changes * 5);
      motors.setRightSpeed((100 * turnPolarity) - (changes * 5));
      //motors.setSpeeds(50,-50);
      delay(70);
      motors.setSpeeds(0, 0);
      delay(50);
    }
  }
  else if (key == 'c')
  {
    if (changes > 5 && abs(previous - distance) < 10)
      Serial1.print('d');
    else
      Serial1.print('g');
  }
}

}
