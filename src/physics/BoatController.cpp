#include <iostream>
#include <cmath>

#include "BoatController.h"
#include "Wind.h"
#include "../Constants.h"
#include "Physics.h"

namespace sail
{

    /*
     * Notes :
     * The boat stabilizes when the rudder angle is 0, in other cases it's just rotating without going anywhere
     * When the boat stabilizes, the sail angle becomes either 1 or -1 (if we look in Physics.cpp, it's when the sheet isn't tight, 1 being the sheet length)
     * -> the latest behavior was caused by a wrong interpretation of the sheet purpose, the sheet length defines how much liberty we give to the sail,
     * it goes from 0 (no liberty, the sail is centered) to 1 (total liberty, the sail angle will follow the wind direction)
     *
     * Interesting information about how to control a sailing boat here : https://sites.google.com/a/washingtonyachtclub.org/new-member-guide/intro-to-sailing
     *
     * Solution :
     * In order to control the boat, we need to slightly move the rudder, let the boat rotate, the set the rudder position to 0,
     * this will set the boat back on a trajectory
     *
     * ==> The boat is stable on a trajectory only when the sheet isn't tight, the sail angle then becomes [ sign of the apparent wind * length of the sheet ]
     *
     * */

    void BoatController::moveRudderRight(Boat &boat)
    { // TODO : a problem remains, the boat control sensibility makes it hard to send 1 single key control, hard to center the rudder on 0
        double angle = boat.getRudderAngle();
        if (angle < 0 && angle > - 2 * RudderStep)
        {
            boat.setRudderAngle(0);
            return;
        }
        double newAngle = angle + RudderStep;
        if (newAngle > RudderMaxAngle)
            newAngle = RudderMaxAngle;
        boat.setRudderAngle(newAngle);
    }

    void BoatController::moveRudderLeft(Boat &boat)
    {
        double angle = boat.getRudderAngle();
        if (angle > 0 && angle < 2 * RudderStep)
        {
            boat.setRudderAngle(0);
            return;
        }
        double newAngle = angle - RudderStep;
        if (newAngle < -RudderMaxAngle)
            newAngle = -RudderMaxAngle;
        boat.setRudderAngle(newAngle);
    }

    void BoatController::centerRudder(Boat &boat)
    {
        boat.setRudderAngle(0);
    }

    void BoatController::sheetOut(Boat &boat)
    {
        double newLength = boat.getSheetLength() + SheetStep;
        if (newLength > SheetMaxLength)
            newLength = SheetMaxLength;
        boat.setSheetLength(newLength);
    }

    void BoatController::sheetIn(Boat &boat)
    {
        double newLength = boat.getSheetLength() - SheetStep;
        if (newLength < SheetMinLength)
            newLength = SheetMinLength;
        boat.setSheetLength(newLength);
    }

    void BoatController::processPlayerAction(Boat &boat, const PlayerAction& action)
    {
        switch (action.sailAction)
        {
            case PlayerAction::Type::Right:
            {
                sheetOut(boat);
                break;
            }
            case PlayerAction::Type::Left:
            {
                sheetIn(boat);
                break;
            }
        }

        switch (action.rudderAction)
        {
            case PlayerAction::Type::Right:
            {
                moveRudderRight(boat);
                break;
            }
            case PlayerAction::Type::Left:
            {
                moveRudderLeft(boat);
                break;
            }
            case PlayerAction::Type::Center:
            {
                centerRudder(boat);
            }
        }
    }

    void BoatController::updateBoatPosition(Boat& boat, gf::Array2D<float>& windSpeed, gf::Array2D<float>& windDirection, gf::Time dt)
    {
        constexpr double physicGranularity = 0.01; // Any granularity above this one can * sometimes * creates very unpredictable movements
        double loopsAmount = dt.asSeconds() / physicGranularity;

        auto col = static_cast<unsigned>(boat.getLongitude() / TileDegree);
        auto row = static_cast<unsigned>(boat.getLatitude() / TileDegree);

        Wind wind(windSpeed({row, col}), windDirection({row, col}));

        for (int i = 0; i < loopsAmount; i++)
        {
            sailing_physics_update(boat, wind, physicGranularity);
        }
    }

}