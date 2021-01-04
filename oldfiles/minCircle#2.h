//
//#ifndef MINCIRCLE2_H_
//#define MINCIRCLE2_H_
//
//#include <array>
//#include <iostream>
//#include <iterator>
//#include <limits>
//#include <random>
//#include <vector>
//
//
//using namespace std;
//
//// ------------ DO NOT CHANGE -----------
//class Point {
//public:
//    float x, y;
//    Point(float x, float y) : x(x), y(y) {}
//};
//
//class Circle {
//public:
//    Point center;
//    float radius;
//    Circle(Point c, float r) : center(c), radius(r) {}
//};
//// --------------------------------------
//
//// you may add helper functions here
//
/////function that return the distance between two points
//float dist(const Point &a, const Point &b) { return std::hypot(a.x - b.x, a.y - b.y); }
//
//const double EPSILON = 1 + 1e-14;
//
///// function that checks if the point inside or on the boundaries of the circle
//bool ifInsideCircle(const Circle &c, const Point &point) {
//    return dist(c.center, point) <= c.radius * EPSILON;
//}
//
//Point get_circle_center(float bx, float by, float cx, float cy) {
//    float B = bx * bx + by * by;
//    float C = cx * cx + cy * cy;
//    float D = bx * cy - by * cx;
//    return {(cy * B - by * C) / (2 * D), (bx * C - cx * B) / (2 * D)};
//}
//
//
//// Function to return a unique circle that
//// intersects three points
//Circle circle_from(const Point &A, const Point &B, const Point &C) {
//    Point I = get_circle_center(B.x - A.x, B.y - A.y, C.x - A.x, C.y - A.y);
//
//    I.x += A.x;
//    I.y += A.y;
//    return {I, dist(I, A)};
//}
//
//// Function to return the smallest circle
//// that intersects 2 points
//Circle circle_from(const Point &A, const Point &B) {
//    // Set the center to be the midpoint of A and B
//    Point C = {(A.x + B.x) / 2.0f, (A.y + B.y) / 2.0f};
//
//    // Set the radius to be half the distance AB
//    return {C, dist(A, B) / 2.0f};
//}
//
//// Function to check whether a circle
//// encloses the given points
//bool is_valid_circle(const Circle &c, const vector<const Point *> &P) {
//    // Iterating through all the points
//    // to check  whether the points
//    // lie inside the circle or not
//    for (auto p : P)
//        if (!ifInsideCircle(c, *p)) return false;
//    return true;
//}
//
//// Function to return the minimum enclosing
//// circle for N <= 3
//Circle min_circle_trivial(std::vector<const Point *> P) {
//    //assert(P.size() <= 3);
//    if (P.empty()) {
//        return {{0, 0}, 0};
//    } else if (P.size() == 1) {
//        return {*P[0], 0};
//    } else if (P.size() == 2) {
//        return circle_from(*P[0], *P[1]);
//    }
//
//    // To check if MEC can be determined
//    // by 2 points only
//    for (int i = 0; i < 3; i++) {
//        for (int j = i + 1; j < 3; j++) {
//            Circle c = circle_from(*P[i], *P[j]);
//            if (is_valid_circle(c, P)) return c;
//        }
//    }
//    return circle_from(*P[0], *P[1], *P[2]);
//}
//
//// Returns the MEC using Welzl's algorithm
//// Takes a set of input points P and a set R
//// points on the circle boundary.
//// n represents the number of points in P
//// that are not yet processed.
//Circle welzl_helper(std::vector<const Point *> P, std::vector<const Point *> R, size_t n) {
//    // Base case when all points processed or |R| = 3
//    if (n == 0 || R.size() == 3) { return min_circle_trivial(R); }
//
//    // Pick a random point randomly
//    int idx = rand() % n;
//    const Point *p = P[idx];
//
//    // Put the picked point at the end of P
//    // since it's more efficient than
//    // deleting from the middle of the vector
//    swap(P[idx], P[n - 1]);
//
//    // Get the MEC circle d from the
//    // set of points P - {p}
//    Circle d = welzl_helper(P, R, n - 1);
//
//    // If d contains p, return d
//    if (ifInsideCircle(d, *p)) { return d; }
//
//    // Otherwise, must be on the boundary of the MEC
//    R.push_back(p);
//
//    // Return the MEC for P - {p} and R U {p}
//    return welzl_helper(P, R, n - 1);
//}
//
//#include <algorithm>
//
//Circle findMinCircle(Point **points, size_t size) {
//    vector<const Point *> P_copy;
//    P_copy.reserve(size);
//    for (int i = 0; i < size; i++) { P_copy.push_back(points[i]); }
//
//    std::random_device rd;
//    std::mt19937 g(rd());
//    std::shuffle(P_copy.begin(), P_copy.end(), g);
//
//    std::vector<const Point *> R;
//    return welzl_helper(P_copy, R, P_copy.size());
//}
//
//// Driver code
//#endif//MINCIRCLE2_H_
