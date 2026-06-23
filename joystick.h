#include <Servo.h>

Servo jsPitch;
Servo jsRoll;

void joystickSetup() {
  jsPitch.attach(2);
  jsRoll.attach(3);
}

void joystick(float throttle, float yaw) {
  jsPitch.write(throttle * 90 + 90);
  jsRoll.write(yaw * 90 + 90);
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
