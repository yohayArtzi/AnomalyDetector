/*
 *
 * Authors: Gidon Tobin 320518020, Yohay Artzi 208061911
 */
#include "HybridAnomalyDetector.h"
#include "minCircle.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
    // TODO Auto-generated constructor stub
}

HybridAnomalyDetector::~HybridAnomalyDetector() {
    // TODO Auto-generated constructor stub
}

vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries &ts) {

    map<string, vector<float>> features_map = ts.getMap();
    int size = ts.length();

    vector<AnomalyReport> simpleReports = SimpleAnomalyDetector::detect(ts);
    vector<AnomalyReport> *hybridReports = &simpleReports;

    // check each pair that was found as cor in learnNormal function
    for (correlatedFeatures cf: this->cfs) {

        // skip simple anomalies
        if (cf.howToCheck == "simple")
            continue;

        // make array of points
        Point **points = createPoints(features_map[cf.feature1], features_map[cf.feature2],
                                      size);

        // find anomalies and report
        for (int i = 0; i < size; i++) {
            float dev = (*points[i]).distance(*cf.center);
            if (dev > cf.maxDev)
                addReport(hybridReports, (long) i + 1, cf);
        }
        deletePoints(points, size);
        delete cf.center;
        cf.center = NULL;
    }
    return *hybridReports;
}
