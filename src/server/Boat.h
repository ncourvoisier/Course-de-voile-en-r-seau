#ifndef SAILINSANE_BOAT_H
#define SAILINSANE_BOAT_H

#include <gf/Shapes.h>
#include <gf/Vector.h>

namespace sail
{

    class Boat
    {
    public:
        Boat(gf::Vector2f position);
        void setVelocity(gf::Vector2f velocity);
        void update(float dt);

    private:
        gf::Vector2f m_position;
        gf::Vector2f m_velocity;
    };

}


#endif //SAILINSANE_BOAT_H
