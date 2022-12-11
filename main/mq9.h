#ifndef mq9_LIBRARY_H
#define mq9_LIBRARY_H
#include "Arduino.h"
class mq9 {
public:



    void *inits();
    void *calibrate();
    double *getValue(bool printing,char gasName, double *output);
    bool thrValue(char gasName,int threshold);
    bool thrNvalue(int threshold);
private:
    int readADC(int pin);
    void printData(float data);
    float _R0=10;
    double _M0=0.0;
    double _B0=0.0;
    int adcBits = 0;
    float adcVoltage =0;
};
#endif
