#ifndef ILI9341_FUNCTIONS
#define ILI9341_FUNCTIONS

    void drawFramework();
    void initializingDisplay();
    void eraseInformation(int row, int color=0);
    void drawHistogram(const float* data, int numBars);
    void setNumericValue(long value1, char unit[6], int channel);
    void drawInformation(const char text[100], int row, int color);

#endif