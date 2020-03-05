void DS3231::sqWaveGen(int freqOption, bool stopInterrupt){
    if(stopInterrupt){
        changeBits(false, 14, 2);
    }
    switch(freqOption){
        case 1:
            changeBits(false, 14, 3);
            changeBits(false, 14, 4);
            break;
        case 2:
            changeBits(true, 14, 3);
            changeBits(false, 14, 4);
            break;
        case 3:
            changeBits(false, 14, 3);
            changeBits(true, 14, 4);
            break;
        case 4:
            changeBits(true, 14, 3);
            changeBits(true, 14, 4);
            break;
        default:
            printf("Invalid Option\n");
            break;
    }
    writeToReg(CNTL, buf[14]);
    printf("%02d\n", buf[14]);
    return;
}
