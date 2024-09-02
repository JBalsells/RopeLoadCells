#include "definitions.h"

#define WHITE   ILI9341_WHITE
#define BLUE    ILI9341_BLUE
#define RED     ILI9341_RED
#define YELLOW  ILI9341_YELLOW
#define ORANGE  ILI9341_ORANGE
#define GREEN   ILI9341_GREEN
#define PURPLE  ILI9341_PURPLE
#define BLACK   ILI9341_BLACK

const int SCREEN_WIDTH_MIN = 0;
const int SCREEN_HEIGHT_MIN = 0;
const int SCREEN_WIDTH_MAX = 320;
const int SCREEN_HEIGHT_MAX = 240;
const int SCREEN_WIDTH_MIDDLE = (SCREEN_WIDTH_MAX - SCREEN_WIDTH_MIN)/2;
const int SCREEN_HEIGHT_MIDDLE = (SCREEN_HEIGHT_MAX-SCREEN_HEIGHT_MIN)/2;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

void drawHistogram(const float* data, int numBars) {
  int barWidth = 20;
  int spacing = 10;
  int maxBarHeight = 100;
  int startX = 10;
  int startY = 150;

  // Calculate max value for normalization
  float maxValue = 0;
  for (int i = 0; i < numBars; i++) {
    if (data[i] > maxValue) {
      maxValue = data[i];
    }
  }

  // Draw the bars
  for (int i = 0; i < numBars; i++) {
    int barHeight = map(data[i] * 10, 0, maxValue * 10, 0, maxBarHeight); // Scale floats to int
    int x = startX + i * (barWidth + spacing);
    int y = startY - barHeight;

    // Draw a rectangle for each bar
    tft.fillRect(x, y, barWidth, barHeight, ILI9341_BLUE);
  }
}

void drawFramework(){
  tft.setTextColor(GREEN);
  tft.setTextSize(2);
  tft.setCursor(SCREEN_WIDTH_MAX - 120, SCREEN_HEIGHT_MIN + 10);
  tft.println("Channel 1");

  tft.setTextColor(WHITE);
  tft.setTextSize(6);
  tft.setCursor(SCREEN_WIDTH_MAX - 45, (SCREEN_HEIGHT_MIDDLE-SCREEN_HEIGHT_MIN)/2);
  tft.println("N");

  tft.setTextColor(ORANGE);
  tft.setTextSize(2);
  tft.setCursor(SCREEN_WIDTH_MAX - 120, SCREEN_HEIGHT_MIDDLE + 10);
  tft.println("Channel 2");

  tft.setTextColor(WHITE);
  tft.setTextSize(6);
  tft.setCursor(SCREEN_WIDTH_MAX - 45, (SCREEN_HEIGHT_MAX-SCREEN_HEIGHT_MIDDLE)/2 + SCREEN_HEIGHT_MIDDLE);
  tft.println("N");

  tft.drawRect(SCREEN_WIDTH_MIN, SCREEN_HEIGHT_MIN, SCREEN_WIDTH_MAX, SCREEN_HEIGHT_MAX, WHITE);
  tft.drawRect(SCREEN_WIDTH_MIN, SCREEN_HEIGHT_MIN, SCREEN_WIDTH_MAX, SCREEN_HEIGHT_MAX/2, WHITE);
}

void setValue(long value1){
  tft.fillRect((SCREEN_WIDTH_MIN + SCREEN_WIDTH_MIDDLE)/2 + 10, (SCREEN_HEIGHT_MIN + SCREEN_HEIGHT_MIDDLE)/2, SCREEN_WIDTH_MAX - 145, (SCREEN_HEIGHT_MIDDLE-SCREEN_HEIGHT_MIN)/2 - 25, ORANGE);
  tft.setTextColor(WHITE);
  tft.setTextSize(5);
  tft.setCursor((SCREEN_WIDTH_MIN + SCREEN_WIDTH_MIDDLE)/2 + 10, (SCREEN_HEIGHT_MIN + SCREEN_HEIGHT_MIDDLE)/2);
  tft.println(value1);
}

void initializingDisplay(){
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(5, 50);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);
  tft.println("LOAD CELLS DRIVER");
  delay(2000);
  tft.fillScreen(ILI9341_BLACK);
  drawFramework();
}
