#ifndef ILI9341_FUNCTIONS
#define ILI9341_FUNCTIONS

    void initializingDisplay();
    void setValue(long value1, char unit[6], int channel);
    void drawHistogram(const float* data, int numBars);

#endif