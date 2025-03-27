#include "../include/Point.hpp"
#include <cmath>

Point::Point(double x, double y, double z) : x(x), y(y), z(z) {}

Point::Point() : x(0), y(0), z(0) {}

Point::Point(const Point &p) : x(p.x), y(p.y), z(p.z) {}

Point &Point::operator=(const Point &p) {
    x = p.x;
    y = p.y;
    z = p.z;
    return *this;
}

Point::Point(Point &&p) : x(p.x), y(p.y), z(p.z) {}

Point &Point::operator=(Point &&p) {
    x = p.x;
    y = p.y;
    z = p.z;
    return *this;
}

Point Point::operator+(const Point& p) {
    return Point(x + p.x, y + p.y, z + p.z);
}

Point Point::operator*(double k) {
    return Point(x * k, y * k, z * k);
}

Point Point::operator/(double k) {
    return Point(x / k, y / k, z / k);
}

Point Point::operator+=(const Point& p) {
    x += p.x;
    y += p.y;
    z += p.z;
    return *this;
}

Point Point::operator+(const Vect& v) {
    return Point(x + v.getX(), y + v.getY(), z + v.getZ());
}

Point Point::operator+=(const Vect& v) {
    x += v.getX();
    y += v.getY();
    z += v.getZ();
    return *this;
}

Vect Point::operator-(const Point& p) {
    return Vect(x - p.x, y - p.y, z - p.z);
}

double Point::dist(const Point &p) const {
    return sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y) + (z - p.z) * (z - p.z));
}
