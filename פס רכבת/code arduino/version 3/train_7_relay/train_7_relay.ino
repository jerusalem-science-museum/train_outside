#include "basic_routine.h"

// ====== SETUP ======
void setup() {
  Serial.begin(BAUDRATE);

  if (trainDetectionSignal == LOW) {
    for (int i = 0; i < numOfSegments; i++) pinMode(sensorPins[i], INPUT_PULLUP);
  } else {
    for (int i = 0; i < numOfSegments; i++) pinMode(sensorPins[i], INPUT);
  }
  for (int i = 0; i < numOfSegments; i++) pinMode(STATIONPins[i], OUTPUT);

  pinMode(BUTTON_NEXT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_OK_PIN,   INPUT_PULLUP);

  // Tout ouvert au départ
  for (int i = 0; i < numOfSegments; i++) Station_Disconnected(i);
  // Exemple d'état initial
  Station_Connected(0 % numOfSegments);

  // Mode manuel au boot
  manualRelaySelector();

  wdt_enable(WDTO_2S);
}

// ====== BOUCLE AUTO ======
void loop() {
  
  wdt_reset();
  int STATIONCurrentState;
  for (int i = 0; i < numOfSegments; i++) {
    int sensorPin = sensorPins[i];
    if(i == 5 || i == 6 || i == 7){
      STATIONCurrentState = analogRead(sensorPin);
    }
    else{
      STATIONCurrentState = digitalRead(sensorPin);
    }
    
    int prevSTATIONPin = STATIONPins[getPreviuosSegment(i)];
    if (STATIONCurrentState == trainDetectionSignal) {  
      Serial.println("sensorPin:" +String(i));
      Serial.println("STATIONCurrentState:" +String(STATIONCurrentState));
      Station_Disconnected(i);
      delay(100);
      Station_Connected(getPreviuosSegment(i));  
      delay(100);
    }
  }
}
