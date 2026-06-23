#include <PinChangeInterrupt.h>
// #include <PulseFilter.h>

#ifndef PulseInput_h
#define PulseInput_h

// Read signal at given pin
template<uint8_t PIN, volatile uint16_t* time_change, volatile uint32_t* time_stamp, uint8_t channel>
void attachPulseInput() {

  // lamda function to update time
  auto interruptFunc = [] (void) {
    static uint32_t time_last = micros();
    if ( digitalRead(PIN) ) {     // rising
      time_last = micros();
    } else {                      // falling
      time_change[channel] = uint16_t(micros() - time_last);
      time_stamp[channel] = millis();
    }
  };

  // set pin interrupt
  time_change[channel] = 0;
  pinMode(PIN, INPUT_PULLUP);
  attachPCINT(digitalPinToPCINT(PIN), interruptFunc, CHANGE);
}

// Stop pin from reading signal
void detachPulseInput(uint8_t PIN) {
  detachPCINT( digitalPinToPCINT(PIN) );
}

// Macro for function-like notation
#define attachPulseInput(pin, input, timestamp, channel)    attachPulseInput<pin, input, timestamp, channel>()

#endif
