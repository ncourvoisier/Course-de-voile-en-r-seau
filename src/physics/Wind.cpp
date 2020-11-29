#include "Wind.h"

namespace sail {

    Wind::Wind()
    : m_speed(4)
    , m_direction(-M_PI_2)
    {

    }

    Wind::Wind(double speed, double direction)
    : m_speed(speed)
    , m_direction(direction)
    {

    }

    double Wind::getSpeed() const {
        return m_speed;
    }

    void Wind::setSpeed(double speed) {
        Wind::m_speed = speed;
    }

    double Wind::getDirection() const {
        return m_direction;
    }

    void Wind::setDirection(double direction) {
        Wind::m_direction = direction;
    }

}