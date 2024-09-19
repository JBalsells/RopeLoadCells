#include <Arduino.h>
#include "definitions.h"
#include "math_functions.h"
#include "ILI9341_functions.h"
#include "ADS1232_functions.h"

char UNIT[6] = "";
int CHANNEL = 1;
int SAMPLES = 20;
bool PlayAndStop = true;
int OFFSET_CHANNEL_1 = 0;
int OFFSET_CHANNEL_2 = 0;
long raw_value_channel_1 = 0;
long raw_value_channel_2 = 0;
double normalized_channel_1 = 0;
double normalized_channel_2 = 0;
float scaled_value_channel_1 = 0;
float scaled_value_channel_2 = 0;
double interpolated_value_channel_1 = 0;
double interpolated_value_channel_2 = 0;

int vector_index = 0;
int graphics_zero = 0;
std::vector<double> graphics_channel_1_vector;
std::vector<double> graphics_channel_2_vector;
std::vector<double> moving_average_channel_1 = {0,0,0,0,0};
std::vector<double> moving_average_channel_2 = {0,0,0,0,0};
std::vector<double> normalized_channel_1_vector = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
std::vector<double> normalized_channel_2_vector = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
std::vector<double> scaled_channel_1_vector = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
std::vector<double> scaled_channel_2_vector = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

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

  raw_value_channel_1 = readADS1232(pinData1, pinSCLK1, pinPOMN1);
  raw_value_channel_2 = readADS1232(pinData2, pinSCLK2, pinPOMN2);

  normalized_channel_1 = raw_value_channel_1 - OFFSET_CHANNEL_1;
  normalized_channel_2 = raw_value_channel_2 - OFFSET_CHANNEL_2;

  moving_average_channel_1.erase(moving_average_channel_1.begin());
  moving_average_channel_1.push_back(normalized_channel_1);
  moving_average_channel_2.erase(moving_average_channel_2.begin());
  moving_average_channel_2.push_back(normalized_channel_2);

  interpolated_value_channel_1 = interpolation(moving_average_channel_1);
  interpolated_value_channel_2 = interpolation(moving_average_channel_2);

  scaled_value_channel_1 = abs(load_scale(interpolated_value_channel_1));
  scaled_value_channel_2 = abs(load_scale(interpolated_value_channel_2));

  scaled_channel_1_vector.erase(scaled_channel_1_vector.begin());
  scaled_channel_1_vector.push_back(scaled_value_channel_1);
  scaled_channel_2_vector.erase(scaled_channel_2_vector.begin());
  scaled_channel_2_vector.push_back(scaled_value_channel_2);

  normalized_channel_1_vector.erase(normalized_channel_1_vector.begin());
  normalized_channel_1_vector.push_back(normalized_channel_1);
  normalized_channel_2_vector.erase(normalized_channel_2_vector.begin());
  normalized_channel_2_vector.push_back(normalized_channel_2);

  std::tie(graphics_channel_1_vector, graphics_channel_2_vector, graphics_zero) = normalize_vectors(normalized_channel_1_vector, normalized_channel_2_vector);

  if(PlayAndStop == true){
    CHANNEL = 1;
    strcpy(UNIT, "N");
    setChannelValue(scaled_value_channel_1, UNIT, CHANNEL);

    CHANNEL = 2;
    strcpy(UNIT, "N");
    setChannelValue(scaled_value_channel_2, UNIT, CHANNEL);

    setRelationValue((float)scaled_value_channel_1/(float)scaled_value_channel_2);

    setGraphicalValue(false, graphics_channel_1_vector, graphics_channel_2_vector, scaled_channel_1_vector, scaled_channel_2_vector, graphics_zero);
    setHistogramValue(false, scaled_channel_1_vector, scaled_channel_2_vector);

    drawPlayAndStop(true);
  }
  else{
    drawPlayAndStop(false);
  }
}
