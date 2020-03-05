void I2CDevice::setupProc(){
    this->file = openBus();
    connectDevice();
    resetReadAddr();
    this->readBuffer();
}
