#include "Wind.h"

namespace sail {

    Wind::Wind() {
        speed = 4;
        direction = M_PI_2;
    }

    Wind::~Wind() {

    }

    double Wind::getSpeed() const {
        return speed;
    }

    void Wind::setSpeed(double speed) {
        this->speed = speed;
    }

    double Wind::getDirection() const {
        return direction;
    }

    void Wind::setDirection(double direction) {
        this->direction = direction;
    }

}