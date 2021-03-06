//
// Created by exilio016 on 18/05/18.
//

#include "spider.h"
#include <GL/glut.h>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <types.h>
#include <spider.h>

#define BOXTAM 90

void rotate_point (point *p, double angle){
    double x,y,z;
    x = p->x;
    y = p->y;
    z = p->z;

    p->x = z*sin(angle) + x*cos(angle);
    p->y = y;
    p->z =  z*cos(angle) - x*sin(angle);
}
GLUquadric* getTexQuad(cv::Mat img, GLuint *textures){
    //Set spider
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.cols, img.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, img.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glEnable(GL_TEXTURE_2D);
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    return quad;
    glDisable(GL_TEXTURE_2D);
}

void Spider::draw() {
    glPushMatrix();
    glTranslated(position->x, position->y, position->z);
    glRotated(angle, 0, 1, 0);

    glColor3f(1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glEnable(GL_TEXTURE_2D); //Ativa textura
    glPushMatrix(); //Draw spider abdome
    glTranslated(-TORAX_SIZE, 0, 0);
    gluSphere(bodyQuad, TORAX_SIZE, 100, 100);
    glPopMatrix();

    glPushMatrix(); //Draw spider cephalothorax
    glTranslated(TORAX_SIZE/2 - TORAX_SIZE/8, 0, 0);
    gluSphere(bodyQuad, TORAX_SIZE/2, 100, 100);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D); //Desativa textura

    glColor3f(0.6, 0.4, 0);
    int k = 0;
    for (int i = 1; i < 12; i++) {
    glPushMatrix(); //Draw spider cephalothorax
    glTranslated(TORAX_SIZE/2 - TORAX_SIZE/8, 0, 0);
        if (i != 5 && i != 7 && i != 12 && i != 6) {

          point *o = new point;
         o->y = 0;

         o->x = (TORAX_SIZE/2) * cos(M_PI*i/6);
         o->z = (TORAX_SIZE/2) * sin(M_PI*i/6);

         if (i < 6)
            draw_leg(o, legs[k].leg_ang, legs[k].artic_ang, legs[k].x_ang, false);
         else
             draw_leg(o, legs[k].leg_ang, legs[k].artic_ang, legs[k].x_ang, true);

         k++;
      }
    glPopMatrix();
   }
    animate();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

Spider::Spider(GLuint *textures) {
    position = new point;
    position->x = 0; position->y = TORAX_SIZE - TORAX_SIZE/10; position->z = 0;
    angle = 0;

    iteration = 0;
    walking = false;
    currentState = stopped;
    oldState = stopped;
    this->textures = textures;
    printf("%p\n", textures);

    int k = 0;
    for (int i = 1; i < 12; i++) {
        if (i != 5 && i != 7 && i != 12 && i != 6) {
            double x_ang = (i == 1 || i == 11 ) ? 45 : 0;
            x_ang = (i == 8 ||i == 4) ? -45 : x_ang;

            legs[k].x_ang = x_ang;
            legs[k].artic_ang = 70;
            legs[k].leg_ang = -20;
            k++;
        }
    }

    texSpider = cv::imread("images/spider-body.jpg");
    cv::flip(texSpider, texSpider, 0);

    bodyQuad = getTexQuad(texSpider, textures);
}

void Spider::walk_left(double ang){
    legs[0].x_ang -= ang;
    legs[1].x_ang += ang;
    legs[2].x_ang -= ang;
    legs[3].x_ang += ang;

    legs[4].x_ang -= ang;
    legs[5].x_ang += ang;
    legs[6].x_ang -= ang;
    legs[7].x_ang += ang;

}

void Spider::walk_right(double ang){
    legs[0].x_ang += ang;
    legs[1].x_ang -= ang;
    legs[2].x_ang += ang;
    legs[3].x_ang -= ang;

    legs[4].x_ang += ang;
    legs[5].x_ang -= ang;
    legs[6].x_ang += ang;
    legs[7].x_ang -= ang;

}

void Spider::animate(){
    double ang = 0.25;
    if(currentState == walking_left){
        if(iteration <= MAX_ITERATION){
            walk_left(ang);
            iteration++;
        }
        else{
           iteration = 0;
           currentState = stopped;
           oldState = walking_left;
        }
    }
    if(currentState == walking_right){
        if(iteration <= MAX_ITERATION){
            walk_right(ang);
            iteration++;
        }
        else{
            iteration = 0;
            currentState = stopped;
            oldState = walking_right;
        }
    }
    else if (currentState == stopped){
        if(iteration <= MAX_ITERATION){
            walking = false;
            if(oldState == walking_right)
                walk_left(ang);

            else if(oldState == walking_left)
                walk_right(ang);
            iteration++;
        } else {
            if(walking){
                iteration =  0;
                if(oldState == walking_left) currentState = walking_right;
                else currentState = walking_left;
            }

        }
    }

}

void Spider::move(bool forward){
    int signal = (forward ? 1 : -1);
    point *p = new point;
    p->x = signal * MOVE_LENGTH;
    p->y = 0;
    p->z = 0;

    rotate_point(p, angle*M_PI/180);

    if(fabs(position->x + p->x) < (BOXTAM - TORAX_SIZE) && fabs(position->z + p->z) < (BOXTAM - TORAX_SIZE)){
      position->x += p->x;
      position->z += p->z;
      walking = true;
    }
}

void Spider::rotate(bool right){
    int signal = (right ? -1 : 1);
    angle += signal * ROTATE_ANG;
    if (angle > 360) angle -= 360;
    if (angle < -360) angle += 360;
    walking = true;
}

void Spider::draw_leg(point *orig, double leg_ang, double artic_ang, double x_ang, bool sideRight) {
    if(orig == nullptr) return;

    int side = (sideRight ? -1 : 1);

    glPushMatrix();
    glTranslated(orig->x, orig->y, orig->z);

    glPushMatrix();
    glRotated(side*x_ang, 0, 1, 0);
    glRotated(side*leg_ang, 1, 0, 0);
    glBegin(GL_LINES);
    glVertex3d(0, 0, 0);
    glVertex3d(0, 0, side*LEG_SIZE/2);
    glEnd();

    glPushMatrix();
    glTranslated(0, 0, side*LEG_SIZE/2);
    glRotated(side*artic_ang, 1, 0, 0);
    glBegin(GL_LINES);
    glVertex3d(0, 0, 0);
    glVertex3d(0, 0, side*LEG_SIZE/2);
    glEnd();
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();
}

point *Spider::getPosition() {
    return this->position;
}
