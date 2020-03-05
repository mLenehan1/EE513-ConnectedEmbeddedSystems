void DS3231::writeTime(char* times){
    writeToReg(SEC_REG, decToBCD(times[0]));
    writeToReg(MIN_REG, decToBCD(times[1]));
    writeToReg(HR_REG, decToBCD(hrRegChangeUpperBits(times[2], 2)));
    return;
}

void DS3231::writeDate(char* date){
    if((date[0] > 0 && date[0] < 8) && date[1] <= 31){
        writeToReg(DAY_REG, decToBCD(date[0]));
        char dateVal = dateRegChangeUpperBits(date[1], 4);
        writeToReg(DATE_REG, decToBCD(dateVal));
        writeToReg(MTH_REG, decToBCD(date[2]));
        writeToReg(YR_REG, decToBCD(date[3]));
    }
    else perror("Incorrect Date Entered\n");
    return;
}
