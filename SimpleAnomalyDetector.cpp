/*
 *
 * Authors: Gidon Tobin 320518020, Yohay Artzi 208061911
 */
#include "SimpleAnomalyDetector.h"
#include "anomaly_detection_util.h"
#include "timeseries.h"
#include <map>
#include <vector>
#include <iostream>

using std::vector;
using std::map;
using std::string;

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    // TODO Auto-generated constructor stub
    this->threshold = 0.9;

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    // TODO Auto-generated destructor stub
}
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
    map<string, vector<float>> map_elements = ts.getMap();
    vector<string> keys = ts.get_features();
    int num_of_rows = ts.length();
    float minThreshold = 0.5;

    vector<string>::iterator header;
    for (header = keys.begin(); header != keys.end(); header++) {
        float maxCor = 0;
        string corTo = "";

        vector<string>::iterator next = header;
        for (next++; next != keys.end(); next++) {
            float cor = pearson((map_elements[header->data()]).data(),
                                (map_elements[next->data()]).data(), num_of_rows);
            if (cor < 0)
                cor = -cor;

            if (cor > maxCor && cor > minThreshold) {
                maxCor = cor;
                corTo = next->data();
            }
        }
        if (corTo == "")
            continue;

        Point **points = new Point *[num_of_rows];
        for (int i = 0; i < num_of_rows; i++) {
            points[i] = new Point((map_elements[header->data()])[i],
                                  map_elements[corTo][i]);
        }

        float maxDev = 0;
        Line lin_reg = linear_reg(points, num_of_rows);
        for (int x = 0; x < num_of_rows; x++) {
            float getDev = dev(*points[x], lin_reg);
            if (getDev > maxDev)
                maxDev = getDev;
        }
        correlatedFeatures corOfI;
        corOfI.feature1 = header->data();
        corOfI.feature2 = corTo;
        corOfI.corrlation = maxCor;
        corOfI.lin_reg = lin_reg;
        corOfI.threshold = minThreshold;
        corOfI.howToCheck = fillWayOfChecking(maxCor);
        corOfI.maxDev = fillMaxDev(maxDev, corOfI.howToCheck, points, num_of_rows);
        corOfI.center = fillCenter(corOfI.howToCheck, points, num_of_rows);
        this->cfs.push_back(corOfI);

        // remember to delete points: all points and finally array
        deletePoints(points, num_of_rows);
    }
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts) {

    map<string,vector<float>> map_elements = ts.getMap();
    int num_of_rows = ts.length();
    vector<AnomalyReport>* reports = new vector<AnomalyReport>;

    //check each pair that was found as cor in learnNormal function
    for(correlatedFeatures cf : this->cfs){

        // if not simple anomaly check
        if (cf.howToCheck != "simple")
            continue;

        //make array of points
        Point** points = createPoints(map_elements[cf.feature1], map_elements[cf.feature2],
                                      num_of_rows);

        // find anomalies and report
        float normalDev = cf.maxDev * 1.1;
        for(int i = 0; i < num_of_rows; i++){
            float pointDev = dev(*points[i], cf.lin_reg);
            if(pointDev >= normalDev)
                addReport(reports, (long)i + 1, cf);
        }
        deletePoints(points, num_of_rows);
        delete cf.center;
        cf.center = NULL;
    }
    return *reports;
}

Point** SimpleAnomalyDetector::createPoints(vector<float> feature1, vector<float> feature2, int size){

    Point** points = new Point* [size];
    for(int i = 0; i < size; i++) {
        points[i] = new Point(feature1[i], feature2[i]);
    }
    return points;
}

void SimpleAnomalyDetector::addReport(vector<AnomalyReport>* reports, long timeStep, correlatedFeatures cf){

    const string des = cf.feature1 + "-" + cf.feature2;
    AnomalyReport* ar = new AnomalyReport(des, timeStep);
    (*reports).push_back(*ar);

    return;
}

void SimpleAnomalyDetector::deletePoints(Point** points, int size) {
    for (int i = 0; i < size; i++) {
        delete points[i];
    }
    delete[] points;
}