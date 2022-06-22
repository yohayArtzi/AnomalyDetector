/*
 * Authors: Gidon Tobin 320518020, Yohay Artzi 208061911
 */
#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string>
#include <math.h>

struct correlatedFeatures{
    string feature1,feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
    float maxDev;
    string howToCheck;
    Point* center;
};

class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{

protected:
    vector<correlatedFeatures> cfs;
    float threshold;
public:
    SimpleAnomalyDetector();
    virtual ~SimpleAnomalyDetector();

    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    vector<correlatedFeatures> getNormalModel(){
        return cfs;
    }
    float getThreshold(){
        return this->threshold;
    }
    void setThreshold(float t){
        this->threshold = t;
    }
    virtual string fillWayOfChecking (float correlation) {
        if (correlation >= this->threshold)
            return "simple";
        return "no correlation";
    }
    virtual float fillMaxDev(float maxDev, string howToCheck, Point** points, int size) {
        return maxDev;
    }
    virtual Point* fillCenter(string howToCheck, Point** points, int size){
        return new Point(0,0);
    }
    Point** createPoints (vector<float> feature1, vector<float> feature2, int size);
    void addReport(vector<AnomalyReport>* reports, long timeStep, struct correlatedFeatures cf);
    void deletePoints(Point** points, int size);
};



#endif /* SIMPLEANOMALYDETECTOR_H_ */