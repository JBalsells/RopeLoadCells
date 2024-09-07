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
const int SCREEN_HEIGHT_MIDDLE = (SCREEN_HEIGHT_MAX - SCREEN_HEIGHT_MIN)/2;
const int MARGIN_SIZE = 10;
const int ROW_SIZE = 20;

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

void eraseText(int color, int x, int y, int w, int h){
  tft.fillRect(x, y, w, h, color);
}

void drawText(int color, int x, int y, int text_size, String text){
  tft.setTextColor(color);
  tft.setTextSize(text_size);
  tft.setCursor(x, y);
  tft.println(text);
}

void drawFramework(){
  drawText(GREEN, SCREEN_WIDTH_MIN + MARGIN_SIZE, SCREEN_HEIGHT_MIN + MARGIN_SIZE + ROW_SIZE*0, 2, "Channel 1");
  drawText(GREEN, SCREEN_WIDTH_MIN + MARGIN_SIZE, SCREEN_HEIGHT_MIN + MARGIN_SIZE + ROW_SIZE*1, 2, "Channel 2");
  drawText(ORANGE, SCREEN_WIDTH_MIN + MARGIN_SIZE, SCREEN_HEIGHT_MIN + MARGIN_SIZE + ROW_SIZE*2, 2, "Relation CH2/CH1");
  drawText(ORANGE, SCREEN_WIDTH_MIN + MARGIN_SIZE, SCREEN_HEIGHT_MIN + MARGIN_SIZE + ROW_SIZE*3, 2, "Relation CH2/CH1");
  drawText(ORANGE, SCREEN_WIDTH_MIN + MARGIN_SIZE, SCREEN_HEIGHT_MIN + MARGIN_SIZE + ROW_SIZE*4, 2, "Relation CH2/CH1");

  tft.drawRect(SCREEN_WIDTH_MIN, SCREEN_HEIGHT_MIN, SCREEN_WIDTH_MAX, SCREEN_HEIGHT_MAX, WHITE);
  tft.drawRect(SCREEN_WIDTH_MIN, SCREEN_HEIGHT_MIDDLE, SCREEN_WIDTH_MAX, SCREEN_HEIGHT_MIDDLE, WHITE);
  tft.drawRect(SCREEN_WIDTH_MIDDLE, SCREEN_HEIGHT_MIDDLE, SCREEN_WIDTH_MIDDLE, SCREEN_HEIGHT_MIDDLE, WHITE);
}

void setValue(long value1, char unit[6], int channel){
  int x = 0;
  int y = 0;
  int w = 0;
  int h = 0;
  int row = 0;
  int color = 0;

  if (channel == 1){
    row = 0;
    color = YELLOW;
    }
  if (channel == 2){
    row = 1;
    color = YELLOW;
    }

  x = SCREEN_WIDTH_MIDDLE;
  y = SCREEN_HEIGHT_MIN + MARGIN_SIZE + ROW_SIZE*row;
  w = SCREEN_WIDTH_MIDDLE - 1;
  h = ROW_SIZE - 1;

  char str[12];
  snprintf(str, sizeof(str), "%d", value1);
  strcat(str, " ");
  strcat(str, unit);

  eraseText(BLACK, x, y, w, h);
  drawText(color, x, y, 2, str);
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
