#include "../include/Vect.hpp"
#include <cmath>

Vect::Vect(double x, double y, double z) : x(x), y(y), z(z) {}

Vect::Vect() : x(0), y(0), z(0) {}

Vect::Vect(const Vect &v) : x(v.x), y(v.y), z(v.z) {}

Vect &Vect::operator=(const Vect &v) {
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}

Vect::Vect(Vect &&v) : x(v.x), y(v.y), z(v.z) {}

Vect &Vect::operator=(Vect &&v) {
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}

Vect Vect::operator+(const Vect& v) {
    return Vect(x + v.x, y + v.y, z + v.z);
}

Vect Vect::operator*(double k) {
    return Vect(x * k, y * k, z * k);
}

Vect Vect::operator/(double k) {
    return Vect(x / k, y / k, z / k);
}

Vect Vect::operator+=(const Vect& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

Vect Vect::operator-() {
    return Vect(-x, -y, -z);
}

Vect Vect::operator-(const Vect& v) {
    return Vect(x - v.x, y - v.y, z - v.z);
}

double Vect::norm() const {
    return std::sqrt(x * x + y * y + z * z);
}

Vect Vect::normalized() const {
    double n = norm();
    if (n == 0) return Vect(0, 0, 0);
    return Vect(x / n, y / n, z / n);
}

std::ostream& operator<<(std::ostream& os, const Vect& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}
