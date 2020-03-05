void DS3231::setAlarms(char* times){
    writeToReg(A1_SEC, decToBCD((times[0] &= ~(1UL << 7))));
    writeToReg(A1_MIN, decToBCD((times[1] &= ~(1UL << 7))));
    char hr = hrRegChangeUpperBits(times[2], 9);
    hr &= ~(1UL << 7);
    writeToReg(A1_HR, decToBCD(hr));
    char dy = dateRegChangeUpperBits(times[3], 10);
    dy &= ~(1UL << 7);
    writeToReg(A1_DAY, decToBCD(dy));
    writeToReg(A2_MIN, decToBCD((times[4]) &= ~(1UL << 7)));
    char hr2 = hrRegChangeUpperBits(times[5], 12);
    hr2 &= ~(1UL << 7);
    writeToReg(A2_HR, decToBCD(hr2));
    char dy2 = dateRegChangeUpperBits(times[6], 13);
    dy2 &= ~(1UL << 7);
    writeToReg(A2_DAY, decToBCD(dy2));
    return;
}
