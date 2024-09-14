#ifndef ILI9341_FUNCTIONS
#define ILI9341_FUNCTIONS

    void drawFramework();
    void initializingDisplay();
    void eraseInformation(int row, int color=0);
    void drawHistogram(const float* data, int numBars);
    void setChannelValue(double value1, char unit[6], int channel);
    void setRelationValue(float value1);
    void drawInformation(const char text[100], int row, int color);
    void setGraphicalValue(bool initialized=true, std::vector<double> channel_1={0}, std::vector<double> channel_2={0});
    void setGraphicalLimitsInformation(std::vector<double> channel_1={0}, std::vector<double> channel_2={0});
    void drawPlayAndStop(bool status=true);
#endif
