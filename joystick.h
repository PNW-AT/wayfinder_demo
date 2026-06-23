const int JOY_BACK=120;
const int JOY_FORW=50;
const int JOY_L=60;
const int JOY_R=130;

#include <Servo.h>

Servo jsPitch;
Servo jsRoll;

void joystickSetup() {
  jsPitch.attach(2);
  jsRoll.attach(3);
}

void joystick(float throttle, float yaw) {
  //forward 50
  //backward 120
  //left is 60
  //right is 130
  int pitchVal = map(throttle * 1000, -1000, 1000, JOY_BACK, JOY_FORW);
  int rollVal = map(yaw * 1000, -1000, 1000, JOY_L, JOY_R);
  Serial.print(rollVal);
  Serial.print(",");
  Serial.println(pitchVal);
  jsPitch.write(pitchVal);
  jsRoll.write(rollVal);
}
void joystickDelay(float throttle, float yaw, int del) {
  joystick(throttle, yaw);
  delay(del);
}
void joystickDelayRecenterDelay(float throttle, float yaw, int del1, int del2) {
  joystick(throttle, yaw);
  delay(del1);
  joystick(0, 0);
  delay(del2);
}

/**
   animate the joystick
*/
void joystickFlightControlsCheck() {
  joystick(0, 0);
  joystickDelayRecenterDelay(1, 0, 150, 100);
  joystickDelayRecenterDelay(1, 0, 150, 100);
  joystickDelayRecenterDelay(-1, 0, 150, 100);
  joystickDelayRecenterDelay(-1, 0, 150, 100);
  joystickDelayRecenterDelay(0, -1, 150, 100);
  joystickDelayRecenterDelay(0, 1, 150, 100);
  joystickDelayRecenterDelay(0, -1, 150, 100);
  joystickDelayRecenterDelay(0, 1, 150, 100);
  for (int i = 0; i < 1000; i++) {
    joystick(cos(i * 6.283 / 1000), sin(i * 6.283 / 1000));
    delay(2);
  }
  joystick(0, 0);
  delay(100);
}
