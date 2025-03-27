#include "../include/Link.hpp"

#include <gfl.h>

Link::Link(PMat *M1, PMat *M2, int type, double k, double z, float r, float g, float b): M1(M1), M2(M2), type(type), k(k), z(z), r(r), g(g), b(b) {
    l0 = M1->getPos().dist(M2->getPos());
}

void Link::update() {
    switch (type)
    {
    case 0:
        updateDampedHook();
        break;
    case 1:
        updateGravity();
        break;
    default:
        break;
    }
}

void Link::updateDampedHook() {
    double d = M1->getPos().dist(M2->getPos());
    Vect u = (M2->getPos() - M1->getPos()) / d;
    // Vect f = u * (-k * (d - l0));
    Vect f = u * (-k * (d - l0)) - (M2->getVit() - M1->getVit()) * z;
    M1->addForce(-f);
    M2->addForce(f);
}

void Link::updateGravity() {
    Vect f = Vect(0, -9.81, 0) * M1->getM();
    M1->addForce(f);
    // M2->addForce(-f);
}

void Link::draw() const {
    glColor3f(r, g, b);
    
    glBegin(GL_LINES);
    glVertex3f(M1->getPos().getX(), M1->getPos().getY(), M1->getPos().getZ());
    glVertex3f(M2->getPos().getX(), M2->getPos().getY(), M2->getPos().getZ());
    glEnd();
}
