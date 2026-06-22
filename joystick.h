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
