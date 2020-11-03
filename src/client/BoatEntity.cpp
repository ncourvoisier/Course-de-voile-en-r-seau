#include <gf/Shapes.h>
#include <gf/RenderTarget.h>
#include <iostream>

#include "BoatEntity.h"

namespace sail
{

    BoatEntity::BoatEntity(gf::Color4f color)
    : m_position({0, 0})
    , m_velocity({0, 0})
    , m_size(10.0f)
    , m_color(color)
    {

    }

    BoatEntity::~BoatEntity()
    {

    }

    void BoatEntity::update(gf::Time time)
    {
        //std::cout << "dt : " << time.asSeconds() << "\n";
        m_position += time.asSeconds() * m_velocity;
        //std::cout << "Velocity : x = " << m_velocity.x << ", y = " << m_velocity.y << "\n";
        //std::cout << "Pos : x = " << m_position.x << ", y = " << m_position.y << "\n";
    }

    void BoatEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
    {
        gf::RectangleShape shape({m_size, m_size});
        shape.setPosition(m_position);
        shape.setColor(m_color);
        shape.setAnchor(gf::Anchor::Center);
        target.draw(shape, states);
        //std::cout << "Render with : " << m_position.x << ", " << m_position.y << "\n";
    }

    gf::Vector2f BoatEntity::getPosition()
    {
        return m_position;
    }

    void BoatEntity::setPosition(const gf::Vector2f &mPosition)
    {
        m_position = mPosition;
    }

    gf::Vector2f BoatEntity::getVelocity()
    {
        return m_velocity;
    }

    void BoatEntity::setVelocity(const gf::Vector2f &mVelocity)
    {
        m_velocity = mVelocity;
    }

    ClientBoatData BoatEntity::getClientBoatData()
    {
        return { m_position, m_velocity };
    }

}