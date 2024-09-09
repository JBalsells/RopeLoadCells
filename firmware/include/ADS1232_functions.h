#ifndef ADS1232_FUNCTIONS
#define ADS1232_FUNCTIONS

    long readADS1232(int PinData, int PinSCLK, int PinPOMN);
    long auto_calibrate(int samples, int PinData, int PinSCLK, int PinPOMN);

#endif
