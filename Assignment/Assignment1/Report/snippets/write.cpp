void I2CDevice::writeToReg(unsigned int reg, char val){
    unsigned char writeBuffer[] = {reg, val};
    if(write(file, writeBuffer, 2)!=2){
        perror("Failed to write\n");
        return;
    }
}
