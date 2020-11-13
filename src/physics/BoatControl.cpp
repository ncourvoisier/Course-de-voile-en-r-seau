#include "BoatControl.h"

namespace sail
{

    void BoatControl::moveRudderRight(Boat &boat)
    {
        boat.setRudderAngle(
                boat.getRudderAngle()
                + RudderStep);
    }

    void BoatControl::moveRudderLeft(Boat &boat)
    {
        boat.setRudderAngle(
                boat.getRudderAngle()
                - RudderStep);
    }

    void BoatControl::moveSailRight(Boat &boat)
    {
        boat.setSailAngle(
                boat.getSailAngle()
                + SailStep);
    }

    void BoatControl::moveSailLeft(Boat &boat)
    {
        boat.setSailAngle(
                boat.getSailAngle()
                - SailStep);
    }

}