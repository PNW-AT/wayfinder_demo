// typedef struct pin_channel_t{
//   int receiverChannel;
//   int inputPin;
//   volatile uint16_t pulseWidth;
//   volatile uint32_t lastSignalTime;
//   volatile uint32_t riseTime; //DO NOT TOUCH
// };

// pin_channel_t inputs[] = { //DO NOT REMOVE ZEROS
//   {1, A8, 0, 0, 0},
//   {2, A9, 0, 0, 0},
//   {3, A10, 0, 0, 0},
//   {4, A11, 0, 0, 0},
//   {5, A12, 0, 0, 0},
//   {6, A13, 0, 0, 0}
// };

// const uint8_t numInputs = sizeof(inputs)/sizeof(inputs[0]);

// for(int i = 0; i < numInputs; i++){
//   int channel = inputs[i].receiverChannel;
//   int pin = inputs[i].inputPin;
//   Serial.print(channel);
//   Serial.print(", ");
//   Serial.println(pin);
//   attachPulseInput(pin, signal, lastSignalTime, channel);
// }
