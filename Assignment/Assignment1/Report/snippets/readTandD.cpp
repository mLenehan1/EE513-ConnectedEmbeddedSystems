void DS3231::readTimeAndDate(){
    printf("The RTC time is %02d:%02d:%02d\n", bcdToDec(buf[2]),
        bcdToDec(buf[1]), bcdToDec(buf[0]));
    printf("The date is %02d - %02d/%02d/%02d\n",
    bcdToDec(buf[3]), bcdToDec(buf[4]), bcdToDec(buf[5]), bcdToDec(buf[6]));
    return;
}
