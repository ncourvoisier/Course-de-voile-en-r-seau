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
     * Solutions :
     * - In order to control the boat, we need to slightly move the rudder, let the boat rotate, the set the rudder position to 0,
     * this will set the boat back on a trajectory
     * - An other solution consists in centering automatically the rudder (which we did)
     *
     * ==> The boat is stable on a trajectory only when the sheet isn't tight, the sail angle then becomes [ sign of the apparent wind * length of the sheet ]
     *
     * */

    BoatController::BoatController(World& world)
    : m_world(world)
    {
    }

    void BoatController::moveRudderRight(Boat &boat)
    {
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
            default:
                break;
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
            default:
                break;
        }
    }

    bool BoatController::updateBoatPosition(Boat& boat, gf::Time dt)
    {
        constexpr double physicGranularity = 0.01; // Any granularity above this one can * sometimes * creates very unpredictable movements
        double loopsAmount = dt.asSeconds() / physicGranularity;

        auto col = static_cast<unsigned>(boat.getLongitude() / TileDegree);
        auto row = static_cast<unsigned>(boat.getLatitude() / TileDegree);

        Wind wind(m_world.getWindSpeed()({row, col}), m_world.getWindDirection()({row, col}));

        for (int i = 0; i < loopsAmount; i++)
        {
            sailing_physics_update(boat, wind, physicGranularity);
        }

        if (isOnLand(boat.getLongitude(), boat.getLatitude()))
        {
            boat.reset(m_world.getStartingPosition().x,
                       m_world.getStartingPosition().y);
            return false;
        }

        return true;
    }

    bool BoatController::isOnLand(double x, double y)
    {
        if (x < 0 || x >= 0.1 || y < 0 || y >= 0.1)
            return true;

        auto col = static_cast<unsigned>(x / TileDegree);
        auto row = static_cast<unsigned>(y / TileDegree);

        return m_world.getTerrain()({ row, col }) > 0.5f;
    }

}