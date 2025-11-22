#ifndef COMPONENTHANDLER_H
#define COMPONENTHANDLER_H

#include <Arduino.h>
#include <Preferences.h>
#include <logHandler.h>
#include <types.h>

#define ANALOG_HIGH (255)
#define ANALOG_LOW (0)

void componentsSetup();
void setLedBuiltInStatus (uint8_t value);
void blinkLedBuiltIn(int ripetitions);

#endif