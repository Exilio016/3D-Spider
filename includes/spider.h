//
// Created by exilio016 on 18/05/18.
//

#ifndef SPIDER_SPIDER_H
#define SPIDER_SPIDER_H


#include "types.h"

class Spider {
private:
    const double TORAX_SIZE = 2;
    const double LEG_SIZE = 2;
    const double LEG_RADIUS = 0.15;
    const double MOVE_LENGTH = 1;
    const double ROTATE_ANG = 10;

    point *position;
    double angle;

    void draw_leg(point *orig, double leg_ang, double artic_ang, double x_ang, bool sideRight);

public:
    Spider();

    void draw();

    void move(bool forward);

    void rotate(bool right);
};


#endif //SPIDER_SPIDER_H
