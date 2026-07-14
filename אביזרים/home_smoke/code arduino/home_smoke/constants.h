#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Arduino.h>

// Serial communication speed
#define BAUDRATE 115200

// I/O Pin Definitions
#define BUTTON_IO 2
#define SMOKE_IO  11  // Relay / Transistor active contactor 

// Timing Configurations (in milliseconds)
const uint32_t ACTIVATION_TIME = 1000;      // Cooldown delay before allowing first press
const uint16_t DELAY_LED = 1000;            // Pulse width for long signals
const uint16_t DELAY_SMOKE = 50;           // Pulse width for short trigger signals
const uint32_t TIME_END_SESSION = 30000;    // Active session duration (30 seconds)
const uint16_t BOUNCE_TIME = 100;           // Debounce delay for the arcade button

#endif