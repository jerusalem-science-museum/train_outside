#include "constants.h"
#include "functions.h"

// Global state variables definition
uint32_t time_start = 0;
uint32_t time_new_session = 0;
bool check_ignit = false;
bool buttonPressed = false;
bool flag_first_press = false;

void setup() {
  // Initialize input pin with internal pull-up
  pinMode(BUTTON_IO, INPUT_PULLUP);  
  
  // Set smoke control pin to OUTPUT and pull HIGH immediately to prevent startup noise
  pinMode(SMOKE_IO, OUTPUT);
  digitalWrite(SMOKE_IO, HIGH); 

  // Initialize Serial Monitor
  Serial.begin(BAUDRATE);            
  Serial.println("System Initialized (Toggle Mode)...");
}

void loop() {
  // Read current state of the arcade button
  buttonPressed = PRESS_BUTTON_IGNITION();

  // State 1: Idle Mode (System is off, waiting for button trigger)
  if (!flag_first_press) {
    // Ensure cooldown period has passed before enabling start
    if ((millis() - time_start) > ACTIVATION_TIME) {
      if (buttonPressed) {
        Serial.println("Button pressed! Starting 30-second session.");
        
        // Send turn-on signal sequence
        SMOKE(DELAY_LED, 2);
        SMOKE(DELAY_SMOKE, 1);

        flag_first_press = true; 
        time_new_session = millis(); // Store session start timestamp
      }
    }
  } 
  // State 2: Active Mode (Mist maker is currently running)
  else {
    // Condition A: User interrupts the session early with another press
    if (buttonPressed) {
      Serial.println("Interrupted by user! Turning off early.");
      stop_and_reset_session();
    }
    // Condition B: Cooldown/Session timer times out automatically (30 seconds)
    else if (millis() - time_new_session > TIME_END_SESSION) {
      Serial.println("30 seconds timeout reached. Turning off automatically.");
      stop_and_reset_session();
    }
  }

  delay(1); // Small system stability delay
}