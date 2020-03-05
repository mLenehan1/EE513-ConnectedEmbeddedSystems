#include "DS3231.h"

DS3231::DS3231() : I2CDevice(BUFFER_SIZE){}

DS3231::DS3231(char address) : I2CDevice(BUFFER_SIZE, address){}

DS3231::DS3231(bool setSystemTime) : I2CDevice(BUFFER_SIZE){
    if(setSystemTime){
        sysTimeRTCInit();
    }
}

// Converts from bcd to dec - used for reading time/date reg
int DS3231::bcdToDec(char b){ return (b/16)*10 + (b%16); }

// Converts from dec to bcd - used to write to time/date reg
int DS3231::decToBCD(char b){ return (b/10)*16 + (b%10); }

char DS3231::hrRegChangeUpperBits(char b, int reg){
    
    /** \brief Modify the upper nibble of the 1 byte 
     *  hour value - Does not affect the 12/24 or
     *  AM/PM bits
     *
     *  This function is used to modify the lower nibble
     *  of the "Hour" register, without unintended modifyications
     *  to the bits of the upper nibble.
     *  The 10/20 hour bits are modified as required to achieve the
     *  correct 24-hour time representation
     */

    char upperBuffNibble = (this->buf[reg] & 0xF0);
    char upperHrNibbleVal = (b & 0xF0) >> 4;
    if(upperHrNibbleVal == 0){
        upperBuffNibble &= ~(1UL << 4);
        upperBuffNibble &= ~(1UL << 5);
    } else if(upperHrNibbleVal == 1){
        upperBuffNibble |= (1UL << 4);
        upperBuffNibble &= ~(1UL << 5);
    }
    else if(upperHrNibbleVal == 2){
        upperBuffNibble &= ~(1UL << 4);
        upperBuffNibble |= (1UL << 5);
    }
    else{
        perror("Invalid Hour Time Entered\n");
        return '\0';
    }
    char lowerHrNibble = b & 0x0F;
    return upperBuffNibble | lowerHrNibble;
}

char DS3231::dateRegChangeUpperBits(char b, int reg){

    /** \brief Modify the upper nibble of the 1 byte 
     *  date value.
     *
     *  This function is used to modify the lower nibble
     *  of the "Date" register, without unintended modifyications
     *  to the bits of the upper nibble.
     *  The 10 date bits are modified as required to achieve the
     *  correct 24-hour time representation
     */
     
    char upperBuffNibble = (this->buf[reg] & 0xF0);
    char upperDateNibbleVal = (b & 0xF0) >> 4;
    if(upperDateNibbleVal == 0){
        upperBuffNibble &= ~(1UL << 4);
        upperBuffNibble &= ~(1UL << 5);
    } else if(upperDateNibbleVal == 1){
        upperBuffNibble |= (1UL << 4);
        upperBuffNibble &= ~(1UL << 5);
    } else if(upperDateNibbleVal == 2){
        upperBuffNibble &= ~(1UL << 4);
        upperBuffNibble |= (1UL << 5);
    } else if(upperDateNibbleVal ==3){
        upperBuffNibble |= (1UL << 4);
        upperBuffNibble |= (1UL << 5);
    }
    else{
        perror("Invalid Date Entered\n");
        return '\0';
    }
    char lowerDateNibble = b & 0x0F;
    return upperBuffNibble | lowerDateNibble;
}

char DS3231::changeBits(bool set, int reg, int pos){

    /** \brief Set/Clear a bit in the required position
     *  in the required register.
     *
     *  The boolean "set" value is checked for set/clear
     *  operations. Using bitwise operations, the bit in
     *  position "pos" in register "reg" is either set or 
     *  cleared.
     */

    if(set){
        return buf[reg] |= 1UL << pos;
    } else{
        return buf[reg] &= ~(1UL << pos);
    }
}

void DS3231::sysTimeRTCInit(){

    /** \brief Sets the RTC time based on the output of the
     *  Linux "date" utility.
     *
     *  This function creates a temporary file, runs the command
     *  line "date" utility - formatting to give time (sec min hr)
     *  and date (day, date, month, year), and stores the output
     *  in the temporary file. This file is read into a string vector
     *  and the temporary file is closed and deleted. The time values
     *  and date values are added to separate char arrays for writing
     *  to the RTC.
     */

    std::string line;
    std::vector<std::string> tmpVals;
    std::ofstream outfile ("tmp");
    outfile.close();
    std::ifstream infile("tmp");
    system("date '+%S %M %H %u %d %m %Y'>> tmp");
    if(infile.is_open()){
        while(getline(infile, line, ' ')){
            tmpVals.push_back(line);
        }
    }
    infile.close();
    remove("tmp");
    char time[3];
    char date[4];
    for(int i=0; i<3; i++){
        time[i] = stoi(tmpVals[i]);
    }
    for(int i=0; i<3; i++){
        date[i] = stoi(tmpVals[i+3]);
    }
    date[3] = stoi(tmpVals[6].substr(2,3));
    writeTime(time);
    writeDate(date);
}

void DS3231::readTimeAndDate(){

    /** \brief Print the RTC Time and Date
     *  
     *  Prints the values stored in registers 2, 1, and 0 (Hours,
     *  Minutes, Seconds), followed by registers 3, 4, 5, and 6 (
     *  Day, Date, Month, Year).
     */

    printf("The RTC time is %02d:%02d:%02d\n", bcdToDec(buf[2]),
        bcdToDec(buf[1]), bcdToDec(buf[0]));
    printf("The date is %02d - %02d/%02d/%02d\n",
    bcdToDec(buf[3]), bcdToDec(buf[4]), bcdToDec(buf[5]), bcdToDec(buf[6]));
    return;
}

void DS3231::writeTime(char* times){

    /**\brief Write the input times to the RTC time registers
     *
     *  Takes a character array containing "time" values to be
     *  written. Calls the "writeToReg" function to write the
     *  required value to the required registers.
     *  Defined values are used to reference the second, minute
     *  and hour registers (0x00, 0x01, and 0x02).     
     */

    writeToReg(SEC_REG, decToBCD(times[0]));
    writeToReg(MIN_REG, decToBCD(times[1]));
    writeToReg(HR_REG, decToBCD(hrRegChangeUpperBits(times[2], 2)));
    return;
}

void DS3231::writeDate(char* date){

    /**\brief Write the input dates to the RTC date registers
     *
     *  Takes a character array containing "date" values to be
     *  written. Calls the "writeToReg" function to write the
     *  required value to the required registers.
     *  Defined values are used to reference the day, date, month.
     *  and year registers (0x03, 0x04, 0x05 and 0x06).  
     *  A check is performed to ensure the date is within the
     *  correct range   
     */

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

void DS3231::readAlarms(){

    /** \brief Print the RTC Alarms
     *  
     *  Prints the values stored in the Alarm 1 and Alarm 2
     *  registers (0x07 - 0x0D).
     */

    printf("Alarm 1 time is %02d:%02d:%02d on %02d\n", 
    bcdToDec(buf[9]), bcdToDec(buf[8]), bcdToDec(buf[7]), bcdToDec(buf[10]));
    printf("Alarm 2 time is %02d:%02d on %02d\n", 
    bcdToDec(buf[12]), bcdToDec(buf[11]), bcdToDec(buf[13]));
    return;
}

void DS3231::setAlarms(char* times){

    /** \brief Set the RTC Alarm Times/Dates
     *
     *  Writes to the Alarm registers (0x07 - 0x0D). Sets all
     *  "Alarm Mask Bits" to 0 - Alarm will trigger when date, hours,
     *  minutes and seconds match
     */

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

void DS3231::readTemp(){

    /** \brief Print the RTC temperature
     *  
     *  Temperature has a precision of 0.25 degrees.
     *  The upper part of the temperature is in reg 1
     *  The lower part of the temperature is in tbe
     *  upper two bits of reg 2
    */

    int tempFrac = 100 * (buf[18] >> 6)/4; // Shift to extract upper 2 bits, temp is a frac of 100.
    printf("The Temperature is %02d.%02d\n", buf[17],
        tempFrac);
    return;
}

void DS3231::setInterrupt(bool alarm1, bool alarm2){

    /** \brief Set the RTC Alarm Interrupt
     *
     *  Takes two boolean values representing the status of
     *  alarms 1 and 2. If either are to be set, the interrupt
     *  is set, along with the corresponding alarm enable bit.
     *  If neither are set, the interrupt bit is cleared, i.e. the
     *  Square Wave Generator is enabled.
     */

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

void DS3231::sqWaveGen(int freqOption, bool stopInterrupt){

    /** \brief Enable the Square Wave Generator at the desired
     *  frequency
     *
     *  Takes an integer value representing one of the four
     *  available frequencies of the square wave generator.
     *  A boolean represents the toggling of the interrupt bit,
     *  which enables the generator is set low.
     *  Each case of the switch statement performs the required
     *  bit modifications for the "Rate Select" bits of register
     *  0x0E. 
     */
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
    return;
}

void DS3231::clearAlarms(){

    /** \brief Clear the RTC Alarm Flags
     *
     *  Clears the Alarm 1 and Alarm 2 Flag bits of register 0x0F.
     */

    changeBits(false, 15, 0);
    changeBits(false, 15, 1);
    writeToReg(STAT, buf[15]);
    return;
}