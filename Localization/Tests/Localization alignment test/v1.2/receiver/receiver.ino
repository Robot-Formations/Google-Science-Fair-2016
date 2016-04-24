#include <QueueList.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include <math.h>
Zumo32U4Motors motors;

const int ping1 = 11;
float duration;
int distance;
int averageDistance = 0;
int lastAverage;
int turnPolarity = 1;
float previous = 0;
float current = 0;
float minAv = NULL;
int changes = 0;
int firstFive = 0;
bool done = false;
QueueList <int> averageFive;

void setup() {
  Serial1.begin(9600);
  Serial.begin(9600);
}

void loop() {

  if (Serial1.available())
  {
    char key = (char)Serial1.read();
    if (key == 's' && !done) {
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

      averageDistance += distance;
      averageFive.push(distance);
      firstFive++;
      if (firstFive > 5)
      {
        lastAverage = averageFive.pop();
        averageDistance -= lastAverage;
      }
      previous = (averageDistance - distance + lastAverage) / 5;
      current = (averageDistance) / 5;
      minAv = minAv==NULL && firstFive > 5?current:min(minAv, current);
      Serial.print("distance:");
      Serial.println(distance);
      Serial.print("current:");
      Serial.println(current);
      Serial.print("previous:");
      Serial.println(previous);


      if (firstFive > 5 && previous - current < -1) {
        turnPolarity *= -1;
        if (previous != 0)
          changes++;
      }
      delay(50);
      if (firstFive > 5) {
        motors.setLeftSpeed((-100 * turnPolarity) + (turnPolarity*changes * 2));
        motors.setRightSpeed((100 * turnPolarity) - (turnPolarity * changes * 2));
      }
      delay(70);
      motors.setSpeeds(0, 0);
      delay(50);

      
      if (changes > 2 && abs(minAv - current) < 2){
        done = true;
        Serial.println("DONE DONE DONE DONE DONE DONE DONE");
        motors.setLeftSpeed((-100 * turnPolarity) + (turnPolarity*changes * 5));
        motors.setRightSpeed((100 * turnPolarity) - (turnPolarity * changes * 5));
        delay(70);
        motors.setSpeeds(0, 0);
        delay(50);
      }
    }
    else if (key == 'c')
    {
      ledRed(1);
      ledYellow(1);
      ledGreen(1);

      // Wait for a second.
      delay(250);

      // Turn the LEDs off.
      ledRed(0);
      ledYellow(0);
      ledGreen(0);
      
      Serial.print("changes:");
      Serial.println(changes);
      Serial.print("minAv");
      Serial.println(minAv);
      if (done){
        Serial1.print('d');
      }
      else
        Serial1.print('g');
    }
  }
}
