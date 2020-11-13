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
        void pullSheet(Boat& boat);
        void releaseSheet(Boat& boat);

    private:
        static constexpr int RudderStepsNumber = 5;
        static constexpr int SheetStepsNumber = 5;

        static constexpr double RudderMaxAngle = M_PI_4;
        static constexpr double SheetMaxLength = M_PI_2;

        static constexpr double RudderStep = RudderMaxAngle / RudderStepsNumber;
        static constexpr double SheetStep = SheetMaxLength / SheetStepsNumber;
    };

}

#endif //SAILINSANE_BOATCONTROL_H
