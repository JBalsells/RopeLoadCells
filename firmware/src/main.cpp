#include <Arduino.h>
#include "definitions.h"
#include "math_functions.h"
#include "ILI9341_functions.h"
#include "ADS1232_functions.h"


void setup() {
  Serial.begin(115200);
  randomSeed(100);
  
  pinMode(pinData1, INPUT);
  pinMode(pinSCLK1, OUTPUT);
  pinMode(pinPOMN1, OUTPUT);

  pinMode(pinData2, INPUT);
  pinMode(pinSCLK2, OUTPUT);
  pinMode(pinPOMN2, OUTPUT);

  digitalWrite(pinSCLK1, LOW);
  digitalWrite(pinPOMN1, HIGH);

  digitalWrite(pinSCLK2, LOW);
  digitalWrite(pinPOMN2, HIGH);

  initializingDisplay();
}

void loop() {
  long load_cell_raw_value = readADS1232();
  setValue(load_cell_raw_value);
  delay(100);

}
