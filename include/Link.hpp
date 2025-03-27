#pragma once

#include "PMat.hpp"

class Link {
public:
    Link(PMat *M1, PMat *M2, int type, double k, double z, float r = 1.0f, float g = 0.0f, float b = 0.0f);
    void update();

    void draw() const;

    void setWind(Vect wind);

private:
    PMat *M1;
    PMat *M2;
    int type;
    double k;
    double z;
    double l0;
    Vect wind;

    void updateDampedHook();

    void updateGravity();

    void updateWind();

    float r;
    float g;
    float b;
};
