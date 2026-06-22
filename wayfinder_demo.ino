// by Palmer and Joshua for PNW-AT June 2026
// for demonstration models of Wayfinders(tm)
// this program reads a RC receiver, controls a left and right ESC with acceleration, and displays the RC input with two servos
#include "inputs.h"
#include "joystick.h"
#include "drivetrain.h"

float throttle = 0;
float yaw = 0;
float speedAccel = 0.5;
float speedDecel = 2;
float turnAccel = 0.75;
float turnDecel = 1.5;

float deadzone=0.1;

void setup() {
  Serial.begin(115200);
  driveSetup();
  attachInputs();
  joystickSetup();
}

void loop() {
  throttle = -getInput(ch2);
  yaw = -getInput(ch1);
  
  if(active()) {
    drive(throttle, yaw, speedAccel, speedDecel, turnAccel, turnDecel, deadzone);
    joystick(throttle, yaw);
  } else {
    park();
    joystick(0, 0);
  }
}

bool active() {
  if(connected(ch1) && connected(ch2) && connected(ch3)/*&& connected(ch4) && connected(ch5) && connected(ch6)*/ && signal[ch3] > 1100){
    return true;
  } else {
    return false;
  }
}
