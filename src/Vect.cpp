#include "../include/Vect.hpp"

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
