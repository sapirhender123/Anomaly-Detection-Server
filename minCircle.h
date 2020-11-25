// ID1	ID2

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <limits>



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
    return ((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

/// function that checks if the point inside or on the boundaries of the circle
bool ifInsideCircle(const Circle &c, const Point &point) {
    return dist(c.center, point) <= c.radius;
}

//// function to get a circle defined by 3 points
Point getCircleCenter(float ax, float ay, float bx, float by) {
    float A = ax * ax + ay * ay;
    float B = bx * bx + by * by;
    float C = ax * by - ay * bx;
    return {(by * B - ay * C) / (2 * C), (ax * C - bx * B) / (2 * C)};
}

/// Function to return a circle that intersects three points
Circle circleFromThree(const Point &A, const Point &B, const Point &C) {
    Point c = getCircleCenter(B.x - A.x, B.y - A.y, C.x - A.x, C.y - A.y);
    c.x += A.x;
    c.y += A.y;
    return {C, dist(C, A)};
}

/// Function to return a circle that intersects two points
Circle circleFromTwo(const Point &A, const Point &B) {
    // Set the center to be the midpoint of A and B
    Point C = {(A.x + B.x) / 2, (A.y + B.y) / 2};

    // Set the radius to be half the distance AB
    return {C, dist(A, B) / 2};
}


/// Function to check if the given circle encloses the given points (that in the vector)
bool ifCircleEnclose(const Circle &c, Point **points, size_t size) {
    // passing all the points and check ig it inside the circle of not
    for (int i = 0; i < size; ++i) {
        if (!ifInsideCircle(c, *points[i])) return false;
    }
    return true;
}

/// function that return the minimum circle of the given points
Circle findMinCircle(Point **points, size_t size) {

    if (size == 0) { return Circle(Point(0, 0), 0); }
    if (size == 1) {
        return Circle(*points[0], 0);
    }

    // Circle from the two first points for start
    Circle C2 = Circle(Point((points[0]->x + points[1]->x) / 2,
                                 (points[0]->y + points[1]->y) / 2),
                           float(dist(*points[0], *points[1]) / 2));

    Circle tmp = Circle(Point(0, 0), 0);
    // for all the points, i will check if the point is inside the circle that already calculated in the previous iteration or not.
    // if yes, I will use it.
    for (int i = 3; i < size; ++i) {
        Circle prevC = Circle(Point((points[i-1]->x + points[i-2]->x) / 2,
                                (points[i-1]->y + points[i-2]->y) / 2),
                          float(dist(*points[i-1], *points[i-2]) / 2));
        if (ifInsideCircle(prevC, *points[i])) {
            tmp = prevC;
        } else {
            //solution with one point
            // for each point from j=2 until size check if it inside the circle
            // if not, solution with two points - with line...
        };

    }




    ////////////////////////////////////////////////////////////////////////////////////////

//    //const int infinity = std::numeric_limits<int>::max();
//
//    // Set initial MEC to have infinity radius
//    Circle mec = { { 0, 0 }, 429496725.0};
//
//    // Go over all pair of points
//    Circle tmp = {{0, 0}, 0};
//    for (int i = 0; i < size; i++) {
//        for (int j = i + 1; j < size; j++) {
//
//            // Get the smallest circle that
//            // intersects P[i] and P[j]
//            tmp = circleFromtwo(*points[i], *points[j]);
//
//            // Update MEC if tmp encloses all points
//            // and has a smaller radius
//
//            if (tmp.radius < mec.radius && ifCircleEnclose(tmp, points, size))
//                mec = tmp;
//        }
//    }
//
//    // Go over all triples of points
//    for (int i = 0; i < size; i++) {
//        for (int j = i + 1; j < size; j++) {
//            for (int k = j + 1; k < size; k++) {
//
//                // Get the circle that intersects P[i], P[j], P[k]
//                Circle tmp = circleFromThree(*points[i], *points[j], *points[k]);
//
//                // Update MEC if tmp encloses all points
//                // and has smaller radius
//                if (tmp.radius < mec.radius && ifCircleEnclose(tmp, points, size))
//                    mec = tmp;
//            }
//        }
//    }
//    return mec;
}



#endif /* MINCIRCLE_H_ */
