//
// Created by exilio016 on 18/05/18.
//

#ifndef SPIDER_SPIDER_H
#define SPIDER_SPIDER_H


#include "types.h"

class Spider {
private:
    const double TORAXSIXE = 2;
    const int LEGSIZE = 1;

    point *position;
    double angle;

    void draw_leg(point *orig, double leg_ang, double artic_ang);

public:
    Spider();

    void draw();

};


#endif //SPIDER_SPIDER_H
