/*
 *
 * Authors: Gidon Tobin 320518020, Yohay Artzi 208061911
 */
#include "timeseries.h"
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

TimeSeries::TimeSeries(const char* CSVfileName) {

    fileName = CSVfileName;
    ifstream inFileStream(fileName);
    string strLine, feature;
    char delim = ',';
    getline(inFileStream, strLine);
    istringstream issLine(strLine);

    while (getline(issLine, feature, delim)) {
        vector<float> values;
        features.push_back(feature);
        featuresMap[feature] = values;

    }

    while (getline(inFileStream, strLine)) {
        istringstream issLine(strLine);
        string strValue;
        int i = 0;
        while (getline(issLine, strValue, delim)) {
            featuresMap[features.at(i)].push_back(stof(strValue));
            i++;
        }
    }
}

map<string,vector<float>>TimeSeries::getMap()const{
    return featuresMap;
}

int TimeSeries::size() const{
    return features.size();
}

int TimeSeries::length() const{
    return featuresMap.begin()->second.size();
}

vector<string> TimeSeries:: get_features() const{
    return features;
}