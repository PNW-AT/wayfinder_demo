// from https://github.com/gobabygocarswithjoysticks/car-code

/**
  because Arduino only gives integer versions of the map() function
*/
float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
  like map() but has a center position, and can have different scaling in each direction
  deadzone value is the value returned when x == inMin (since neg_out_min and pos_out_min don't have to be equal)
*/
float twoMap(float x, float neg_in_max, float inMin, float pos_in_max, float neg_out_max, float neg_out_min, float pos_out_min, float pos_out_max, float deadzoneValue) {
  if (x == inMin) {
    return deadzoneValue;
  } else if (x > inMin) {
    return floatMap(x, inMin, pos_in_max, pos_out_min, pos_out_max);
  } else {  // (x < inMin)
    return floatMap(x, inMin, neg_in_max, neg_out_min, neg_out_max);
  }
}

/**
  Given a speed and turning value, this function edits the values to be sent to the motors.
  This function scales the input for the motor controllers if it is given a value for what makes the motor controller stop, when it starts moving, and when it moves at its max speed.
*/
void DriveController_TwoSideDrive(float turnToDrive, float speedToDrive, int& leftMotorWrite, int& rightMotorWrite,
                                  int LEFT_MOTOR_CENTER, int LEFT_MOTOR_SLOW, int LEFT_MOTOR_FAST,
                                  int RIGHT_MOTOR_CENTER, int RIGHT_MOTOR_SLOW, int RIGHT_MOTOR_FAST) {
  leftMotorWrite = twoMap(turnToDrive + speedToDrive, -1, 0, 1, LEFT_MOTOR_CENTER - LEFT_MOTOR_FAST, LEFT_MOTOR_CENTER - LEFT_MOTOR_SLOW, LEFT_MOTOR_CENTER + LEFT_MOTOR_SLOW, LEFT_MOTOR_CENTER + LEFT_MOTOR_FAST, LEFT_MOTOR_CENTER);
  leftMotorWrite = constrain(leftMotorWrite, LEFT_MOTOR_CENTER - abs(LEFT_MOTOR_FAST), LEFT_MOTOR_CENTER + abs(LEFT_MOTOR_FAST));

  rightMotorWrite = twoMap(-turnToDrive + speedToDrive, -1, 0, 1, RIGHT_MOTOR_CENTER - RIGHT_MOTOR_FAST, RIGHT_MOTOR_CENTER - RIGHT_MOTOR_SLOW, RIGHT_MOTOR_CENTER + RIGHT_MOTOR_SLOW, RIGHT_MOTOR_CENTER + RIGHT_MOTOR_FAST, RIGHT_MOTOR_CENTER);
  rightMotorWrite = constrain(rightMotorWrite, RIGHT_MOTOR_CENTER - abs(RIGHT_MOTOR_FAST), RIGHT_MOTOR_CENTER + abs(RIGHT_MOTOR_FAST));
}

float InputReader_JoystickAxis(float reading, float neg, float cent, float pos, float deadzone) {
  deadzone = max(deadzone, 0);
  ////////// scale joystick values from adjustible range to -1 to 1
  float output = 0;
  if (neg < pos) {  //if the value for when the joystick is towards the left is less then the value for when the joystick is towards the right...
    if (reading < cent - deadzone && neg < cent - deadzone)
      output = floatMap(reading, neg, cent - deadzone, -1, 0);
    if (reading > cent + deadzone && pos > cent + deadzone)
      output = floatMap(reading, pos, cent + deadzone, 1, 0);
  }
  if (neg > pos) {  // joystick axis is backwards
    if (reading > cent + deadzone && neg > cent + deadzone)
      output = floatMap(reading, neg, cent + deadzone, -1, 0);
    if (reading < cent - deadzone && pos < cent - deadzone)
      output = floatMap(reading, pos, cent - deadzone, 1, 0);
  }
  return constrain(output, -1, 1);
}

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
