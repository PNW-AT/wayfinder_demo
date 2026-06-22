#include <Servo.h>

Servo leftESC;
Servo rightESC;

const uint8_t leftESCPin = 4;
const uint8_t rightESCPin = 5;

float speed; //-1 to 1, positive is forwards
float rotation; //-1 to 1, positive is clockwise

// from https://github.com/gobabygocarswithjoysticks/car-code/blob/e2ecab3b4b52e4c09a45647078ed4a7925474c09/gbg_program/_Other_Functions.ino#L6
float calculateTimeInterval() {
  static unsigned long lastMicros = micros();
  if (lastMicros != 0) {
    unsigned long tempMicros = micros();
    float intervalTemp = ((unsigned long)(tempMicros - lastMicros)) / 1000000.0;
    lastMicros = micros();
    return intervalTemp;
  } else {  // lastMicros==0;
    lastMicros = micros();
    return 0.0;
  }
}
// acceleration and deceleration functions from https://github.com/gobabygocarswithjoysticks/car-code/blob/e2ecab3b4b52e4c09a45647078ed4a7925474c09/gbg_program/_Input_Processors.ino#L22-L65
float InputProcessor_LimitAccelerationFourSettings(float velocity, float velocityTarget, float scaler, float ACCELERATION_FORWARD, float DECELERATION_FORWARD, float ACCELERATION_BACKWARD, float DECELERATION_BACKWARD, float timeInterval) {
  ACCELERATION_FORWARD = max(ACCELERATION_FORWARD, (float)0.0) * scaler;
  DECELERATION_FORWARD = max(DECELERATION_FORWARD, (float)0.0) * scaler;
  ACCELERATION_BACKWARD = max(ACCELERATION_BACKWARD, (float)0.0) * scaler;
  DECELERATION_BACKWARD = max(DECELERATION_BACKWARD, (float)0.0) * scaler;
  if (velocity == 0) {
    velocity += constrain(velocityTarget - velocity, -ACCELERATION_BACKWARD * timeInterval, ACCELERATION_FORWARD * timeInterval);
  } else if (velocity > 0) {
    velocity += constrain(velocityTarget - velocity, -DECELERATION_FORWARD * timeInterval, ACCELERATION_FORWARD * timeInterval);
    if (velocity < 0) {  // prevent decel from crossing zero and causing accel
      velocity = 0;
    }
  } else {  //velocity < 0
    velocity += constrain(velocityTarget - velocity, -ACCELERATION_BACKWARD * timeInterval, DECELERATION_BACKWARD * timeInterval);
    if (velocity > 0) {  // prevent decel from crossing zero and causing accel
      velocity = 0;
    }
  }
  return velocity;
}
float InputProcessor_LimitAccelerationTwoSettings(float velocity, float velocityTarget, float scaler, float ACCELERATION, float DECELERATION, float timeInterval) {
  ACCELERATION = max(ACCELERATION, (float)0.0) * scaler;
  DECELERATION = max(DECELERATION, (float)0.0) * scaler;
  if (velocity == 0) {
    velocity += constrain(velocityTarget - velocity, -ACCELERATION * timeInterval, ACCELERATION * timeInterval);
  } else if (velocity > 0) {
    velocity += constrain(velocityTarget - velocity, -DECELERATION * timeInterval, ACCELERATION * timeInterval);
    if (velocity < 0) {  // prevent decel from crossing zero and causing accel
      velocity = 0;
    }
  } else {  //velocity < 0
    velocity += constrain(velocityTarget - velocity, -ACCELERATION * timeInterval, DECELERATION * timeInterval);
    if (velocity > 0) {  // prevent decel from crossing zero and causing accel
      velocity = 0;
    }
  }
  return velocity;
}

float InputProcessor_LimitAccelerationOneSetting(float velocity, float velocityTarget, float scaler, float CELERATION, float timeInterval) {
  CELERATION = max(CELERATION, (float)0.0) * scaler;
  velocity += constrain(velocityTarget - velocity, -CELERATION * timeInterval, CELERATION * timeInterval);
  return velocity;
}

void driveSetup() {
  leftESC.attach(leftESCPin);
  rightESC.attach(rightESCPin);
  leftESC.write(90);
  rightESC.write(90);
  speed = 0;
  rotation = 0;
  delay(5000);
}

void drive(float throttle, float yaw, float speedAccel, float speedDecel, float turnAccel, float turnDecel, float deadzone) {
  if(!leftESC.attached() || !rightESC.attached()) {
    leftESC.attach(leftESCPin);
    rightESC.attach(rightESCPin);
  }


  float timeInterval = calculateTimeInterval();

  throttle=constrain(throttle,-1,1);
  yaw=constrain(yaw,-1,1);

  deadzone=constrain(deadzone,0,1);
  if(abs(throttle)<deadzone){
    throttle=0;
  }
  if(abs(yaw)<deadzone){
    yaw=0;
  }

  speed=InputProcessor_LimitAccelerationTwoSettings(speed, throttle, 1.0, speedAccel, speedDecel, timeInterval);
  rotation=InputProcessor_LimitAccelerationTwoSettings(rotation, yaw, 1.0, turnAccel, turnDecel, timeInterval);

  speed=constrain(speed,-1,1);
  rotation=constrain(rotation,-1,1);

  int leftSpeed = (speed + rotation) * 500 + 1500;
  int rightSpeed = (speed - rotation) * 500 + 1500;

  leftESC.writeMicroseconds(leftSpeed);
  rightESC.writeMicroseconds(rightSpeed);
  Serial.print(leftSpeed);
  Serial.print(", ");
  Serial.println(rightSpeed);
}

void park() {
  calculateTimeInterval();
  speed = 0;
  rotation = 0;
  leftESC.write(90);
  rightESC.write(90);
}
