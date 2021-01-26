#include "ServerBoat.h"

namespace sail
{

    ServerBoat::ServerBoat(gf::Id playerId, double x, double y)
    : Boat(x, y)
    , m_playerId(playerId)
    {

    }

    BoatData ServerBoat::getBoatData()
    {
        return { m_playerId, m_x, m_y, angle, sail_angle, rudder_angle, sheet_length, rotational_velocity, v };
    }

}