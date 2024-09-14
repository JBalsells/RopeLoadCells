#include "definitions.h"

#define WHITE   ILI9341_WHITE
#define BLUE    ILI9341_BLUE
#define RED     ILI9341_RED
#define YELLOW  ILI9341_YELLOW
#define ORANGE  ILI9341_ORANGE
#define GREEN   ILI9341_GREEN
#define PURPLE  ILI9341_PURPLE
#define BLACK   ILI9341_BLACK
#define GRAY    ILI9341_DARKGREY
#define DGRAY   0x2104

#define CHANNEL_1_COLOR       0x077f
#define CHANNEL_1_DARK_COLOR  0x0619
#define CHANNEL_2_COLOR       0xf2bf
#define CHANNEL_2_DARK_COLOR  0xa9d5

const int ROW_SIZE = 20;
const int MARGIN_SIZE = 10;
const int SCREEN_WIDTH_MIN = 0;
const int SCREEN_HEIGHT_MIN = 0;
const int SCREEN_WIDTH_MAX = 320;
const int SCREEN_HEIGHT_MAX = 240;
const int SCREEN_WIDTH_MIDDLE = (SCREEN_WIDTH_MAX - SCREEN_WIDTH_MIN)/2;
const int SCREEN_HEIGHT_MIDDLE = (SCREEN_HEIGHT_MAX - SCREEN_HEIGHT_MIN)/2 - ROW_SIZE;
const int GRAPHIC_X_AXIS = (SCREEN_HEIGHT_MAX-SCREEN_HEIGHT_MIDDLE)/2 + SCREEN_HEIGHT_MIDDLE;
const int HISTOGRAM_Y_AXIS = (SCREEN_WIDTH_MAX-SCREEN_WIDTH_MIDDLE)/2 + SCREEN_WIDTH_MIDDLE;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

void drawPlayAndStop(bool status=true){

  int radius = 10;
  int circle_margin = 4;

  if(status==true){
    tft.fillCircle(SCREEN_WIDTH_MAX - radius - circle_margin, SCREEN_HEIGHT_MIN + radius + circle_margin, radius, GREEN);
  }
  else{
    tft.fillCircle(SCREEN_WIDTH_MAX - radius - circle_margin, SCREEN_HEIGHT_MIN + radius + circle_margin, radius, RED);
  }
}

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

void setHistogramValue(int initialized=true, long value=0, int channel=0){
  
  int row = 0;
  int row_size = 8;
  int margin_size = 3;

  //Este fillrect es el area maxima a utilizar en el grafico
  tft.fillRect(SCREEN_WIDTH_MIDDLE+1, SCREEN_HEIGHT_MIDDLE+1, SCREEN_WIDTH_MIDDLE-2, SCREEN_HEIGHT_MIDDLE + 2*ROW_SIZE - 2, BLACK);

  if(initialized!=false){
    for(int i=-75;i<=75;i+=15){
      tft.drawLine(HISTOGRAM_Y_AXIS+i, SCREEN_HEIGHT_MIDDLE+1, HISTOGRAM_Y_AXIS+i, SCREEN_HEIGHT_MAX-2, DGRAY);
      if(i==0){tft.drawLine(HISTOGRAM_Y_AXIS, SCREEN_HEIGHT_MIDDLE+1, HISTOGRAM_Y_AXIS, SCREEN_HEIGHT_MAX-2, RED);}
    }

    row = 0;
    drawText(CHANNEL_1_COLOR, SCREEN_WIDTH_MIDDLE + margin_size, SCREEN_HEIGHT_MIDDLE + margin_size + row_size*row, 1, "std: ");
    row = 1;
    drawText(CHANNEL_1_COLOR, SCREEN_WIDTH_MIDDLE + margin_size, SCREEN_HEIGHT_MIDDLE + margin_size + row_size*row, 1, "mean: ");
    row = 2;
    drawText(CHANNEL_2_COLOR, SCREEN_WIDTH_MIDDLE + margin_size, SCREEN_HEIGHT_MIDDLE + margin_size + row_size*row, 1, "std: ");
    row = 3;
    drawText(CHANNEL_2_COLOR, SCREEN_WIDTH_MIDDLE + margin_size, SCREEN_HEIGHT_MIDDLE + margin_size + row_size*row, 1, "mean: ");
  }
}

void setGraphicalLimitsInformation(std::vector<double> channel_1={0}, std::vector<double> channel_2={0}){
  int row = 0;
  int row_size = 8;
  int margin_size = 3;

  char message[40];
  double ch1_max = *std::max_element(channel_1.begin(), channel_1.end());
  double ch1_min = *std::min_element(channel_1.begin(), channel_1.end());
  double ch2_max = *std::max_element(channel_2.begin(), channel_2.end());
  double ch2_min = *std::min_element(channel_2.begin(), channel_2.end());

  row = 0;
  snprintf(message, sizeof(message), "max: %.2f", ch1_max);
  drawText(CHANNEL_1_COLOR, SCREEN_WIDTH_MIN + margin_size, SCREEN_HEIGHT_MIDDLE + margin_size + row_size*row, 1, message);

  row = 1;
  snprintf(message, sizeof(message), "min: %.2f", ch1_min);
  drawText(CHANNEL_1_COLOR, SCREEN_WIDTH_MIN + margin_size, SCREEN_HEIGHT_MIDDLE + margin_size + row_size*row, 1, message);

  row = 2;
  snprintf(message, sizeof(message), "max: %.2f", ch2_max);
  drawText(CHANNEL_2_COLOR, SCREEN_WIDTH_MIN + margin_size, SCREEN_HEIGHT_MIDDLE + margin_size + row_size*row, 1, message);

  row = 3;
  snprintf(message, sizeof(message), "min: %.2f", ch2_min);
  drawText(CHANNEL_2_COLOR, SCREEN_WIDTH_MIN + margin_size, SCREEN_HEIGHT_MIDDLE + margin_size + row_size*row, 1, message); 
}

void setGraphicalValue(bool initialized=true, std::vector<double> channel_1={0}, std::vector<double> channel_2={0}){
  
  int row = 0;
  int row_size = 8;
  int margin_size = 3;

  //Este fillrect es el area maxima a utilizar en el grafico
  tft.fillRect(SCREEN_WIDTH_MIN +1, SCREEN_HEIGHT_MIDDLE +1, SCREEN_WIDTH_MIDDLE -1, SCREEN_HEIGHT_MIDDLE + 2*ROW_SIZE - 2, BLACK);
  
  if(initialized!=true){
    int color_channel_1 = CHANNEL_1_DARK_COLOR;
    int color_channel_2 = CHANNEL_2_DARK_COLOR;

    for(int i=-60;i<=60;i+=15){
      tft.drawLine(SCREEN_WIDTH_MIN+1, GRAPHIC_X_AXIS+i, SCREEN_WIDTH_MIDDLE-1, GRAPHIC_X_AXIS+i, DGRAY);
      if(i==0){tft.drawLine(SCREEN_WIDTH_MIN+1, GRAPHIC_X_AXIS, SCREEN_WIDTH_MIDDLE-1, GRAPHIC_X_AXIS, RED);}
    }

    for(int i=1;i<channel_1.size();i++){
      tft.drawPixel(i, GRAPHIC_X_AXIS+channel_1[i], color_channel_1);
      tft.drawPixel(i, GRAPHIC_X_AXIS+channel_2[i], color_channel_2);
    }
  }
}

void setRelationValue(float value){
  int x = 0;
  int y = 0;
  int w = 0;
  int h = 0;
  int row = 0;
  int color = 0;

  row = 2;
  color = YELLOW;

  x = SCREEN_WIDTH_MIDDLE;
  y = SCREEN_HEIGHT_MIN + MARGIN_SIZE + ROW_SIZE*row;
  w = SCREEN_WIDTH_MIDDLE - 1;
  h = ROW_SIZE - 1;

  char float_value[20];
  snprintf(float_value, sizeof(float_value), "%.4f", value);

  eraseText(BLACK, x, y, w, h);
  drawText(color, x, y, 2, float_value);
}

void setChannelValue(double value, char unit[6], int channel){
  int x = 0;
  int y = 0;
  int w = 0;
  int h = 0;
  int row = 0;
  int color = 0;

  if (channel == 1){
    row = 0;
    color = WHITE;
    }
  if (channel == 2){
    row = 1;
    color = WHITE;
    }

  x = SCREEN_WIDTH_MIDDLE;
  y = SCREEN_HEIGHT_MIN + MARGIN_SIZE + ROW_SIZE*row;
  w = SCREEN_WIDTH_MIDDLE - 1;
  h = ROW_SIZE - 1;

  char concatenated_value[20];
  snprintf(concatenated_value, sizeof(concatenated_value), "%.2f", value);
  strcat(concatenated_value, " ");
  strcat(concatenated_value, unit);

  eraseText(BLACK, x, y, w-25, h);
  drawText(color, x, y, 2, concatenated_value);
}

void drawInformation(const char text[100], int row, int color=0){
  //Information is in rows 3 and 4

  if(color==0){
    color = WHITE;
    }
  if(color==1){
    color = RED;
    }

  drawText(color, SCREEN_WIDTH_MIN + MARGIN_SIZE, SCREEN_HEIGHT_MIN + MARGIN_SIZE + ROW_SIZE*row, 2, text);
}

void eraseInformation(int row, int color=0){
  if(color==0){
    color = BLACK;
    }
  eraseText(color, SCREEN_WIDTH_MIN + MARGIN_SIZE, SCREEN_HEIGHT_MIN + MARGIN_SIZE + ROW_SIZE*row, SCREEN_WIDTH_MAX - 2*MARGIN_SIZE, ROW_SIZE - 1);
}

void drawFramework(){
  drawText(CHANNEL_1_COLOR, SCREEN_WIDTH_MIN + MARGIN_SIZE, SCREEN_HEIGHT_MIN + MARGIN_SIZE + ROW_SIZE*0, 2, "Channel 1: ");
  drawText(CHANNEL_2_COLOR, SCREEN_WIDTH_MIN + MARGIN_SIZE, SCREEN_HEIGHT_MIN + MARGIN_SIZE + ROW_SIZE*1, 2, "Channel 2: ");
  drawText(WHITE, SCREEN_WIDTH_MIN + MARGIN_SIZE, SCREEN_HEIGHT_MIN + MARGIN_SIZE + ROW_SIZE*2, 2, "Rel CH1/CH2: ");
  drawInformation("", 3);
  drawInformation("", 4);

  tft.drawRect(SCREEN_WIDTH_MIN, SCREEN_HEIGHT_MIN, SCREEN_WIDTH_MAX, SCREEN_HEIGHT_MAX, GRAY);
  tft.drawRect(SCREEN_WIDTH_MIN, SCREEN_HEIGHT_MIDDLE, SCREEN_WIDTH_MAX, SCREEN_HEIGHT_MIDDLE + 2*ROW_SIZE, GRAY);
  tft.drawRect(SCREEN_WIDTH_MIDDLE, SCREEN_HEIGHT_MIDDLE, SCREEN_WIDTH_MIDDLE, SCREEN_HEIGHT_MIDDLE + 2*ROW_SIZE, GRAY);

  setGraphicalValue(true);
  setHistogramValue(true);
  drawPlayAndStop(false);
}

void initializingDisplay(){
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(5, 50);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);
  tft.println("LOAD CELLS DRIVER");
  delay(1000);

  tft.fillScreen(ILI9341_BLACK);
}
