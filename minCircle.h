// ID1	ID2

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <array>
#include <iostream>
#include <iterator>
#include <limits>
#include <random>
#include <vector>


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

// you may add helper functions here

///function that return the distance between two points
float dist(const Point &a, const Point &b) {
    return std::hypot(a.x - b.x, a.y - b.y);
}

const double EPSILON = 1 + 1e-14;

/// function that checks if the point inside or on the boundaries of the circle
bool ifInsideCircle(const Circle &c, const Point &point) {
    return dist(c.center, point) <= c.radius * EPSILON;
}

// todo explain
Circle circleFromThree(const Point &a, const Point &b, const Point &c) {

        float x1 = a.x;
        float y1 = a.y;

        float x2 = b.x;
        float y2 = b.y;

        float x3 = c.x;
        float y3 = c.y;


        float x12 = x1 - x2;
        float x13 = x1 - x3;

        float y12 = y1 - y2;
        float y13 = y1 - y3;

        float y31 = y3 - y1;
        float y21 = y2 - y1;

        float x31 = x3 - x1;
        float x21 = x2 - x1;

        // x1^2 - x3^2
        float sx13 = pow(x1, 2) - pow(x3, 2);

        //y1^2 - y3^2
        float sy13 = pow(y1, 2) - pow(y3, 2);

        float sx21 = pow(x2, 2) - pow(x1, 2);
        float sy21 = pow(y2, 2) - pow(y1, 2);

        float f = ((sx13) * (x12)
                 + (sy13) * (x12)
                 + (sx21) * (x13)
                 + (sy21) * (x13))
                / (2 * ((y31) * (x12) - (y21) * (x13)));
        float g = ((sx13) * (y12)
                 + (sy13) * (y12)
                 + (sx21) * (y13)
                 + (sy21) * (y13))
                / (2 * ((x31) * (y12) - (x21) * (y13)));

        float c1 = -pow(x1, 2) - pow(y1, 2) - 2 * g * x1 - 2 * f * y1;

        // eqn of circle be x^2 + y^2 + 2*g*x + 2*f*y + c = 0
        // where centre is (h = -g, k = -f) and radius r
        // as r^2 = h^2 + k^2 - c
        float h = -g;
        float k = -f;
        float sqr_of_r = h * h + k * k - c1;

        // r is the radius
        float r = sqrt(sqr_of_r);
        return {Point(h, k), r};

    }
//    double ox = (min(min(a.x, b.x), c.x) + max(max(a.x, b.x), c.x)) / 2;
//    double oy = (min(min(a.y, b.y), c.y) + max(max(a.y, b.y), c.y)) / 2;
//    double ax = a.x - ox, ay = a.y - oy;
//    double bx = b.x - ox, by = b.y - oy;
//    double cx = c.x - ox, cy = c.y - oy;
//    double d = (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by)) * 2;
//    if (d == 0) return {Point(0, 0), 0};
//    double x = ((ax * ax + ay * ay) * (by - cy) + (bx * bx + by * by) * (cy - ay) +
//                (cx * cx + cy * cy) * (ay - by)) /
//               d;
//    double y = ((ax * ax + ay * ay) * (cx - bx) + (bx * bx + by * by) * (ax - cx) +
//                (cx * cx + cy * cy) * (bx - ax)) /
//               d;
//    Point p{(float) (ox + x), (float) (oy + y)};
//    float r = max(max(dist(p, a), dist(p, b)), dist(p, c));
//    return Circle{p, r};
//}

Point subtractTwoPoints(Point point1, Point point2) {
    return {point1.x - point2.x, point1.y - point2.y};
}

float vecMult(Point point1, Point point2) { return point1.x * point2.y - point2.x * point1.y; }

///function that calculates the circle of two points while the points is the Diameter of the circle
Circle twoPointsDiameter(const Point &point1, const Point &point2) {
    Point center{(point1.x + point2.x) / 2.0f, (point1.y + point2.y) / 2.0f};
    return Circle{center, max(dist(point1, center), dist(point2, center))};
}

Circle minCircleFromTwo(const Point &point1, const Point &point2, size_t size, Point **points) {
    // initialize left circle
    Circle leftCircle = Circle(Point(0, 0), -1);
    // initialize right circle
    Circle rightCircle = Circle(Point(0, 0), -1);
    // initialize circle from two points
    Circle circleFromTwoPoints = twoPointsDiameter(point1, point2);
    // calculate point that created from subtraction of two points
    Point subp1p2 = subtractTwoPoints(point2, point1);
    //Circle circleFromThreePoints = {{0, 0}, -1};
    // for each point in the array, I will check if it inside the circle
    for (int i = 0; i < size; ++i) {
        const Point point3 = *points[i];
        if (ifInsideCircle(circleFromTwoPoints, point3)) { continue; }
        // calculate point that created from subtraction of two points
        Point subp3p1 = subtractTwoPoints(point3, point1);
        // calculate the vector multiplication
        //cross
        double vecMultipication = vecMult(subp1p2, subp3p1);
        // calculate the circle that made from three points
        Circle circleFromThreePoints = circleFromThree(point1, point2, point3);
        // if the circle was just initialized
        if (circleFromThreePoints.radius < 0) continue;
        // decide which circle is right circle and which is left
        else if (vecMultipication > 0.0f &&
                 ((leftCircle.radius < 0.0f ||
                   vecMult(subp1p2, subtractTwoPoints(circleFromThreePoints.center, point1)) >
                           vecMult(subp1p2, subtractTwoPoints(leftCircle.center, point1))))) {
            leftCircle = circleFromThreePoints;
        } else if (vecMultipication < 0.0f &&
                   ((rightCircle.radius < 0 ||
                     vecMult(subp1p2, subtractTwoPoints(circleFromThreePoints.center, point1)) <
                             vecMult(subp1p2, subtractTwoPoints(rightCircle.center, point1))))) {
            rightCircle = circleFromThreePoints;
        }

        // now, I'm comparing between the circles (according to the minimal radius).
        if (leftCircle.radius < 0 && rightCircle.radius < 0) return circleFromTwoPoints;
        // if the left circle is illegal
        else if (leftCircle.radius < 0) {
            return rightCircle;
        }
        // if the right circle is illegal
        else if (rightCircle.radius < 0) {
            return leftCircle;
        } else {
            // if they both legal, return this one with the minimal radius
            return leftCircle.radius <= rightCircle.radius ? leftCircle : rightCircle;
        }
    }
}

Circle minCircleFromOne(Point point1, size_t size, Point **points) {
    //        Circle circle = twoPointsDiameter(point1, *points[0]);
    Circle circle = {point1, 0};
    for (int i = 0; i < size; ++i) {
        Point point2 = *points[i];
        // check if the circle that I initialized already enclose the current point
        if (!ifInsideCircle(circle, point2)) {
            if (circle.radius == 0) {
                circle = twoPointsDiameter(point1, point2);
            } else {
                circle = minCircleFromTwo(point1, point2, i + 1, points);
            }
        }
    }
    return circle;
}

#include <algorithm>

/// function that return the minimum circle of the given points
Circle findMinCircle(Point **points, size_t size) {
    //    if (size == 0) { return {Point(0, 0), 0}; }
    //    if (size == 1) { return {*points[0], 0}; }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(&points[0], &points[size], g);

    Circle tmp = Circle(Point(0, 0), -1);
    for (int j = 0; j < size; ++j) {
        Point p = *points[j];
        if (tmp.radius < 0 || !ifInsideCircle(tmp, p)) { tmp = minCircleFromOne(p, j + 1, points); }
    }
    return tmp;
}


#endif /* MINCIRCLE_H_ */
