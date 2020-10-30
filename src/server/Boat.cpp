#include <gf/RenderTarget.h>
#include "Boat.h"

namespace sail
{

    Boat::Boat(gf::Vector2f position)
    : m_position(position)
    , m_velocity(0, 0)
    {

    }

    void Boat::setVelocity(gf::Vector2f velocity) {
        m_velocity = velocity;
    }

    void Boat::update(float dt) {
        m_position += dt * m_velocity;
    }

}