//
// Created by exilio016 on 18/05/18.
//

#include "spider.h"
#include <GL/glut.h>

void Spider::draw() {
    glPushMatrix();
    glRotated(angle, 0, 1, 0);
    glTranslated(position->x, position->y, position->z);

    glPushMatrix(); //Draw spider torax
    glTranslated(-TORAXSIXE, 0, 0);
    glutSolidSphere(TORAXSIXE, 100, 100);
    glPopMatrix();

    glPushMatrix(); //Draw spider head
    glTranslated(TORAXSIXE/2 - TORAXSIXE/8, 0, 0);
    glutSolidSphere(TORAXSIXE/2, 100, 100);
    glPopMatrix();

    //TODO Draw spider legs

    glPopMatrix(); //Apply the first transformation;

}

Spider::Spider() {
    position = new point;
    position->x = 0; position->y = 2; position->z = 0;
    angle = 0;
}

void Spider::draw_leg(point *orig, double leg_ang, double artic_ang) {
    if(orig == nullptr) return;

    glPushMatrix();

}
