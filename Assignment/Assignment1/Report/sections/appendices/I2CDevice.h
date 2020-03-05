/**
 * I2CDevice Class
 */

#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<linux/i2c.h>
#include<linux/i2c-dev.h>
#include<vector>

class I2CDevice{
    protected:
    int file;
    std::vector<char> buf;
    char addr;
    int openBus();
    void connectDevice();
    void resetReadAddr();
    char* readBuffer(); 
    void writeToReg(unsigned int, char);
    void setupProc();
    public:
    I2CDevice(int);
    I2CDevice(int, char);
    virtual ~I2CDevice();
};
