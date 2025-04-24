#pragma once

#include <ostream>

class Vect {
public:
    Vect(double x, double y, double z);
    Vect();

    Vect(const Vect &v);
    Vect &operator=(const Vect &v);
    Vect(Vect &&v);
    Vect &operator=(Vect &&v);

    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }

    void setX(double x) { this->x = x; }
    void setY(double y) { this->y = y; }
    void setZ(double z) { this->z = z; }

    Vect operator+(const Vect& v);
    Vect operator*(double k);
    Vect operator/(double k);
    Vect operator+=(const Vect& v);
    Vect operator-();
    Vect operator-(const Vect& v);

    double norm() const;

    Vect normalized() const;

    friend std::ostream& operator<<(std::ostream& os, const Vect& v);

private:
    double x;
    double y;
    double z;
};

