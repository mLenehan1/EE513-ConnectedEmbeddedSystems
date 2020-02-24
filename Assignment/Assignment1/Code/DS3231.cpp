#include "DS3231.h"

using namespace std;

    int bcdToDec(char b){
        return (b/16)*10 + (b%16);
        }

    int openBus(int file){
        if((file=open("/dev/i2c-2", O_RDWR)) < 0){
            perror("failed to open the bus\n");
            return 1;
        }
        return file;
    }

    int connectSlave(int file){
        if(ioctl(file, I2C_SLAVE, 0x68) < 0){
            perror("Failed to connect to the sensor\n");
            return 1;
        }
        return 0;
    }

    int resetReadAddress(int file, char addr){
        char writeBuffer[1] = {addr};
        if(write(file, writeBuffer, 1)!=1){
            perror("Failed to reset the read address\n");
            return 1;
        }
        return 0;
    }

    int readBuffer(int file, char* buf){
        if(read(file, buf, BUFFER_SIZE)!=BUFFER_SIZE){
            perror("Failed to read in the buffer\n");
            return 1;
        }
        return 0;
    }
    
    DS3231::DS3231(){}
    
    int readTimeAndDate(){
        char buf[BUFFER_SIZE];
        openBus(file);
        connectSlave(file);
        resetReadAddress(file, {0x00});
        readBuffer(file, buf);
        printf("The RTC time is %02d:%02d:%02d\n", bcdToDec(buf[2]),
        bcdToDec(buf[1]), bcdToDec(buf[0]));
        close(file);
        return 0;
    }
    
    int setTimeAndDate(){
        return 0;
    }
    
    int readTemp(){
        char buf[BUFFER_SIZE];
        openBus(file);
        connectSlave(file);
        resetReadAddress(file, {0x10});
        readBuffer(file, buf);
        printf("The Temperature is %02d.%02d\n", bcdToDec(buf[2]),
        bcdToDec(buf[1]));
        close(file);
        return 0;
    }
    
    int readAlarms(){
        char buf[BUFFER_SIZE];
        printf("An Alarm is set for %02d:%02d:%02d\n", bcdToDec(buf[9]),
        bcdToDec(buf[8]), bcdToDec(buf[7]));
        printf("An Alarm is set for %02d:%02d\n", bcdToDec(buf[12]),
        bcdToDec(buf[11]));
        close(file);
        return 0;
    }
    
    int setAlarms(){
        return 0;
    }