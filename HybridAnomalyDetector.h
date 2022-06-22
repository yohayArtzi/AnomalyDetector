/*
 *
 * Authors: Gidon Tobin 320518020, Yohay Artzi 208061911
 */

#ifndef ADVANCED_PROGRAMMING_1_HYBRIDANOMALYDETECTOR_H
#define ADVANCED_PROGRAMMING_1_HYBRIDANOMALYDETECTOR_H

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector : public SimpleAnomalyDetector{
public:
    HybridAnomalyDetector();
    virtual ~HybridAnomalyDetector();
    virtual vector<AnomalyReport> detect(const TimeSeries& ts) override;
    virtual string fillWayOfChecking (float correlation) override{
        if (correlation < threshold)
            return "hybrid";
        return "simple";
    }
    virtual float fillMaxDev(float maxDev, string howToCheck, Point** points, int size) override{
        if (howToCheck == "simple")
            return maxDev;

        // find max deviation
        Circle c = findMinCircle(points, size_t(size));
        return c.radius*1.1;
    }
    virtual Point* fillCenter(string howToCheck, Point** points, int size) override{
        if (howToCheck == "simple")
            return new Point(0,0);
        Point center = findMinCircle(points, size_t(size)).center;
        return new Point(center.x, center.y);
    }
};


#endif //ADVANCED_PROGRAMMING_1_HYBRIDANOMALYDETECTOR_H
