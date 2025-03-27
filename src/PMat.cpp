#include "../include/PMat.hpp"

#include <gfl.h>

PMat::PMat(double m, Point pos, Vect vit, int type, double x_prop, double y_prop): m(m), pos(pos), vit(vit), type(type), x_prop(x_prop), y_prop(y_prop) {
    frc = Vect(0, 0, 0);
}

void PMat::update(double h) {
    switch (type) {
        case 0:
            updateFixe(h);
            break;
        case 1:
            updateEulerExp(h);
            break;
        case 2:
            updateLeapFrog(h);
            break;
        default:
            break;
    }
}

void PMat::addForce(Vect f) {
    frc += f;
}

void PMat::updateLeapFrog(double h) {
    vit += frc * h / m;
    pos += vit * h;
    frc = Vect(0, 0, 0);
}

void PMat::updateEulerExp(double h) {
    pos += vit * h;
    vit += frc * h / m;
    frc = Vect(0, 0, 0);
}

void PMat::updateFixe(double h) {
    frc = Vect(0, 0, 0);
}

void PMat::draw() const {
    // gfl_Color4fv({1, 0, 0, 1});
    glColor3f(x_prop, y_prop, 1.0f); 
    glBegin(GL_POINTS);
    glVertex3f(pos.getX(), pos.getY(), pos.getZ());
    glEnd();
}
