/*
 * anomaly_detection_util.cpp
 *
 * Author: Sapir Hender 208414573
 */
#include "anomaly_detection_util.h"
#include <cmath>

float average(float *x, int size) {
    if (size == 0) { return 0.0f; }
    float sum = 0;
    for (int i = 0; i <= size - 1; ++i) { sum += x[i]; }
    return sum / size;
}
// returns the variance of X and Y
float var(float *x, int size) {
    if (size == 0) { return 0.0f; }

    float experienceAverage = average(x, size);
    float sumOfDif = 0;
    for (int j = 0; j <= size - 1; ++j) { sumOfDif += pow((x[j] - experienceAverage), 2); }
    return sumOfDif / size;
}

// returns the covariance of X and Y
float cov(float *x, float *y, int size) {
    if (size == 0) { return 0.0f; }

    float xAverage = average(x, size);
    float yAverage = average(y, size);
    float xySum = 0;
    for (int i = 0; i <= size - 1; ++i) { xySum += (x[i] - xAverage) * (y[i] - yAverage); }
    return xySum / size;
}

// returns the Pearson correlation coefficient of X and Y
float pearson(float *x, float *y, int size) {
    float varX = var(x, size);
    float varY = var(y, size);
    if ((varX <= 0) || (varY <= 0)) { return 0.0f; }
    return cov(x, y, size) / (sqrt(varX) * sqrt(varY));
}

// performs a linear regression and return s the line equation
Line linear_reg(Point **points, int size) {
    float coordXArray[size];
    float coordYArray[size];
    for (int i = 0; i <= size - 1; ++i) {
        coordXArray[i] = points[i]->x;
        coordYArray[i] = points[i]->y;
    }
    float varXArray = var(coordXArray, size);
    if (varXArray == 0) { return Line(0, 0); }
    float a = cov(coordXArray, coordYArray, size) / varXArray;
    float b = average(coordYArray, size) - a * average(coordXArray, size);
    return Line(a, b);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point **points, int size) {
    Line l = linear_reg(points, size);
    return abs(p.y - l.f(p.x));
}

// returns the deviation between point p and the line
float dev(Point p, Line l) {
    float val = p.y - l.f(p.x);
    return val < 0 ? -1 * val : val;
}

//
///*
// * animaly_detection_util.cpp
// *
// *  Created on: 11 ����� 2020
// *      Author: Eli
// */
//
//#include <math.h>
//#include "anomaly_detection_util.h"
//
//float avg(float* x, int size){
//    float sum=0;
//    for(int i=0;i<size;sum+=x[i],i++);
//    return sum/size;
//}
//
//// returns the variance of X and Y
//float var(float* x, int size){
//    float av=avg(x,size);
//    float sum=0;
//    for(int i=0;i<size;i++){
//        sum+=x[i]*x[i];
//    }
//    return sum/size - av*av;
//}
//
//// returns the covariance of X and Y
//float cov(float* x, float* y, int size){
//    float sum=0;
//    for(int i=0;i<size;i++){
//        sum+=x[i]*y[i];
//    }
//    sum/=size;
//
//    return sum - avg(x,size)*avg(y,size);
//}
//
//
//// returns the Pearson correlation coefficient of X and Y
//float pearson(float* x, float* y, int size){
//    return cov(x,y,size)/(sqrt(var(x,size))*sqrt(var(y,size)));
//}
//
//// performs a linear regression and returns the line equation
//Line linear_reg(Point** points, int size){
//    float x[size],y[size];
//    for(int i=0;i<size;i++){
//        x[i]=points[i]->x;
//        y[i]=points[i]->y;
//    }
//    float a=cov(x,y,size)/var(x,size);
//    float b=avg(y,size) - a*(avg(x,size));
//
//    return Line(a,b);
//}
//
//// returns the deviation between point p and the line equation of the points
//float dev(Point p,Point** points, int size){
//    Line l=linear_reg(points,size);
//    return dev(p,l);
//}
//
//// returns the deviation between point p and the line
//float dev(Point p,Line l){
//    float x=p.y-l.f(p.x);
//    if(x<0)
//        x*=-1;
//    return x;
//}
//
//
//
//
