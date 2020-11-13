#ifndef SAILINSANE_BOATWRAPPER_H
#define SAILINSANE_BOATWRAPPER_H

#include <gf/Shapes.h>
#include <gf/Vector.h>
#include "../Protocol.h"
#include "../physics/Boat.h"
#include "../physics/Wind.h"
#include "../physics/Physics.h"

namespace sail
{

    class BoatWrapper
    {
    public:
        BoatWrapper() = default;
        BoatWrapper(gf::Id playerId, gf::Vector2f position);
        void setVelocity(gf::Vector2f velocity);
        gf::Vector2f getVelocity();
        gf::Vector2f getPosition();
        void setPosition(const gf::Vector2f &mPosition);
        void update(float dt);
        BoatData getBoatData();
        void trajectoryCalcul(float dt);

    private:
        gf::Id m_playerId;
        gf::Vector2f m_position;
        gf::Vector2f m_velocity;
        Boat boat;
        Wind wind;
    };

}


#endif //SAILINSANE_BOATWRAPPER_H
