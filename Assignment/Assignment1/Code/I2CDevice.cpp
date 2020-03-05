#include "I2CDevice.h"

I2CDevice::I2CDevice(int bufSize) : buf(bufSize){
    addr = 0x00;
    setupProc();
}

I2CDevice::I2CDevice(int bufSize, char address) : 
    buf(bufSize), addr(address)
{
    setupProc();
}

I2CDevice::~I2CDevice(){
    close(file);
    printf("Destroyed\n");
}

int I2CDevice::openBus(){

    /** \brief Open the Bus connection to the I2C Device
     *
     */

    int file;
    if((file=open("/dev/i2c-1", O_RDWR)) < 0){
        perror("failed to open the bus\n");
        return 1;
    }
    return file;
}

void I2CDevice::connectDevice(){

    /** \brief Open the connection to the I2C Device
     *
     */

    if(ioctl(file, I2C_SLAVE, 0x68) < 0){
       perror("Failed to connect to the sensor\n");
       return;
    }
}

void I2CDevice::resetReadAddr(){

    /** \brief Set the initial read address for the I2C Device
     *
     */

    char writeBuffer[1] = {addr};
    if(write(file, writeBuffer, 1)!=1){
        perror("Failed to reset the read address\n");
        return;
    }
}

char* I2CDevice::readBuffer(){

    /** \brief Reads from the I2C devices registers
     *
     */

    char* buffer = &buf[0];
    if(read(file, buffer, buf.size())!=buf.size()){
        perror("Failed to read in the buffer\n");
        return buffer;
    }
    return buffer;
}

void I2CDevice::writeToReg(unsigned int reg, char val){

    /** \brief Write to the I2C Devices registers
     *
     */

    unsigned char writeBuffer[] = {reg, val};
    if(write(file, writeBuffer, 2)!=2){
        perror("Failed to write\n");
        return;
    }
}

void I2CDevice::setupProc(){

    /** \brief Setup Procedure for I2C Devices
     *
     *  Calls the functions required to open an I2C connection to
     * a device, and read the buffers of this device.
     */

    this->file = openBus();
    connectDevice();
    resetReadAddr();
    this->readBuffer();
}