int DS3231::decToBCD(char b){ return (b/10)*16 + (b%10); }
