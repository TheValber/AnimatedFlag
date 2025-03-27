#pragma once

#include <gfl.h>

#include "Vect.hpp"

class Point {
public:
    Point(double x, double y, double z);
    Point();

    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }

    double* getYPtr() { return &y; }

    Point(const Point &p);
    Point &operator=(const Point &p);
    Point(Point &&p);
    Point &operator=(Point &&p);

    Point operator+(const Point& p);
    Point operator*(double k);
    Point operator/(double k);
    Point operator+=(const Point& p);
    Point operator+(const Vect& v);
    Point operator+=(const Vect& v);
    Vect operator-(const Point& p);

    double dist(const Point &p) const;

private:
    double x;
    double y;
    double z;
};

