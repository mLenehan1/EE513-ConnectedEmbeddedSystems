void DS3231::setInterrupt(bool alarm1, bool alarm2){
    if(alarm1 || alarm2){
        changeBits(true, 14, 2);
        if(alarm1){ changeBits(true, 14, 0); }
        else{ changeBits(false, 14, 0); }
        if(alarm2){ changeBits(true, 14, 1); }
        else{ changeBits(false, 14, 1); }

    }
    else{
        changeBits(false, 14, 2);
    }
    writeToReg(CNTL, buf[14]);
    return;
}
