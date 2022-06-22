/*
 *
 * Authors: Gidon Tobin 320518020, Yohay Artzi 208061911
 */

#include "anomaly_detection_util.h"

float avg(float *x, int size) {
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum = sum + x[i];
    }
    return sum / size;
}

// returns the variance of X
float var(float *x, int size) {
    float a = avg(x, size);
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum = sum + (x[i] * x[i]);

    }
    return sum / size - (a * a);
}

// returns the covariance of X and Y
float cov(float *x, float *y, int size) {
    float result = 0;
    for(int i=0; i < size; i++){
        result = result + (x[i]*y[i]);
    }
    result = result/size;
    return result - (avg(y,size)*avg(x,size));
}

// returns the Pearson correlation coefficient of X and Y
float pearson(float *x, float *y, int size) {
    float sqX = sqrtf(var(x, size));
    float sqY = sqrtf(var(y, size));
    return cov(x, y, size) / (sqX * sqY);
}


// performs a linear regression and returns the line equation
Line linear_reg(Point **points, int size) {
    float x[size];
    float y[size];
    for (int i = 0; i < size; i++) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    float a = cov(x, y, size) / var(x, size);
    float b = avg(y, size) - (a * avg(x, size));

    return Line(a, b);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point **points, int size) {
    Line reg = linear_reg(points, size);
    if (((reg.a * p.x + reg.b) - p.y) >= 0)
        return (reg.a * p.x + reg.b) - p.y;
    else
        return -((reg.a * p.x + reg.b) - p.y);
}

// returns the deviation between point p and the line
float dev(Point p, Line l) {
    if (((l.a * p.x + l.b) - p.y) >= 0)
        return (l.a * p.x + l.b) - p.y;
    else
        return -((l.a * p.x + l.b) - p.y);
}
