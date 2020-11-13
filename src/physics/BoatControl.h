#ifndef SAILINSANE_BOATCONTROL_H
#define SAILINSANE_BOATCONTROL_H

#include <cmath>

#include "Boat.h"

namespace sail
{

    class BoatControl
    {
    public:
        void moveRudderRight(Boat& boat);
        void moveRudderLeft(Boat& boat);
        void moveSailRight(Boat& boat);
        void moveSailLeft(Boat& boat);

    private:
        static constexpr int RudderStepsNumber = 10;
        static constexpr int SailStepsNumber = 20;

        static constexpr double RudderStep = M_PI / RudderStepsNumber;
        static constexpr double SailStep = M_PI / SailStepsNumber;
    };

}

#endif //SAILINSANE_BOATCONTROL_H
