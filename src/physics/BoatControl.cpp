#include <iostream>
#include "BoatControl.h"

namespace sail
{

    void BoatControl::moveRudderRight(Boat &boat)
    {
        double newAngle = boat.getRudderAngle() + RudderStep;
        if (newAngle > RudderMaxAngle)
            newAngle = RudderMaxAngle;
        boat.setRudderAngle(newAngle);
    }

    void BoatControl::moveRudderLeft(Boat &boat)
    {
        double newAngle = boat.getRudderAngle() - RudderStep;
        if (newAngle < -RudderMaxAngle)
            newAngle = -RudderMaxAngle;
        boat.setRudderAngle(newAngle);
    }

    void BoatControl::pullSheet(Boat &boat)
    {
        double newLength = boat.getSheetLength() + SheetStep;
        if (newLength >= SheetMaxLength)
            newLength = SheetMaxLength;
        std::cout << "moving with sheet : " << newLength << "\n";
        boat.setSheetLength(newLength);
    }

    void BoatControl::releaseSheet(Boat &boat)
    {
        double newLength = boat.getSheetLength() - SheetStep;
        if (newLength <= -SheetMaxLength)
            newLength = -SheetMaxLength;
        std::cout << "moving with sheet : " << newLength << "\n";
        boat.setSheetLength(newLength);
    }

}