#include "PulseInput.h"

/*
  channel 1: R stick LR
  channel 2: R stick UD
  channel 3: L stick UD (throttle), switches set to 1000
  channel 4: L stick LR
  channel 5: R knob
  channel 6: L knob
*/

const uint8_t numInputs = 3;
const uint8_t ch1 = 0;
const uint8_t ch2 = 1;
const uint8_t ch3 = 2;
//const uint8_t ch4 = 3;
//const uint8_t ch5 = 4;
//const uint8_t ch6 = 5;

const uint8_t ch1Pin = A8;
const uint8_t ch2Pin = A9;
const uint8_t ch3Pin = A10;
//const uint8_t ch4Pin=A11;
//const uint8_t ch5Pin=A12;
//const uint8_t ch6Pin=A13;

volatile unsigned int signal[numInputs];
volatile unsigned long lastSignalTime[numInputs];
const unsigned long maxSignalDelay = 50;

void attachInputs() {
  attachPulseInput(ch1Pin, signal, lastSignalTime, ch1);
  attachPulseInput(ch2Pin, signal, lastSignalTime, ch2);
  attachPulseInput(ch3Pin, signal, lastSignalTime, ch3);
  // attachPulseInput(ch4Pin, signal, lastSignalTime, ch4);
  // attachPulseInput(ch5Pin, signal, lastSignalTime, ch5);
  // attachPulseInput(ch6Pin, signal, lastSignalTime, ch6);
}

bool connected(uint8_t channel, unsigned long maxSignalDelay = 50) {
  if (millis() - lastSignalTime[channel] < maxSignalDelay) {
    return true;
  } else {
    return false;
  }
}

float getInput(uint8_t channel) {
  return constrain(((int32_t) signal[channel] - 1500) / 400.0, -1, 1);
}

// void printInputs(){
// Serial.print(signal[0]);  //for debugging
// Serial.print(", ");
// Serial.print(signal[1]);
// Serial.print(", ");
// Serial.print(signal[2]);
// Serial.print(", ");
// Serial.print(signal[3]);
// Serial.print(", ");
// Serial.print(signal[4]);
// Serial.print(", ");
// Serial.print(signal[5]);
// Serial.print(", ");

// Serial.println(getInput(ch1));

// if(connected(ch1) && connected(ch2) && connected(ch3) && connected(ch4) && connected(ch5) && connected(ch6) && signal[ch3] > 1100){
//   Serial.println("Connected!");
// } else{
//   Serial.println("Disconnected!");
// }
// }
