#include "definitions.h"
#include "math_functions.h"

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

#define CHANNEL_1_COLOR             0x079f
#define CHANNEL_1_GRAPH_COLOR       0x0597
#define CHANNEL_1_STATISTICS_COLOR  0x0431
#define CHANNEL_2_COLOR             0xffa0
#define CHANNEL_2_GRAPH_COLOR       0xce20
#define CHANNEL_2_STATISTICS_COLOR  0x8c40

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

void eraseText(int color, int x, int y, int w, int h){
  tft.fillRect(x, y, w, h, color);
}

void drawText(int color, int x, int y, int text_size, String text){
  tft.setTextColor(color);
  tft.setTextSize(text_size);
  tft.setCursor(x, y);
  tft.println(text);
}

void setHistogramInformation(std::vector<double> channel_1={0}, std::vector<double> channel_2={0}){
  int row = 0;
  int row_size = 8;
  int margin_size = 3;

  char message[40];
  double ch1_mean = 0;
  double ch1_std = 0;
  double ch2_mean = 0;
  double ch2_std = 0;
  std::tie(ch1_mean, ch1_std) = calculateStandardDeviation(channel_1);
  std::tie(ch2_mean, ch2_std) = calculateStandardDeviation(channel_2);

  row = 0;
  snprintf(message, sizeof(message), "std: %.2f", ch1_std);
  drawText(CHANNEL_1_COLOR, SCREEN_WIDTH_MIDDLE + margin_size, SCREEN_HEIGHT_MIDDLE + margin_size + row_size*row, 1, message);
  
  row = 1;
  snprintf(message, sizeof(message), "mean: %.2f", ch1_mean);
  drawText(CHANNEL_1_COLOR, SCREEN_WIDTH_MIDDLE + margin_size, SCREEN_HEIGHT_MIDDLE + margin_size + row_size*row, 1, message);
  
  row = 2;
  snprintf(message, sizeof(message), "std: %.2f", ch2_std);
  drawText(CHANNEL_2_COLOR, SCREEN_WIDTH_MIDDLE + margin_size, SCREEN_HEIGHT_MIDDLE + margin_size + row_size*row, 1, message);
  
  row = 3;
  snprintf(message, sizeof(message), "mean: %.2f", ch2_mean);
  drawText(CHANNEL_2_COLOR, SCREEN_WIDTH_MIDDLE + margin_size, SCREEN_HEIGHT_MIDDLE + margin_size + row_size*row, 1, message);
}

void setHistogramValue(int initialized=true, std::vector<double> channel_1 = {0}, std::vector<double> channel_2 = {0}){

  int minHeight = 0; // Altura mínima de la barra
  int maxHeight = SCREEN_HEIGHT_MAX - SCREEN_HEIGHT_MIDDLE - 2; // Altura máxima de la barra

  int numBins = 25;
  int barWidth = (SCREEN_WIDTH_MAX - SCREEN_WIDTH_MIDDLE) / numBins;

  // Este fillrect es el área máxima a utilizar en el gráfico
  tft.fillRect(SCREEN_WIDTH_MIDDLE+1, SCREEN_HEIGHT_MIDDLE+1, SCREEN_WIDTH_MIDDLE-2, SCREEN_HEIGHT_MIDDLE + 2*ROW_SIZE - 2, BLACK);


  // Calcular el rango de valores en channel_1
  double max_value_1 = *std::max_element(channel_1.begin(), channel_1.end());
  double min_value_1 = *std::min_element(channel_1.begin(), channel_1.end());

  // Crear los bins y contar las frecuencias para channel_1
  std::vector<int> binCounts_ch_1(numBins, 0);
  double binSize_1 = (max_value_1 - min_value_1) / numBins;

  for(double value : channel_1){
    int binIndex = std::min(static_cast<int>((value - min_value_1) / binSize_1), numBins - 1);
    binCounts_ch_1[binIndex]++;
  }

  // Calcular el rango de valores en channel_2
  double max_value_2 = *std::max_element(channel_2.begin(), channel_2.end());
  double min_value_2 = *std::min_element(channel_2.begin(), channel_2.end());

  // Crear los bins y contar las frecuencias para channel_2
  std::vector<int> binCounts_ch_2(numBins, 0);
  double binSize_2 = (max_value_2 - min_value_2) / numBins;

  for(double value : channel_2){
    int binIndex = std::min(static_cast<int>((value - min_value_2) / binSize_2), numBins - 1);
    binCounts_ch_2[binIndex]++;
  }

  // Determinar el máximo valor de frecuencia entre ambos canales
  int maxCount = std::max(*std::max_element(binCounts_ch_1.begin(), binCounts_ch_1.end()),
                          *std::max_element(binCounts_ch_2.begin(), binCounts_ch_2.end()));

  for(int i = 0; i < numBins; i++){
    // Normalizar la altura de la barra basada en la frecuencia del bin para channel_1
    int barHeight_1 = minHeight + (binCounts_ch_1[i] * (maxHeight - minHeight)) / maxCount;
    // Normalizar la altura de la barra basada en la frecuencia del bin para channel_2
    int barHeight_2 = minHeight + (binCounts_ch_2[i] * (maxHeight - minHeight)) / maxCount;
    
    // Coordenadas y dimensiones de la barra para channel_1
    int x = SCREEN_WIDTH_MIDDLE + 1 + i * barWidth;
    int y_1 = SCREEN_HEIGHT_MAX - barHeight_1 - 1;
    int height_1 = barHeight_1;

    // Coordenadas y dimensiones de la barra para channel_2
    int y_2 = SCREEN_HEIGHT_MAX - barHeight_2 - 1;
    int height_2 = barHeight_2;

    // Mezclar colores en las intersecciones
    uint16_t color_1 = CHANNEL_1_STATISTICS_COLOR;
    uint16_t color_2 = CHANNEL_2_STATISTICS_COLOR;
    uint16_t mixedColor = ((color_1 & 0xF800) >> 8) | ((color_2 & 0xF800) >> 3) | (((color_1 & 0x07E0) >> 8) | ((color_2 & 0x07E0) >> 5) << 5) | (((color_1 & 0x001F) << 3) | ((color_2 & 0x001F) >> 3) << 11);

    // Dibujar las barras de channel_1
    tft.fillRect(x, y_1, barWidth, height_1, color_1);

    // Dibujar las barras de channel_2
    tft.fillRect(x, y_2, barWidth, height_2, color_2);

    // Mezcla en la intersección de las dos barras
    if (y_1 < y_2 + height_2 && y_2 < y_1 + height_1) {
      int overlapHeight = std::min(y_1 + height_1, y_2 + height_2) - std::max(y_1, y_2);
      if (overlapHeight > 0) {
        tft.fillRect(x, std::max(y_1, y_2), barWidth, overlapHeight, mixedColor);
      }
    }
  }

  // Dibujar líneas del eje del histograma
  if(initialized != true){
    for(int i=-75; i<=75; i+=15){
      tft.drawLine(HISTOGRAM_Y_AXIS + i, SCREEN_HEIGHT_MIDDLE + 1, HISTOGRAM_Y_AXIS + i, SCREEN_HEIGHT_MAX - 2, DGRAY);
      if(i == 0){ tft.drawLine(HISTOGRAM_Y_AXIS, SCREEN_HEIGHT_MIDDLE + 1, HISTOGRAM_Y_AXIS, SCREEN_HEIGHT_MAX - 2, RED); }
    }
  }

  setHistogramInformation(channel_1, channel_2);
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

void setGraphicalValue(bool initialized = true, std::vector<double> channel_1 = {0}, std::vector<double> channel_2 = {0}, std::vector<double> scaled_channel_1 = {0}, std::vector<double> scaled_channel_2 = {0}, int zero = 0) {
  
  int row = 0;
  int row_size = 8;
  int margin_size = 3;

  // Este fillRect es el área máxima a utilizar en el gráfico
  tft.fillRect(SCREEN_WIDTH_MIN + 1, SCREEN_HEIGHT_MIDDLE + 1, SCREEN_WIDTH_MIDDLE - 1, SCREEN_HEIGHT_MIDDLE + 2 * ROW_SIZE - 2, BLACK);

  if (initialized != true) {
    int color_channel_1 = CHANNEL_1_GRAPH_COLOR;
    int color_channel_2 = CHANNEL_2_GRAPH_COLOR;

    // Dibujar las líneas de la cuadrícula
    for (int i = -60; i <= 60; i += 15) {
      tft.drawLine(SCREEN_WIDTH_MIN + 1, GRAPHIC_X_AXIS + i, SCREEN_WIDTH_MIDDLE - 1, GRAPHIC_X_AXIS + i, DGRAY);
    }

    // Dibujar líneas continuas para channel_1 y channel_2
    for (int i = 1; i < channel_1.size(); i++) {
      // Dibujar líneas continuas en lugar de píxeles para channel_1 y channel_2
      tft.drawLine(i - 1, GRAPHIC_X_AXIS + channel_1[i - 1], i, GRAPHIC_X_AXIS + channel_1[i], color_channel_1);
      tft.drawLine(i - 1, GRAPHIC_X_AXIS + channel_2[i - 1], i, GRAPHIC_X_AXIS + channel_2[i], color_channel_2);
    }

    tft.drawLine(SCREEN_WIDTH_MIN + 1, GRAPHIC_X_AXIS + zero, SCREEN_WIDTH_MIDDLE - 1, GRAPHIC_X_AXIS + zero, RED);
  }

  setGraphicalLimitsInformation(scaled_channel_1, scaled_channel_2);
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
