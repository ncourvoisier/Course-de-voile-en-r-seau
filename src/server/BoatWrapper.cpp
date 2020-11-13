#include <gf/RenderTarget.h>
#include <iostream>
#include "BoatWrapper.h"

namespace sail
{

    BoatWrapper::BoatWrapper(gf::Id playerId, gf::Vector2f position)
    : m_playerId(playerId)
    , m_position(position)
    , m_velocity(0, 0)
    {
        std::cout << "Mass of sail : " << boat.getMass() << std::endl;
    }

    void BoatWrapper::setVelocity(gf::Vector2f velocity)
    {
        m_velocity = velocity;
    }

    void BoatWrapper::update(float dt)
    {
        m_position += dt * 0.0001 *  m_velocity;
    }

    BoatData BoatWrapper::getBoatData()
    {
        return { m_playerId, m_position, m_velocity };
    }

    gf::Vector2f BoatWrapper::getVelocity()
    {
        return m_velocity;
    }

    gf::Vector2f BoatWrapper::getPosition()
    {
        return m_position;
    }

    void BoatWrapper::setPosition(const gf::Vector2f &mPosition)
    {
        m_position = mPosition;
    }

}