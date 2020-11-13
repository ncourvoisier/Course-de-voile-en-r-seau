#include "Wind.h"

namespace sail {

    Wind::Wind()
    : speed(4)
    , direction(M_PI_2)
    {

    }

    double Wind::getSpeed() const {
        return speed;
    }

    void Wind::setSpeed(double speed) {
        Wind::speed = speed;
    }

    double Wind::getDirection() const {
        return direction;
    }

    void Wind::setDirection(double direction) {
        Wind::direction = direction;
    }

}