#include "gbgdrivefuncs.h"
#include <Servo.h>

Servo leftESC;
Servo rightESC;

const uint8_t leftESCPin = 4;
const uint8_t rightESCPin = 5;

float speed; //-1 to 1, positive is forwards
float rotation; //-1 to 1, positive is clockwise


void driveSetup() {
  leftESC.attach(leftESCPin);
  rightESC.attach(rightESCPin);
  leftESC.write(90);
  rightESC.write(90);
  speed = 0;
  rotation = 0;
}

/**
   call continuously when driving is activated
*/
void drive(float throttle, float yaw, float speedAccel, float speedDecel, float turnAccel, float turnDecel, float deadzone, int LEFT_MOTOR_CENTER, int LEFT_MOTOR_SLOW, int LEFT_MOTOR_FAST, int RIGHT_MOTOR_CENTER, int RIGHT_MOTOR_SLOW, int RIGHT_MOTOR_FAST) {

  float timeInterval = calculateTimeInterval();

  throttle = constrain(throttle, -1, 1);
  yaw = constrain(yaw, -1, 1);

  deadzone = constrain(deadzone, 0, 1);

  // adds deadzone to joystick input, re-scales output to a continuous -1 to 1
  throttle = InputReader_JoystickAxis(throttle, -1, 0, 1, deadzone);
  yaw = InputReader_JoystickAxis(yaw, -1, 0, 1, deadzone);

  // limits celeration of speed and rotation inputs
  speed = InputProcessor_LimitAccelerationTwoSettings(speed, throttle, 1.0, speedAccel, speedDecel, timeInterval);
  rotation = InputProcessor_LimitAccelerationTwoSettings(rotation, yaw, 1.0, turnAccel, turnDecel, timeInterval);

  int leftSpeed = 0;
  int rightSpeed = 0;

  // convert xy to lr, remap range, compensate for ESC deadzones
  DriveController_TwoSideDrive(rotation, speed, leftSpeed, rightSpeed,
                               LEFT_MOTOR_CENTER, LEFT_MOTOR_SLOW, LEFT_MOTOR_FAST,
                               RIGHT_MOTOR_CENTER, RIGHT_MOTOR_SLOW, RIGHT_MOTOR_FAST);

//  Serial.print(leftSpeed);
//  Serial.print(",");
//  Serial.print(rightSpeed);
//  Serial.print("\n");
  leftESC.writeMicroseconds(leftSpeed);
  rightESC.writeMicroseconds(rightSpeed);
}

/**
   call continuously when driving is deactivated
*/
void park() {
  calculateTimeInterval(); // keep this running so the loop interval is accurate when drive starts up again
  // stop the car without deceleration
  leftESC.write(90);
  rightESC.write(90);
  // the car is stopped, so make the slowly celerated variables know that
  speed = 0;
  rotation = 0;
}
