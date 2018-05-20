//
// Created by exilio016 on 18/05/18.
//

#include "spider.h"
#include <GL/glut.h>
#include <math.h>
#include <types.h>


/* i j k
 * x y h
 * r 0 h
 *
 * y*h(i) - x*h+r*h(j) -r*y(k)
 */

void rotate_point (point *p, double angle){
    p->z =  p->z * cos(angle*M_PI/180) + p->z * sin(angle*M_PI/180);
    p->x = p->x * -sin(angle*M_PI/180) + p->x * cos(angle*M_PI/180);
}

void draw_cylinder(GLdouble radius, GLdouble height)
{
    GLdouble x              = 0.0;
    GLdouble y              = 0.0;
    GLdouble angle          = 0.0;
    GLdouble angle_stepsize = 0.1;

    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
    while( angle < 2*M_PI ) {
        x = radius * cos(angle);
        y = radius * sin(angle);
        glNormal3d(-y * height, x*height, 0);
        glVertex3d(x, y , height);
        glVertex3d(x, y , 0.0);
        angle = angle + angle_stepsize;
    }
    glVertex3d(radius, 0.0, height);
    glVertex3d(radius, 0.0, 0.0);
    glEnd();

    glBegin(GL_POLYGON);
    angle = 0.0;
    while( angle < 2*M_PI ) {
        x = radius * cos(angle);
        y = radius * sin(angle);
        glNormal3d(y*height, height*(radius-x), -radius*y);
        glVertex3d(x, y , height);
        angle = angle + angle_stepsize;
    }
    glVertex3d(radius, 0.0, height);
    glEnd();
}

void Spider::draw() {
    glColor3f(1, 0, 0);
    glPushMatrix();
    glTranslated(position->x, position->y, position->z);
    glRotated(angle, 0, 1, 0);

    glPushMatrix(); //Draw spider abdome
    glTranslated(-TORAX_SIZE, 0, 0);
    glutSolidSphere(TORAX_SIZE, 100, 100);
    glPopMatrix();

    glPushMatrix(); //Draw spider cephalothorax
    glTranslated(TORAX_SIZE/2 - TORAX_SIZE/8, 0, 0);
    glutSolidSphere(TORAX_SIZE/2, 100, 100);
    glPopMatrix();

    //TODO Draw spider legs
    for (int i = 1; i < 12; i++) {
    glPushMatrix(); //Draw spider cephalothorax
    glTranslated(TORAX_SIZE/2 - TORAX_SIZE/8, 0, 0);
       if (i != 5 && i != 7 && i != 12 && i != 6) {
          point *o = new point;
         o->y = 0;
    
         o->x = (TORAX_SIZE/2) * cos(M_PI*i/6);
         o->z = (TORAX_SIZE/2) * sin(M_PI*i/6);

         double x_ang = (i == 1 || i == 11 ) ? 45 : 0;
         x_ang = (i == 8 ||i == 4) ? -45 : x_ang;

         if (i < 6)
            draw_leg(o, 45, 45, x_ang, false);
         else
            draw_leg(o, 45, 45, x_ang, true);
      }
    glPopMatrix();
    }

    glPopMatrix();
}

Spider::Spider() {
    position = new point;
    position->x = 0; position->y = 0; position->z = 0;
    angle = 0;
}

void Spider::move(bool forward){
    int signal = (forward ? 1 : -1);
    point *p = new point;
    p->x = signal * MOVE_LENGTH;
    p->y = 0;
    p->z = 0;

    rotate_point(p, angle);

    position->x += p->x;
    position->z += p->z;
}

void Spider::rotate(bool right){
    int signal = (right ? -1 : 1);
    angle += signal * ROTATE_ANG;
    if (angle > 360) angle -= 360;
    if (angle < -360) angle += 360;
}

void Spider::draw_leg(point *orig, double leg_ang, double artic_ang, double x_ang, bool sideRight) {
    if(orig == nullptr) return;

    int side = (sideRight ? -1 : 1);

    glPushMatrix();
    glTranslated(orig->x, orig->y, orig->z);

    double ym = LEG_SIZE/2 * sin(leg_ang);
    double zm = LEG_SIZE/2 * cos(leg_ang);
    double xm = zm * sin(x_ang);

    double y = ym - LEG_SIZE/2 * sin(artic_ang);
    double z = zm +  LEG_SIZE/2 * cos(artic_ang);
    double x = z * sin(x_ang);

    glLineWidth(LEG_RADIUS);
    glBegin(GL_LINES);
    glVertex3d(0, 0, 0);
    glVertex3d(xm, ym, side*zm);
    glVertex3d(xm, ym, side *zm);
    glVertex3d(x, y, side*z);
    glEnd();
    glLineWidth(1);

    glPopMatrix();
}
