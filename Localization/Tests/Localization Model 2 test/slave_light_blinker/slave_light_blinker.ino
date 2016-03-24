#define ROBOT_ID '4'
#include <Wire.h>
#include <Zumo32U4.h>

void setup() {
  Serial1.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial1.available())
  {
    char key = (char)Serial1.read();
    if (key == 'b') {
      ledRed(1);
      ledYellow(1);
      ledGreen(1);

      // Wait for a second.
      delay(250);

      // Turn the LEDs off.
      ledRed(0);
      ledYellow(0);
      ledGreen(0);
    }
    else if (key == 's')
    {
      Serial1.print(ROBOT_ID);
    }
  }
}
