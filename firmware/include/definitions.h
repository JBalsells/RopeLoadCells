#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SPI.h>

#include <vector>

// Configuración de pines para el ILI9341 TFT
#define TFT_CS    5
#define TFT_DC    2
#define TFT_RST   4

// Configuración de pines para el ADS1232 canal 1
#define pinData1 34
#define pinPOMN1 33
#define pinSCLK1 32

// Configuración de pines para el ADS1232 canal 2
#define pinData2 27
#define pinPOMN2 26
#define pinSCLK2 25
