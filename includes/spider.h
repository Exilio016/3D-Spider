//
// Created by exilio016 on 18/05/18.
//

#ifndef SPIDER_SPIDER_H
#define SPIDER_SPIDER_H


#include "types.h"
#include <opencv2/opencv.hpp>
#include <GL/glut.h>

typedef enum {
    walking_right,
    stopped,
    walking_left
}State;

class Spider {
private:
    const double TORAX_SIZE = 2;
    const double LEG_SIZE = 5;
    const double LEG_RADIUS = 2;
    const double MOVE_LENGTH = 0.5;
    const double ROTATE_ANG = 10;
    const int MAX_ITERATION = 24;
    cv::Mat texSpider;
    GLUquadric *bodyQuad;

    point *position;
    double angle;
    bool walking;
    int iteration;
    State currentState;
    State oldState;

    typedef struct {
        double x_ang;
        double leg_ang;
        double artic_ang;
    }leg_rotate;

    leg_rotate legs[8];

    void draw_leg(point *orig, double leg_ang, double artic_ang, double x_ang, bool sideRight);

public:
    Spider();

    void draw();

    void move(bool forward);

    void rotate(bool right);

    point *getPosition();

    void animate();

    void walk_left(double ang);

    void walk_right(double ang);
};


#endif //SPIDER_SPIDER_H
