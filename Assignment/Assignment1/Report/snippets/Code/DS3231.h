/**
 *  DS3231 Class
 *  Inherits from I2CDevice class
 */

#ifndef DS3231_h
#define DS3231_h

#include "I2CDevice.h"
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#define BUFFER_SIZE 19      //0x00 to 0x12
#define SEC_REG 0x00
#define MIN_REG 0x01
#define HR_REG 0x02
#define DAY_REG 0x03
#define DATE_REG 0x04
#define MTH_REG 0x05
#define YR_REG 0x06
#define A1_SEC 0x07
#define A1_MIN 0x08
#define A1_HR 0x09
#define A1_DAY 0x0a
#define A2_MIN 0x0b
#define A2_HR 0x0c
#define A2_DAY 0x0d
#define CNTL 0x0e

class DS3231 : public I2CDevice{
    private:
    int bcdToDec(char);
    int decToBCD(char);
    char hrRegChangeUpperBits(char, int);
    char dateRegChangeUpperBits(char, int);
    char changeBits(bool, int, int);
    public:    
    DS3231();
    DS3231(char);
    DS3231(bool);
    void sysTimeRTCInit();
    void readTimeAndDate();
    void writeTime(char*);
    void writeDate(char*);
    void readAlarms();
    void setAlarms(char*);
    void readTemp();
    void setInterrupt(bool, bool);
    void sqWaveGen(int, bool);
    void clearAlarms();
};

#endif