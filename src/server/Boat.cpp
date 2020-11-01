#include <gf/RenderTarget.h>
#include "Boat.h"

namespace sail
{

    Boat::Boat(gf::Id playerId, gf::Vector2f position)
    : m_playerId(playerId)
    , m_position(position)
    , m_velocity(0, 0)
    {

    }

    void Boat::setVelocity(gf::Vector2f velocity)
    {
        m_velocity = velocity;
    }

    void Boat::update(float dt)
    {
        m_position += dt * m_velocity;
    }

    BoatData Boat::getBoatData()
    {
        return { m_playerId, m_position, m_velocity };
    }

    gf::Vector2f Boat::getVelocity()
    {
        return m_velocity;
    }

    gf::Vector2f Boat::getPosition()
    {
        return m_position;
    }

    void Boat::setPosition(const gf::Vector2f &mPosition)
    {
        m_position = mPosition;
    }

}