#include <Arduino.h>
#include "definitions.h"
#include "math_functions.h"
#include "ILI9341_functions.h"
#include "ADS1232_functions.h"

char UNIT[6] = "";
int OFFSET_CHANNEL_1 = 0;
int OFFSET_CHANNEL_2 = 0;
int CHANNEL = 1;
int SAMPLES = 150;
long raw_value_channel_1 = 0;
long raw_value_channel_2 = 0;
double normalized_channel_1 = 0;
double normalized_channel_2 = 0;

int vector_index = 0;
std::vector<int> channel_1_vector = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
std::vector<int> channel_2_vector = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


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

  char samples_str[10];
  snprintf(samples_str, sizeof(samples_str), "%d", SAMPLES);
  char ch1_info[50] = "CALIBRATING CH1 SMP: ";
  drawInformation(strcat(ch1_info, samples_str), 3, 1);
  OFFSET_CHANNEL_1 = auto_calibrate(SAMPLES, pinData1, pinSCLK1, pinPOMN1);
  eraseInformation(3);
  char ch2_info[50] = "CALIBRATING CH2 SMP: ";
  drawInformation(strcat(ch2_info, samples_str), 3, 1);
  OFFSET_CHANNEL_2 = auto_calibrate(SAMPLES, pinData2, pinSCLK2, pinPOMN2);
  eraseInformation(3);

  char offset_str_1[20];
  char offset_str_2[20];
  char calibration_info[50] = "";
  snprintf(offset_str_1, sizeof(offset_str_1), "%d", OFFSET_CHANNEL_1);
  snprintf(offset_str_2, sizeof(offset_str_2), "%d", OFFSET_CHANNEL_2);
  strcat(calibration_info, " OS1:");
  strcat(calibration_info, offset_str_1);
  strcat(calibration_info, " OS2:");
  strcat(calibration_info, offset_str_2);
  drawInformation(calibration_info, 3, 0);
}

void loop() {

  CHANNEL = 1;
  strcpy(UNIT, "u");
  raw_value_channel_1 = readADS1232(pinData1, pinSCLK1, pinPOMN1);
  normalized_channel_1 = load_scale(raw_value_channel_1 - OFFSET_CHANNEL_1);
  setChannelValue(normalized_channel_1, UNIT, CHANNEL);

  CHANNEL = 2;
  strcpy(UNIT, "u");
  raw_value_channel_2 = readADS1232(pinData2, pinSCLK2, pinPOMN2);
  normalized_channel_2 = load_scale(raw_value_channel_2 - OFFSET_CHANNEL_2);
  setChannelValue(normalized_channel_2, UNIT, CHANNEL);

  setRelationValue((float)normalized_channel_1/(float)normalized_channel_2);


  channel_1_vector.erase(channel_1_vector.begin());
  channel_1_vector.push_back(normalized_channel_1);
  std::vector<int> channel_1_normalized_vector = normalize_vector(channel_1_vector);


  channel_2_vector.erase(channel_2_vector.begin());
  channel_2_vector.push_back(-10);

  setGraphicalValue(false, channel_1_normalized_vector, channel_2_vector);
}
