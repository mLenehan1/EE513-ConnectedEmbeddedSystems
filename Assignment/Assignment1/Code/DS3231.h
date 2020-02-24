#include<stdio.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<linux/i2c.h>
#include<linux/i2c-dev.h>
#define BUFFER_SIZE 19      //0x00 to 0x12

class DS3231{
    private:
    int bcdToDec();
    int openBus();
    int connectSlave();
    int resetReadAddress();
    int readBuffer();
    protected:
    public:
    DS3231();
    int readTimeAndDate();
    int setTimeAndDate();
    int readTemp();
    int readAlarms();
    int setAlarms();
};