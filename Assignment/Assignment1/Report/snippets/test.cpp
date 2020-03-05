#include "DS3231.h"

int main(){
    char address = 0x00;
    DS3231 testObject(address);
    char times[] = {20, 30, 12};
    char alarms[] = {30, 30, 12, 2, 30, 12, 2};
    char date[] = {1, 2, 3, 20};
    testObject.clearAlarms();
    printf("Starting the DS3231 test application\n");
    printf("------------------------------------\n");
    printf("Test 1: Read Time and Date:\n");
    testObject.readTimeAndDate();
    printf("------------------------------------\n");
    printf("Test 2: Write Time (12:30:20):\n");
    testObject.writeTime(times);
    usleep(100000);
    testObject.readTimeAndDate();
    printf("------------------------------------\n");
    printf("Test 3: Write Date (13/03/20):\n");
    testObject.writeDate(date);
    usleep(100000);
    testObject.readTimeAndDate();
    printf("------------------------------------\n");
    printf("Test 4: Read Alarms:\n");
    testObject.readAlarms();
    printf("------------------------------------\n");
    printf("Test 5: Write Alarms (30th @ 12:30:30,\n 10th @ 11:12):\n");
    testObject.setAlarms(alarms);
    usleep(100000);
    testObject.readAlarms();
    printf("------------------------------------\n");
    printf("Test 6: Set Interrupt:\n");
    testObject.setInterrupt(true, true);
    printf("NOTICE: Please visually verify alarm is set!\n");
    usleep(15000000);
    printf("------------------------------------\n");
    printf("Test 7: Square Wave Generator:\n");
    testObject.sqWaveGen(1, true);
    printf("------------------------------------\n");
    printf("Test 8: Read Temperature:\n");
    testObject.readTemp();
    printf("------------------------------------\n");
    printf("------------------------------------\n");
    DS3231 testObject2;
    testObject2.readTimeAndDate();
    testObject2.readAlarms();
    testObject2.sysTimeRTCInit();
}