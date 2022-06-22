/*
 *
 * Authors: Gidon Tobin 320518020, Yohay Artzi 208061911
 */
#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <vector>
#include <map>
#include <string>

using namespace std;

class TimeSeries {

    const char* fileName;
    map<string, vector<float>> featuresMap;
    vector<string> features;

public:
    TimeSeries(const char *CSVfileName);
    map<string, vector<float>> getMap()const;
    int size() const;
    int length() const;
    vector<string> get_features() const;
};


#endif /* TIMESERIES_H_ */
