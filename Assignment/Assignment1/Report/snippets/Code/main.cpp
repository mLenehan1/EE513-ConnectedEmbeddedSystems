#include "DS3231.h"

int main(){
    DS3231 test(true);
    test.readTimeAndDate();
    DS3231 test2;
    test2.readTimeAndDate();
}