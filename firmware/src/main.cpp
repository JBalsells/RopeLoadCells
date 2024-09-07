#include <Arduino.h>
#include "definitions.h"
#include "math_functions.h"
#include "ILI9341_functions.h"
#include "ADS1232_functions.h"

char UNIT[6] = "";
int OFFSET = 0;
int CHANNEL = 1;
int SAMPLES = 100;
long raw_value_channel_1 = 0;
long raw_value_channel_2 = 0;

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
  drawFramework();
  eraseInformation(3);
  drawInformation("_____calibrating_____", 3, 1);
  OFFSET = auto_calibrate(SAMPLES);
  eraseInformation(3);

  char samples_str[10];
  char offset_str[50];
  char calibration_info[50] = "samples:";
  snprintf(samples_str, sizeof(samples_str), "%d", SAMPLES);
  snprintf(offset_str, sizeof(offset_str), "%d", OFFSET);
  strcat(calibration_info, samples_str);
  strcat(calibration_info, " offset:");
  strcat(calibration_info, offset_str);
  drawInformation(calibration_info, 3, 0);
}

void loop() {

  CHANNEL = 1;
  strcpy(UNIT, "u");
  raw_value_channel_1 = readADS1232() - OFFSET;
  setNumericValue(raw_value_channel_1, UNIT, CHANNEL);

  CHANNEL = 2;
  strcpy(UNIT, "u");
  raw_value_channel_2 = readADS1232() - OFFSET;
  setNumericValue(raw_value_channel_2, UNIT, CHANNEL);

  CHANNEL = 3;
  setNumericValue(raw_value_channel_2/raw_value_channel_1, "u", CHANNEL);

}
