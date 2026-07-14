#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Arduino.h>
#include <avr/wdt.h>
#include "constants.h"

// Reference global state variables defined in the main .ino sketch
extern uint32_t time_start;
extern uint32_t time_new_session;
extern bool check_ignit;
extern bool flag_first_press;

/**
 * Non-blocking debounce function to register button release (click event)
 * @return true if button was fully pressed and released, false otherwise
 */
inline bool PRESS_BUTTON_IGNITION() {
  wdt_reset(); 

  // Detect initial press down
  if (digitalRead(BUTTON_IO) == LOW && !check_ignit) {
    check_ignit = true;         
    delay(BOUNCE_TIME);         
  }
  
  // Detect button release
  if (digitalRead(BUTTON_IO) == HIGH && check_ignit) {
    check_ignit = false;        
    return true;                
  }
  return false; 
}

/**
 * Triggers pulses on the SMOKE_IO pin
 * Drives the pin LOW actively to trigger and HIGH to release, avoiding floating state noise
 */
inline void SMOKE(int delay_mode, int iteration) {
  for (int i = 0; i < iteration; i++) {
    // 1. Simulate Press (Drive LOW actively)
    pinMode(SMOKE_IO, OUTPUT);
    digitalWrite(SMOKE_IO, LOW); 
    delay(delay_mode);

    // 2. Simulate Release (Drive HIGH actively to prevent floating noise)
    digitalWrite(SMOKE_IO, HIGH);
    delay(500);
  }
}

/**
 * Handles the shutdown sequence, drives the control pin HIGH actively, and resets variables
 */
inline void stop_and_reset_session() {
  Serial.println("Stopping smoke and resetting to idle state...");
  
  // Send turn-off signal sequence (pulses)
  SMOKE(DELAY_LED, 1);
  SMOKE(DELAY_SMOKE, 2);
  
  // Keep the pin driven HIGH actively to prevent picking up noise in idle state
  pinMode(SMOKE_IO, OUTPUT);
  digitalWrite(SMOKE_IO, HIGH);
  
  // Reset active state flags
  flag_first_press = false;
  
  // Start the protective cooldown window
  time_start = millis(); 
}

#endif