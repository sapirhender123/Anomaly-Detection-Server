// 208414573
#ifndef BUILDSTONE1_MINCIRCLE_H
#define BUILDSTONE1_MINCIRCLE_H

#include <array>
#include <iostream>
#include <iterator>
#include <limits>
#include <random>
#include <vector>
#include <algorithm>

using namespace std;

// ------------ DO NOT CHANGE -----------
class Point {
public:
    float x, y;
    Point(float x, float y) : x(x), y(y) {}
};

class Circle {
public:
    Point center;
    float radius;
    Circle(Point c, float r) : center(c), radius(r) {}
};
// --------------------------------------

const double EPSILON = 1 + 1e-14;

///function that return the distance between two points
float dist(const Point &a, const Point &b) { return std::hypot(a.x - b.x, a.y - b.y); }


/// function that checks if the point inside or on the boundaries of the circle
bool ifInsideCircle(const Circle &c, const Point &point) {
    return dist(c.center, point) <= c.radius * EPSILON;
}


/// Helper function to calculate the point that create from subtract og two points
Point subtractTwoPoints(Point point1, Point point2) {
    return {point1.x - point2.x, point1.y - point2.y};
}


/// function that creates circle from 3 points
Circle circleFromThreePoints(const Point &X, const Point &Y, const Point &Z) {
    // subtract Y from X - according to equation
    Point point1 = subtractTwoPoints(Y, X);
    // subtract Z from X - according to equation
    Point point2 = subtractTwoPoints(Z, X);
    // find the center of the circle that created from 2 points
    float powP1 = point1.x * point1.x + point1.y * point1.y;
    float powP2 = point2.x * point2.x + point2.y * point2.y;
     // find the vector multiplication in order to calculate the center point
    float vecMult = point1.x * point2.y - point1.y * point2.x;

    Point center = {(point2.y * powP1 - point1.y * powP2) / (2 * vecMult),
                    (point1.x * powP2 - point2.x * powP1) / (2 * vecMult)};

    center.x = center.x + X.x;
    center.y = center.y + X.y;
    return {center, dist(center, X)};
}


///function that calculates the circle of two points while the points is the Diameter of the circle
Circle twoPointsDiameter(const Point &point1, const Point &point2) {
    Point center{(point1.x + point2.x) / 2.0f, (point1.y + point2.y) / 2.0f};
    return Circle{center, dist(point1, point2) / 2.0f};
}


/// function that check if the circle encloses all of the points
bool ifCircleEnclose(const Circle &c, const vector<const Point *> &P) {
    // check for each point
    for (auto p : P)
        if (!ifInsideCircle(c, *p)) return false;
    return true;
}

/// function that get the smallest circle for size smaller or equal from 3 points
Circle getBasicMinCircle(std::vector<const Point *> points) {
    if (points.size() <= 3) {
        // if there is no points
        if (points.empty()) {
            return {{0, 0}, 0};
        } else if (points.size() == 1) { // of there is only one point
            return {*points[0], 0};
        } else if (points.size() == 2) { // if there are two points
            return twoPointsDiameter(*points[0], *points[1]);
        }
        // check if the circle can be from 2 points
        for (int i = 0; i < 3; i++) {
            for (int j = i + 1; j < 3; j++) {
                Circle circle = twoPointsDiameter(*points[i], *points[j]);
                // check if circle encloses each one of the points
                if (ifCircleEnclose(circle, points)) return circle;
            }
        }
        // if it can not be creates from only two points
        return circleFromThreePoints(*points[0], *points[1], *points[2]);

    } else return {{0, 0}, -1}; // if there is a mistake, return an illegal circle
}


/// function that return the minimal circle of Welzl's algorithm
Circle useWelzlAlgo(std::vector<const Point *> P, std::vector<const Point *> R, size_t n) {
    // Base case when all points processed or |R| = 3
    if (n == 0 || R.size() == 3) { return getBasicMinCircle(R); }
    // get random point
    int idx = rand() % n;
    const Point *p = P[idx];
    // instead of deleting the point, move it to the end of the vector and then in the next
    // time the function will ignore it and execute without it
    swap(P[idx], P[n - 1]);
    // find the minimum circle without the point that I explained before
    Circle minCircle = useWelzlAlgo(P, R, n - 1);
    // If the minimum Circle contains p, return minCircle
    if (ifInsideCircle(minCircle, *p)) { return minCircle; }
    // if the circle is not in the circle, it have to be on the boundary
    R.push_back(p);
    // now, I will return the minimum circle of the n-1 points (recursively)
    return useWelzlAlgo(P, R, n - 1);
}

///find the minimal circle using Welzl's algorithm
Circle findMinCircle(Point **points, size_t size) {
    vector<const Point *> pointsCopy;
    pointsCopy.reserve(size);
    for (int i = 0; i < size; i++) { pointsCopy.push_back(points[i]); }
    // pick a random generator
    std::random_device rd;
    std::mt19937 g(rd());
    // shuffle the points
    std::shuffle(pointsCopy.begin(), pointsCopy.end(), g);
    // create initially an empty vector to the points that on the boundary of the circle
    std::vector<const Point *> pointsOnBound;
    // use the Welzl's algorithm recursively
    return useWelzlAlgo(pointsCopy, pointsOnBound, pointsCopy.size());
}

#endif//BUILDSTONE1_MINCIRCLE_H
