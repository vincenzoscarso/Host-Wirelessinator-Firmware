#ifndef COMPONENTHANDLER_H
#define COMPONENTHANDLER_H

#include <Arduino.h>
#include <Host.h>
#include <logHandler.h>
#include <types.h>

#define ANALOG_HIGH (255)
#define ANALOG_LOW (0)

namespace componentHandler {
const int BLINK_RIPETITIONS_ON_COMMAND = 1;
const int BLINK_RIPETITIONS_ON_SERVER_STARTUP_FAILURE = 1;
const int BLINK_RIPETITIONS_BETWEEN_FAILED_CONNECTION_TRIALS = 3;

void componentsSetup();
void setLedBuiltInStatus(uint8_t value);
void blinkLedBuiltIn(int ripetitions);
void setHostRelayPinStatus(Host host, byte value);
} // namespace componentHandler

#endif
