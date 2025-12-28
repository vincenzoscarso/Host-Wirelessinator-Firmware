#ifndef COMPONENTHANDLER_H
#define COMPONENTHANDLER_H

#include <Arduino.h>
#include <logHandler.h>
#include <types.h>
#include <Host.h>

#define ANALOG_HIGH (255)
#define ANALOG_LOW (0)

namespace componentHandler {
void componentsSetup();
void setLedBuiltInStatus(uint8_t value);
void blinkLedBuiltIn(int ripetitions);
void setHostRelayPinStatus(Host host, byte value);
} // namespace componentHandler

#endif
