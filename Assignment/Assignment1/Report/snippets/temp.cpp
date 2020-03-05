void DS3231::readTemp(){
    int tempFrac = 100 * (buf[18] >> 6)/4; // Shift to extract upper 2 bits, temp is a frac of 100.
    printf("The Temperature is %02d.%02d\n", buf[17],
        tempFrac);
    return;
}
