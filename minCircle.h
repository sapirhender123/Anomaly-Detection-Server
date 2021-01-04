// 208414573
#ifndef BUILDSTONE1_MINE_MINCIRCLE_H
#define BUILDSTONE1_MINE_MINCIRCLE_H

#include <array>
#include <iostream>
#include <iterator>
#include <limits>
#include <random>
#include <vector>
#include <algorithm>
#include "anomaly_detection_util.h"


using namespace std;

//// ------------ DO NOT CHANGE -----------
//class Point {
//public:
//    float x, y;
//    Point(float x, float y) : x(x), y(y) {}
//};

class Circle {
public:
    Point center;
    float radius;
    Circle(Point c, float r) : center(c), radius(r) {}
};
// --------------------------------------
Circle findMinCircle(Point** points,size_t size);


#endif//BUILDSTONE1_MINE_MINCIRCLE_H
