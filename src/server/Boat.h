#ifndef SAILINSANE_BOAT_H
#define SAILINSANE_BOAT_H

#include <gf/Shapes.h>
#include <gf/Vector.h>
#include "../Protocol.h"

namespace sail
{

    class Boat
    {
    public:
        Boat() = default;

        Boat(gf::Id playerId, gf::Vector2f position);

        void setVelocity(gf::Vector2f velocity);

        void update(float dt);

        BoatData getBoatData();

    private:
        gf::Id m_playerId;

        gf::Vector2f m_position;

        gf::Vector2f m_velocity;
    };

}


#endif //SAILINSANE_BOAT_H
