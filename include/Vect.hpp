#pragma once

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

    Vect operator+(const Vect& v);
    Vect operator*(double k);
    Vect operator/(double k);
    Vect operator+=(const Vect& v);
    Vect operator-();
    Vect operator-(const Vect& v);

private:
    double x;
    double y;
    double z;
};

