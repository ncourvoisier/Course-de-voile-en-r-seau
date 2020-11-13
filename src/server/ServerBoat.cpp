#include <gf/RenderTarget.h>
#include <iostream>
#include "ServerBoat.h"

namespace sail
{

    ServerBoat::ServerBoat(gf::Id playerId, gf::Vector2f position)
    : m_playerId(playerId)
    {
        //std::cout << "Mass of sail : " << boat.getMass() << std::endl;
    }

    BoatData ServerBoat::getBoatData()
    {
        return { m_playerId, x, y, angle, sail_angle, rudder_angle };
    }

}