void DS3231::sysTimeRTCInit(){
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
