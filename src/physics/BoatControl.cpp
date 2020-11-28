#include <iostream>
#include <cmath>

#include "BoatControl.h"

namespace sail
{

    /*
     * Notes :
     * The boat stabilizes when the rudder angle is 0, in other cases it's just rotating without going anywhere
     * When the boat stabilizes, the sail angle becomes either 1 or -1 (if we look in Physics.cpp, it's when the sheet isn't tight, 1 being the sheet length)
     *
     * */

    void BoatControl::moveRudderRight(Boat &boat)
    {
        double newAngle = boat.getRudderAngle() + RudderStep;
        if (fabs(newAngle) < RudderStep)
            newAngle = 0;
        else if (newAngle > RudderMaxAngle)
            newAngle = RudderMaxAngle;
        boat.setRudderAngle(newAngle);
    }

    void BoatControl::moveRudderLeft(Boat &boat)
    {
        double newAngle = boat.getRudderAngle() - RudderStep;
        if (fabs(newAngle) < RudderStep)
            newAngle = 0;
        else if (newAngle < -RudderMaxAngle)
            newAngle = -RudderMaxAngle;
        boat.setRudderAngle(newAngle);
    }

    void BoatControl::sheetOut(Boat &boat)
    {
        double newLength = boat.getSheetLength() + SheetStep;
        if (newLength > SheetMaxLength)
            newLength = SheetMaxLength;
        std::cout << "moving with sheet : " << newLength << "\n";
        boat.setSheetLength(newLength);
    }

    void BoatControl::sheetIn(Boat &boat)
    {
        double newLength = boat.getSheetLength() - SheetStep;
        if (newLength < SheetMinLength)
            newLength = SheetMinLength;
        std::cout << "moving with sheet : " << newLength << "\n";
        boat.setSheetLength(newLength);
    }

}