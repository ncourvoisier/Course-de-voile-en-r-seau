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
        void sheetOut(Boat& boat);
        void sheetIn(Boat& boat);

    private:
        static constexpr int RudderStepsNumber = 10;
        static constexpr int SheetStepsNumber = 10;

        static constexpr double RudderMaxAngle = M_PI_4;
        static constexpr double SheetMaxLength = 1.0;
        static constexpr double SheetMinLength = 0.0;

        static constexpr double RudderStep = RudderMaxAngle / RudderStepsNumber;
        static constexpr double SheetStep = (SheetMaxLength - SheetMinLength) / SheetStepsNumber;
    };

}

#endif //SAILINSANE_BOATCONTROL_H
