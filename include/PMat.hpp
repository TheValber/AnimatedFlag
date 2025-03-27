#pragma once

#include "Point.hpp"
#include "Vect.hpp"

class PMat {
public:
    PMat(double m, Point pos, Vect vit, int type, double x_prop = 0, double y_prop = 0);
    void update(double h);

    void addForce(Vect f);

    Point getPos() const { return pos; }
    Vect getVit() const { return vit; }
    double getM() const { return m; }

    void setType(int type) { this->type = type; }

    Point* getPosPtr() { return &pos; }

    void draw() const;

private:
    double m;
    Point pos;
    Vect vit;
    Vect frc;
    int type;

    double x_prop;
    double y_prop;

    void updateLeapFrog(double h);
    void updateEulerExp(double h);
    void updateFixe(double h);
};
